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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ <termios.h>
#যদি HAVE_STROPTS_H
# include <stropts.h>
#endif
#অন্তর্ভাৱ <sys/ioctl.h>

#ifdef WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#endif

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <setjmp.h>
#অন্তর্ভাৱ <pwd.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <signal.h>

#যদি HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#ifndef SA_RESTART
# define SA_RESTART 0
#endif

#অন্তর্ভাৱ <fnmatch.h>

/* Gnulib includes.  */
#অন্তর্ভাৱ "acl.h"
#অন্তর্ভাৱ "areadlink.h"
#অন্তর্ভাৱ "argmatch.h"
#অন্তর্ভাৱ "assure.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "c-strcase.h"
#অন্তর্ভাৱ "canonicalize.h"
#অন্তর্ভাৱ "dev-ino.h"
#অন্তর্ভাৱ "filemode.h"
#অন্তর্ভাৱ "filenamecat.h"
#অন্তর্ভাৱ "filevercmp.h"
#অন্তর্ভাৱ "hard-locale.h"
#অন্তর্ভাৱ "hash.h"
#অন্তর্ভাৱ "human.h"
#অন্তর্ভাৱ "idcache.h"
#অন্তর্ভাৱ "mbswidth.h"
#অন্তর্ভাৱ "mpsort.h"
#অন্তর্ভাৱ "obstack.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "stat-size.h"
#অন্তর্ভাৱ "stat-time.h"
#অন্তর্ভাৱ "strftime.h"
#অন্তর্ভাৱ "xdectoint.h"
#অন্তর্ভাৱ "xgethostname.h"
#অন্তর্ভাৱ "xmemdup0.h"
#অন্তর্ভাৱ "xstrtol-error.h"
#অন্তর্ভাৱ "xstrtol.h"

#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "dircolors.h"
#অন্তর্ভাৱ "ls.h"
#অন্তর্ভাৱ "statx.h"
#অন্তর্ভাৱ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#ifdef HAVE_CAP
# include <sys/capability.h>
#endif

#যদি HAVE_LINUX_XATTR_H
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
#define obstack_chunk_free মুক্ত

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

#যদি READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_সংখ্যা
#অন্যথা
# define RELIABLE_D_INO(dp) D_INO (dp)
#endif

#যদি ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#endif

গণনা filetype
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
গণনা { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
স্থির ৱর্ণ ধ্রুৱ filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
স্থির unsigned ৱর্ণ ধ্রুৱ filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
স্থির ৱর্ণ ধ্রুৱ d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

গণনা acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

সংরচনা fileinfo
  {
    /* The file name.  */
    ৱর্ণ *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    ৱর্ণ *linkname;

    /* For terminal hyperlinks. */
    ৱর্ণ *absolute_name;

    সংরচনা stat stat;

    গণনা filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    ৱর্ণ *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    গণনা acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    পূর্ণাঙ্ক quoted;

    /* Cached screen width (including quoting).  */
    size_t width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

সংরচনা bin_str
  {
    size_t len;			/* Number of bytes */
    ৱর্ণ ধ্রুৱ *string;		/* Pointer to the same */
  };

#যদি ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#endif

স্থির size_t quote_name (ৱর্ণ ধ্রুৱ *name,
                          সংরচনা quoting_options ধ্রুৱ *options,
                          পূর্ণাঙ্ক needs_general_quoting,
                          ধ্রুৱ সংরচনা bin_str *color,
                          bool allow_pad, সংরচনা obstack *স্তূপ,
                          ৱর্ণ ধ্রুৱ *absolute_name);
স্থির size_t quote_name_buf (ৱর্ণ **inbuf, size_t bufsize, ৱর্ণ *name,
                              সংরচনা quoting_options ধ্রুৱ *options,
                              পূর্ণাঙ্ক needs_general_quoting, size_t *width,
                              bool *pad);
স্থির পূর্ণাঙ্ক decode_switches (পূর্ণাঙ্ক argc, ৱর্ণ **argv);
স্থির bool file_ignored (ৱর্ণ ধ্রুৱ *name);
স্থির uintmax_t gobble_file (ৱর্ণ ধ্রুৱ *name, গণনা filetype type,
                              ino_t inode, bool command_সাৰি_arg,
                              ৱর্ণ ধ্রুৱ *dirname);
স্থির ধ্রুৱ সংরচনা bin_str * get_color_indicator (ধ্রুৱ সংরচনা fileinfo *f,
                                                   bool symlink_target);
স্থির bool print_color_indicator (ধ্রুৱ সংরচনা bin_str *ind);
স্থির শূন্য put_indicator (ধ্রুৱ সংরচনা bin_str *ind);
স্থির শূন্য add_ignore_pattern (ৱর্ণ ধ্রুৱ *pattern);
স্থির শূন্য attach (ৱর্ণ *dest, ৱর্ণ ধ্রুৱ *dirname, ৱর্ণ ধ্রুৱ *name);
স্থির শূন্য clear_files (শূন্য);
স্থির শূন্য extract_dirs_from_files (ৱর্ণ ধ্রুৱ *dirname,
                                     bool command_সাৰি_arg);
স্থির bool get_link_name (ৱর্ণ ধ্রুৱ *filename, সংরচনা fileinfo *f,
                           bool command_সাৰি_arg);
স্থির শূন্য indent (size_t from, size_t to);
স্থির idx_t calculate_columns (bool by_columns);
স্থির শূন্য print_current_files (শূন্য);
স্থির শূন্য print_dir (ৱর্ণ ধ্রুৱ *name, ৱর্ণ ধ্রুৱ *realname,
                       bool command_সাৰি_arg);
স্থির size_t print_file_name_and_frills (ধ্রুৱ সংরচনা fileinfo *f,
                                          size_t start_col);
স্থির শূন্য print_horizontal (শূন্য);
স্থির পূর্ণাঙ্ক format_user_width (uid_t u);
স্থির পূর্ণাঙ্ক format_group_width (gid_t g);
স্থির শূন্য print_long_format (ধ্রুৱ সংরচনা fileinfo *f);
স্থির শূন্য print_many_per_সাৰি (শূন্য);
স্থির size_t print_name_with_quoting (ধ্রুৱ সংরচনা fileinfo *f,
                                       bool symlink_target,
                                       সংরচনা obstack *স্তূপ,
                                       size_t start_col);
স্থির শূন্য prep_non_filename_text (শূন্য);
স্থির bool print_type_indicator (bool stat_ok, mode_t mode,
                                  গণনা filetype type);
স্থির শূন্য print_with_separator (ৱর্ণ sep);
স্থির শূন্য queue_directory (ৱর্ণ ধ্রুৱ *name, ৱর্ণ ধ্রুৱ *realname,
                             bool command_সাৰি_arg);
স্থির শূন্য sort_files (শূন্য);
স্থির শূন্য parse_ls_color (শূন্য);

স্থির size_t quote_name_width (ৱর্ণ ধ্রুৱ *name,
                                সংরচনা quoting_options ধ্রুৱ *options,
                                পূর্ণাঙ্ক needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
গণনা { INITIAL_TABLE_SIZE = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
স্থির Hash_table *active_dir_set;

#define LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
স্থির সংরচনা fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
স্থির idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
স্থির idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
স্থির bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
স্থির bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
স্থির শূন্য **sorted_file;
স্থির size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

স্থির bool color_symlink_as_referent;

স্থির ৱর্ণ ধ্রুৱ *hostname;

/* Mode of appropriate file for coloring.  */
স্থির mode_t
file_or_link_mode (সংরচনা fileinfo ধ্রুৱ *file)
{
  ঘূৰাই (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

সংরচনা pending
  {
    ৱর্ণ *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    ৱর্ণ *realname;
    bool command_সাৰি_arg;
    সংরচনা pending *next;
  };

স্থির সংরচনা pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

স্থির সংরচনা timespec current_time;

স্থির bool print_scontext;
স্থির ৱর্ণ UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

স্থির bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

স্থির পূর্ণাঙ্ক inode_সংখ্যা_width;
স্থির পূর্ণাঙ্ক block_size_width;
স্থির পূর্ণাঙ্ক nlink_width;
স্থির পূর্ণাঙ্ক scontext_width;
স্থির পূর্ণাঙ্ক owner_width;
স্থির পূর্ণাঙ্ক group_width;
স্থির পূর্ণাঙ্ক author_width;
স্থির পূর্ণাঙ্ক major_device_সংখ্যা_width;
স্থির পূর্ণাঙ্ক minor_device_সংখ্যা_width;
স্থির পূর্ণাঙ্ক file_size_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

গণনা format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_সাৰি,		/* -1 */
    many_per_সাৰি,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

স্থির গণনা format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
গণনা time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", রিক্ত
};
স্থির গণনা time_style ধ্রুৱ time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

গণনা time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

স্থির গণনা time_type time_type;
স্থির bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

গণনা sort_type
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

স্থির গণনা sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

স্থির bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

স্থির bool print_owner = true;

/* True means to display author information.  */

স্থির bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

স্থির bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

স্থির bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

স্থির bool print_block_size;

/* Human-readable options for output, when printing block counts.  */
স্থির পূর্ণাঙ্ক human_output_opts;

/* The units to use when printing block counts.  */
স্থির uintmax_t output_block_size;

/* Likewise, but for file sizes.  */
স্থির পূর্ণাঙ্ক file_human_output_opts;
স্থির uintmax_t file_output_block_size = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
স্থির bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

গণনা indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

স্থির গণনা indicator_style indicator_style;

/* Names of indicator styles.  */
স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", রিক্ত
};
স্থির গণনা indicator_style ধ্রুৱ indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

স্থির bool print_with_color;

স্থির bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

স্থির bool used_color = false;

গণনা when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

গণনা Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_সাৰি_ARGUMENTS,	/* -H */
    DEREF_COMMAND_সাৰি_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

গণনা indicator_no
  {
    C_বাম, C_ডান, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

স্থির ৱর্ণ ধ্রুৱ indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

সংরচনা color_ext_type
  {
    সংরচনা bin_str ext;		/* The extension we're looking for */
    সংরচনা bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    সংরচনা color_ext_type *next;	/* Next in list */
  };

স্থির সংরচনা bin_str color_indicator[] =
  {
    { 2, (ৱর্ণ ধ্রুৱ []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (ৱর্ণ ধ্রুৱ []) {'m'} },	/* rc: Right of color sequence */
    { 0, রিক্ত },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (ৱর্ণ ধ্রুৱ []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, রিক্ত },			/* no: Normal */
    { 0, রিক্ত },			/* fi: File: default */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (ৱর্ণ ধ্রুৱ []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, রিক্ত },			/* mi: Missing file: undefined */
    { 0, রিক্ত },			/* or: Orphaned symlink: undefined */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((ৱর্ণ ধ্রুৱ [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, রিক্ত },			/* ca: disabled by default */
    { 0, রিক্ত },			/* mh: disabled by default */
    { 3, ((ৱর্ণ ধ্রুৱ [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
স্থির সংরচনা color_ext_type *color_ext_list = রিক্ত;

/* Buffer for color sequences */
স্থির ৱর্ণ *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

স্থির bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

স্থির bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

স্থির গণনা Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

স্থির bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

স্থির bool immediate_dirs;

/* True means that directories are grouped before files. */

স্থির bool directories_first;

/* Which files to ignore.  */

স্থির গণনা
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

সংরচনা ignore_pattern
  {
    ৱর্ণ ধ্রুৱ *pattern;
    সংরচনা ignore_pattern *next;
  };

স্থির সংরচনা ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
স্থির সংরচনা ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
স্থির bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
স্থির bool qmark_newline;

/* Quoting options for file and dir name output.  */

স্থির সংরচনা quoting_options *filename_quoting_options;
স্থির সংরচনা quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
স্থির size_t tabsize;

/* True means print each directory name before listing it.  */

স্থির bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

স্থির size_t সাৰি_length;

/* The local time zone rules, as per the TZ environment variable.  */

স্থির timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

স্থির bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

স্থির bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

স্থির bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

গণনা { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

স্থির ৱর্ণ ধ্রুৱ *long_time_format[2] =
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

স্থির sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

স্থির sig_atomic_t চঞ্চল interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

স্থির sig_atomic_t চঞ্চল stop_signal_গুনতি;

/* Desired exit status.  */

স্থির পূর্ণাঙ্ক exit_status;

/* Exit statuses.  */
গণনা
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
গণনা
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  BLOCK_SIZE_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_সাৰি_SYMLINK_TO_DIR_OPTION,
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

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"all", no_argument, রিক্ত, 'a'},
  {"escape", no_argument, রিক্ত, 'b'},
  {"directory", no_argument, রিক্ত, 'd'},
  {"dired", no_argument, রিক্ত, 'D'},
  {"full-time", no_argument, রিক্ত, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, রিক্ত,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, রিক্ত, 'h'},
  {"inode", no_argument, রিক্ত, 'i'},
  {"kibibytes", no_argument, রিক্ত, 'k'},
  {"numeric-uid-gid", no_argument, রিক্ত, 'n'},
  {"no-group", no_argument, রিক্ত, 'G'},
  {"hide-control-chars", no_argument, রিক্ত, 'q'},
  {"reverse", no_argument, রিক্ত, 'r'},
  {"size", no_argument, রিক্ত, 's'},
  {"width", required_argument, রিক্ত, 'w'},
  {"almost-all", no_argument, রিক্ত, 'A'},
  {"ignore-backups", no_argument, রিক্ত, 'B'},
  {"classify", optional_argument, রিক্ত, 'F'},
  {"file-type", no_argument, রিক্ত, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, রিক্ত, SI_OPTION},
  {"dereference-command-line", no_argument, রিক্ত, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, রিক্ত,
   DEREFERENCE_COMMAND_সাৰি_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, রিক্ত, HIDE_OPTION},
  {"ignore", required_argument, রিক্ত, 'I'},
  {"indicator-style", required_argument, রিক্ত, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, রিক্ত, 'L'},
  {"literal", no_argument, রিক্ত, 'N'},
  {"quote-name", no_argument, রিক্ত, 'Q'},
  {"quoting-style", required_argument, রিক্ত, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, রিক্ত, 'R'},
  {"format", required_argument, রিক্ত, FORMAT_OPTION},
  {"show-control-chars", no_argument, রিক্ত, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, রিক্ত, SORT_OPTION},
  {"tabsize", required_argument, রিক্ত, 'T'},
  {"time", required_argument, রিক্ত, TIME_OPTION},
  {"time-style", required_argument, রিক্ত, TIME_STYLE_OPTION},
  {"zero", no_argument, রিক্ত, ZERO_OPTION},
  {"color", optional_argument, রিক্ত, COLOR_OPTION},
  {"hyperlink", optional_argument, রিক্ত, HYPERLINK_OPTION},
  {"block-size", required_argument, রিক্ত, BLOCK_SIZE_OPTION},
  {"context", no_argument, রিক্ত, 'Z'},
  {"author", no_argument, রিক্ত, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", রিক্ত
};
স্থির গণনা format ধ্রুৱ format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_সাৰি, one_per_সাৰি
};
ARGMATCH_VERIFY (format_args, format_types);

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", রিক্ত
};
স্থির গণনা sort_type ধ্রুৱ sort_types[] =
{
  sort_none, sort_size, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  রিক্ত
};
স্থির গণনা time_type ধ্রুৱ time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

স্থির ৱর্ণ ধ্রুৱ *ধ্রুৱ when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", রিক্ত
};
স্থির গণনা when_type ধ্রুৱ when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
সংরচনা column_info
{
  bool valid_len;
  size_t সাৰি_len;
  size_t *col_arr;
};

/* Array with information about column fullness.  */
স্থির সংরচনা column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
স্থির size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
গণনা { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
স্থির off_t dired_pos;

স্থির শূন্য
dired_outbyte (ৱর্ণ c)
{
  dired_pos++;
  putchar (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
স্থির শূন্য
dired_outbuf (ৱর্ণ ধ্রুৱ *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, পরিমাণ *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
স্থির শূন্য
dired_outstring (ৱর্ণ ধ্রুৱ *s)
{
  dired_outbuf (s, strlen (s));
}

স্থির শূন্য
dired_indent (শূন্য)
{
  যদি (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
স্থির সংরচনা obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
স্থির সংরচনা obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
স্থির শূন্য
ঠেলো_current_dired_pos (সংরচনা obstack *obs)
{
  যদি (dired)
    obstack_grow (obs, &dired_pos, পরিমাণ dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
স্থির সংরচনা obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
স্থির শূন্য
dev_ino_ঠেলো (dev_t dev, ino_t ino)
{
  শূন্য *vdi;
  সংরচনা dev_ino *di;
  পূর্ণাঙ্ক dev_ino_size = পরিমাণ *di;
  obstack_blank (&dev_ino_obstack, dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
স্থির সংরচনা dev_ino
dev_ino_বেৰ_কৰো (শূন্য)
{
  শূন্য *vdi;
  সংরচনা dev_ino *di;
  পূর্ণাঙ্ক dev_ino_size = পরিমাণ *di;
  affirm (dev_ino_size <= obstack_object_size (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_size);
  vdi = obstack_next_free (&dev_ino_obstack);
  di = vdi;
  ঘূৰাই *di;
}

স্থির শূন্য
assert_matching_dev_ino (ৱর্ণ ধ্রুৱ *name, সংরচনা dev_ino di)
{
  MAYBE_UNUSED সংরচনা stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

স্থির ৱর্ণ eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

স্থির শূন্য
dired_dump_obstack (ৱর্ণ ধ্রুৱ *prefix, সংরচনা obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_size (os) / পরিমাণ (dired_pos);
  যদি (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      fputs (prefix, stdout);
      বাবে (size_t i = 0; i < n_pos; i++)
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
স্থির সংরচনা timespec
get_stat_btime (সংরচনা stat ধ্রুৱ *st)
{
  সংরচনা timespec btimespec;

#যদি HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#অন্যথা
  btimespec = get_stat_birthtime (st);
#endif

  ঘূৰাই btimespec;
}

#যদি HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
স্থির unsigned পূর্ণাঙ্ক
time_type_to_statx (শূন্য)
{
  ৱিকল্প (time_type)
    {
    পক্ষ time_ctime:
      ঘূৰাই STATX_CTIME;
    পক্ষ time_mtime:
      ঘূৰাই STATX_MTIME;
    পক্ষ time_atime:
      ঘূৰাই STATX_ATIME;
    পক্ষ time_btime:
      ঘূৰাই STATX_BTIME;
    পক্ষ time_numtypes: স্ৱভাৱ:
      unreachable ();
    }
    ঘূৰাই 0;
}

ATTRIBUTE_PURE
স্থির unsigned পূর্ণাঙ্ক
calc_req_mask (শূন্য)
{
  unsigned পূর্ণাঙ্ক mask = STATX_MODE;

  যদি (print_inode)
    mask |= STATX_INO;

  যদি (print_block_size)
    mask |= STATX_BLOCKS;

  যদি (format == long_format) {
    mask |= STATX_NLINK | STATX_SIZE | time_type_to_statx ();
    যদি (print_owner || print_author)
      mask |= STATX_UID;
    যদি (print_group)
      mask |= STATX_GID;
  }

  ৱিকল্প (sort_type)
    {
    পক্ষ sort_none:
    পক্ষ sort_name:
    পক্ষ sort_version:
    পক্ষ sort_extension:
    পক্ষ sort_width:
      ভঙ্গ;
    পক্ষ sort_time:
      mask |= time_type_to_statx ();
      ভঙ্গ;
    পক্ষ sort_size:
      mask |= STATX_SIZE;
      ভঙ্গ;
    পক্ষ sort_numtypes: স্ৱভাৱ:
      unreachable ();
    }

  ঘূৰাই mask;
}

স্থির পূর্ণাঙ্ক
do_statx (পূর্ণাঙ্ক fd, ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st, পূর্ণাঙ্ক flags,
          unsigned পূর্ণাঙ্ক mask)
{
  সংরচনা statx stx;
  bool want_btime = mask & STATX_BTIME;
  পূর্ণাঙ্ক ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  যদি (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      যদি (want_btime)
        {
          যদি (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          অন্যথা
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
do_stat (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

স্থির পূর্ণাঙ্ক
do_lstat (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

স্থির পূর্ণাঙ্ক
stat_for_mode (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
স্থির পূর্ণাঙ্ক
stat_for_ino (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

স্থির পূর্ণাঙ্ক
fstat_for_ino (পূর্ণাঙ্ক fd, সংরচনা stat *st)
{
  ঘূৰাই do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#অন্যথা
স্থির পূর্ণাঙ্ক
do_stat (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই stat (name, st);
}

স্থির পূর্ণাঙ্ক
do_lstat (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই lstat (name, st);
}

স্থির পূর্ণাঙ্ক
stat_for_mode (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই stat (name, st);
}

স্থির পূর্ণাঙ্ক
stat_for_ino (ৱর্ণ ধ্রুৱ *name, সংরচনা stat *st)
{
  ঘূৰাই stat (name, st);
}

স্থির পূর্ণাঙ্ক
fstat_for_ino (পূর্ণাঙ্ক fd, সংরচনা stat *st)
{
  ঘূৰাই fstat (fd, st);
}
#endif

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
স্থির ৱর্ণ ধ্রুৱ *
first_percent_b (ৱর্ণ ধ্রুৱ *fmt)
{
  বাবে (; *fmt; fmt++)
    যদি (fmt[0] == '%')
      ৱিকল্প (fmt[1])
        {
        পক্ষ 'b': ঘূৰাই fmt;
        পক্ষ '%': fmt++; ভঙ্গ;
        }
  ঘূৰাই রিক্ত;
}

স্থির ৱর্ণ RFC3986[256];
স্থির শূন্য
file_escape_init (শূন্য)
{
  বাবে (পূর্ণাঙ্ক i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

গণনা { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
গণনা { ABFORMAT_SIZE = 128 };
স্থির ৱর্ণ abformat[2][12][ABFORMAT_SIZE];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
স্থির bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

স্থির bool
abmon_init (ৱর্ণ abmon[12][ABFORMAT_SIZE])
{
#ifndef HAVE_NL_LANGINFO
  ঘূৰাই false;
#অন্যথা
  পূর্ণাঙ্ক max_mon_width = 0;
  পূর্ণাঙ্ক mon_width[12];
  পূর্ণাঙ্ক mon_len[12];

  বাবে (পূর্ণাঙ্ক i = 0; i < 12; i++)
    {
      ৱর্ণ ধ্রুৱ *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_SIZE);
      যদি (mon_len[i] == ABFORMAT_SIZE)
        ঘূৰাই false;
      যদি (strchr (abbr, '%'))
        ঘূৰাই false;
      mon_width[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      যদি (mon_width[i] < 0)
        ঘূৰাই false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  বাবে (পূর্ণাঙ্ক i = 0; i < 12; i++)
    {
      পূর্ণাঙ্ক fill = max_mon_width - mon_width[i];
      যদি (ABFORMAT_SIZE - mon_len[i] <= fill)
        ঘূৰাই false;
      bool align_বাম = !c_isdigit (abmon[i][0]);
      পূর্ণাঙ্ক fill_offset;
      যদি (align_বাম)
        fill_offset = mon_len[i];
      অন্যথা
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      memset (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  ঘূৰাই true;
#endif
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

স্থির শূন্য
abformat_init (শূন্য)
{
  ৱর্ণ ধ্রুৱ *pb[2];
  বাবে (পূর্ণাঙ্ক recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  যদি (! (pb[0] || pb[1]))
    ঘূৰাই;

  ৱর্ণ abmon[12][ABFORMAT_SIZE];
  যদি (! abmon_init (abmon))
    ঘূৰাই;

  বাবে (পূর্ণাঙ্ক recent = 0; recent < 2; recent++)
    {
      ৱর্ণ ধ্রুৱ *fmt = long_time_format[recent];
      বাবে (পূর্ণাঙ্ক i = 0; i < 12; i++)
        {
          ৱর্ণ *nfmt = abformat[recent][i];
          পূর্ণাঙ্ক nbytes;

          যদি (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%s", fmt);
          অন্যথা
            {
              যদি (! (pb[recent] - fmt <= MIN (ABFORMAT_SIZE, INT_MAX)))
                ঘূৰাই;
              পূর্ণাঙ্ক prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_SIZE, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          যদি (! (0 <= nbytes && nbytes < ABFORMAT_SIZE))
            ঘূৰাই;
        }
    }

  use_abformat = true;
}

স্থির size_t
dev_ino_hash (শূন্য ধ্রুৱ *x, size_t table_size)
{
  সংরচনা dev_ino ধ্রুৱ *p = x;
  ঘূৰাই (uintmax_t) {p->st_ino} % table_size;
}

স্থির bool
dev_ino_compare (শূন্য ধ্রুৱ *x, শূন্য ধ্রুৱ *y)
{
  সংরচনা dev_ino ধ্রুৱ *a = x;
  সংরচনা dev_ino ধ্রুৱ *b = y;
  ঘূৰাই PSAME_INODE (a, b);
}

স্থির শূন্য
dev_ino_free (শূন্য *x)
{
  মুক্ত (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

স্থির bool
visit_dir (dev_t dev, ino_t ino)
{
  সংরচনা dev_ino *ent;
  সংরচনা dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (পরিমাণ *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_ঢোকাও (active_dir_set, ent);

  যদি (ent_from_table == রিক্ত)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  যদি (found_match)
    {
      /* ent was not inserted, so free it.  */
      মুক্ত (ent);
    }

  ঘূৰাই found_match;
}

স্থির শূন্য
free_pending_ent (সংরচনা pending *p)
{
  মুক্ত (p->name);
  মুক্ত (p->realname);
  মুক্ত (p);
}

স্থির bool
is_colored (গণনা indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  যদি (len == 0)
    ঘূৰাই false;
  যদি (2 < len)
    ঘূৰাই true;
  ৱর্ণ ধ্রুৱ *s = color_indicator[type].string;
  ঘূৰাই (s[0] != '0') | (s[len - 1] != '0');
}

স্থির শূন্য
restore_default_color (শূন্য)
{
  put_indicator (&color_indicator[C_বাম]);
  put_indicator (&color_indicator[C_ডান]);
}

স্থির শূন্য
set_normal_color (শূন্য)
{
  যদি (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_বাম]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_ডান]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

স্থির শূন্য
sighandler (পূর্ণাঙ্ক sig)
{
  যদি (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

স্থির শূন্য
stophandler (MAYBE_UNUSED পূর্ণাঙ্ক sig)
{
  যদি (! interrupt_signal)
    stop_signal_গুনতি++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

স্থির শূন্য
process_signals (শূন্য)
{
  যেতিয়ালৈ (interrupt_signal || stop_signal_গুনতি)
    {
      পূর্ণাঙ্ক sig;
      পূর্ণাঙ্ক stops;
      sigset_t oldset;

      যদি (used_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_গুনতি;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      যদি (stops)
        {
          stop_signal_গুনতি = stops - 1;
          sig = SIGSTOP;
        }
      অন্যথা
        সঙ্কেত (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      উত্থাপয (sig);
      sigprocmask (SIG_SETMASK, &oldset, রিক্ত);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

স্থির শূন্য
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  স্থির পূর্ণাঙ্ক ধ্রুৱ stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  গণনা { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  যদি (init)
    {
      সংরচনা sigaction act;

      sigemptyset (&caught_signals);
      বাবে (পূর্ণাঙ্ক j = 0; j < nsigs + nstop; j++)
        {
          পূর্ণাঙ্ক sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, রিক্ত, &act);
          যদি (act.sa_handler != SIG_IGN)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      বাবে (পূর্ণাঙ্ক j = 0; j < nsigs + nstop; j++)
        {
          পূর্ণাঙ্ক sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          যদি (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, রিক্ত);
            }
        }
    }
  অন্যথা /* restore.  */
    {
      বাবে (পূর্ণাঙ্ক j = 0; j < nsigs + nstop; j++)
        {
          পূর্ণাঙ্ক sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          যদি (sigismember (&caught_signals, sig))
            সঙ্কেত (sig, SIG_DFL);
        }
    }
}

স্থির শূন্য
signal_init (শূন্য)
{
  signal_setup (true);
}

স্থির শূন্য
signal_restore (শূন্য)
{
  signal_setup (false);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক i;
  সংরচনা pending *thispend;
  পূর্ণাঙ্ক n_files;

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
  pending_dirs = রিক্ত;

  current_time.tv_sec = TYPE_MINIMUM (time_t);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  যদি (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  যদি (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  যদি (directories_first)
    check_symlink_mode = true;
  অন্যথা যদি (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      যদি (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  যদি (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_সাৰি_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  যদি (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_SIZE, রিক্ত,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      যদি (active_dir_set == রিক্ত)
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

  যদি (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  যদি (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      যদি (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * পরিমাণ *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  যদি (n_files <= 0)
    {
      যদি (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_সংখ্যা, true, রিক্ত);
      অন্যথা
        queue_directory (".", রিক্ত, true);
    }
  অন্যথা
    কৰা
      gobble_file (argv[i++], unknown, NOT_AN_INODE_সংখ্যা, true, রিক্ত);
    যেতিয়ালৈ (i < argc);

  যদি (cwd_n_used)
    {
      sort_files ();
      যদি (!immediate_dirs)
        extract_dirs_from_files (রিক্ত, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  যদি (cwd_n_used)
    {
      print_current_files ();
      যদি (pending_dirs)
        dired_outbyte ('\n');
    }
  অন্যথা যদি (n_files <= 1 && pending_dirs && !pending_dirs->next)
    print_dir_name = false;

  যেতিয়ালৈ (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->next;

      যদি (LOOP_DETECT)
        {
          যদি (thispend->name == রিক্ত)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              সংরচনা dev_ino di = dev_ino_বেৰ_কৰো ();
              সংরচনা dev_ino *found = hash_remove (active_dir_set, &di);
              যদি (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              অনুৱর্ত;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_সাৰি_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  যদি (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      যদি (!(color_indicator[C_বাম].len == 2
            && memeq (color_indicator[C_বাম].string, "\033[", 2)
            && color_indicator[C_ডান].len == 1
            && color_indicator[C_ডান].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      বাবে (পূর্ণাঙ্ক j = stop_signal_গুনতি; j; j--)
        উত্থাপয (SIGSTOP);
      পূর্ণাঙ্ক j = interrupt_signal;
      যদি (j)
        উত্থাপয (j);
    }

  যদি (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      printf ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  যদি (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  ঘূৰাই exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

স্থির ptrdiff_t
decode_সাৰি_length (ৱর্ণ ধ্রুৱ *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  ৱিকল্প (xstrtoumax (spec, রিক্ত, 0, &val, ""))
    {
    পক্ষ LONGINT_OK:
      ঘূৰাই val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    পক্ষ LONGINT_OVERFLOW:
      ঘূৰাই 0;

    পক্ষ LONGINT_INVALID:
    পক্ষ LONGINT_INVALID_SUFFIX_CHAR:
    পক্ষ LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      ঘূৰাই -1;

    স্ৱভাৱ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

স্থির bool
stdout_isatty (শূন্য)
{
  স্থির signed ৱর্ণ out_tty = -1;
  যদি (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  ঘূৰাই out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

স্থির পূর্ণাঙ্ক
decode_switches (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ *time_style_option = রিক্ত;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  পূর্ণাঙ্ক format_opt = -1;
  পূর্ণাঙ্ক hide_control_chars_opt = -1;
  পূর্ণাঙ্ক quoting_style_opt = -1;
  পূর্ণাঙ্ক sort_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t width_opt = -1;

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক oi = -1;
      পূর্ণাঙ্ক c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      যদি (c == -1)
        ভঙ্গ;

      ৱিকল্প (c)
        {
        পক্ষ 'a':
          ignore_mode = IGNORE_MINIMAL;
          ভঙ্গ;

        পক্ষ 'b':
          quoting_style_opt = escape_quoting_style;
          ভঙ্গ;

        পক্ষ 'c':
          time_type = time_ctime;
          explicit_time = true;
          ভঙ্গ;

        পক্ষ 'd':
          immediate_dirs = true;
          ভঙ্গ;

        পক্ষ 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          ভঙ্গ;

        পক্ষ FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          ভঙ্গ;

        পক্ষ 'g':
          format_opt = long_format;
          print_owner = false;
          ভঙ্গ;

        পক্ষ 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_size = output_block_size = 1;
          ভঙ্গ;

        পক্ষ 'i':
          print_inode = true;
          ভঙ্গ;

        পক্ষ 'k':
          kibibytes_specified = true;
          ভঙ্গ;

        পক্ষ 'l':
          format_opt = long_format;
          ভঙ্গ;

        পক্ষ 'm':
          format_opt = with_commas;
          ভঙ্গ;

        পক্ষ 'n':
          numeric_ids = true;
          format_opt = long_format;
          ভঙ্গ;

        পক্ষ 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          ভঙ্গ;

        পক্ষ 'p':
          indicator_style = slash;
          ভঙ্গ;

        পক্ষ 'q':
          hide_control_chars_opt = true;
          ভঙ্গ;

        পক্ষ 'r':
          sort_reverse = true;
          ভঙ্গ;

        পক্ষ 's':
          print_block_size = true;
          ভঙ্গ;

        পক্ষ 't':
          sort_opt = sort_time;
          ভঙ্গ;

        পক্ষ 'u':
          time_type = time_atime;
          explicit_time = true;
          ভঙ্গ;

        পক্ষ 'v':
          sort_opt = sort_version;
          ভঙ্গ;

        পক্ষ 'w':
          width_opt = decode_সাৰি_length (optarg);
          যদি (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          ভঙ্গ;

        পক্ষ 'x':
          format_opt = horizontal;
          ভঙ্গ;

        পক্ষ 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          ভঙ্গ;

        পক্ষ 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          ভঙ্গ;

        পক্ষ 'C':
          format_opt = many_per_সাৰি;
          ভঙ্গ;

        পক্ষ 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          ভঙ্গ;

        পক্ষ 'F':
          {
            পূর্ণাঙ্ক i;
            যদি (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            অন্যথা
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            যদি (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            ভঙ্গ;
          }

        পক্ষ 'G':		/* inhibit display of group info */
          print_group = false;
          ভঙ্গ;

        পক্ষ 'H':
          dereference = DEREF_COMMAND_সাৰি_ARGUMENTS;
          ভঙ্গ;

        পক্ষ DEREFERENCE_COMMAND_সাৰি_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_সাৰি_SYMLINK_TO_DIR;
          ভঙ্গ;

        পক্ষ 'I':
          add_ignore_pattern (optarg);
          ভঙ্গ;

        পক্ষ 'L':
          dereference = DEREF_ALWAYS;
          ভঙ্গ;

        পক্ষ 'N':
          quoting_style_opt = literal_quoting_style;
          ভঙ্গ;

        পক্ষ 'Q':
          quoting_style_opt = c_quoting_style;
          ভঙ্গ;

        পক্ষ 'R':
          recursive = true;
          ভঙ্গ;

        পক্ষ 'S':
          sort_opt = sort_size;
          ভঙ্গ;

        পক্ষ 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          ভঙ্গ;

        পক্ষ 'U':
          sort_opt = sort_none;
          ভঙ্গ;

        পক্ষ 'X':
          sort_opt = sort_extension;
          ভঙ্গ;

        পক্ষ '1':
          /* -1 has no effect after -l.  */
          যদি (format_opt != long_format)
            format_opt = one_per_সাৰি;
          ভঙ্গ;

        পক্ষ AUTHOR_OPTION:
          print_author = true;
          ভঙ্গ;

        পক্ষ HIDE_OPTION:
          {
            সংরচনা ignore_pattern *hide = xmalloc (পরিমাণ *hide);
            hide->pattern = optarg;
            hide->next = hide_patterns;
            hide_patterns = hide;
          }
          ভঙ্গ;

        পক্ষ SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          ভঙ্গ;

        পক্ষ GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          ভঙ্গ;

        পক্ষ TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          ভঙ্গ;

        পক্ষ FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          ভঙ্গ;

        পক্ষ FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (ৱর্ণ *) "full-iso";
          ভঙ্গ;

        পক্ষ COLOR_OPTION:
          {
            পূর্ণাঙ্ক i;
            যদি (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            অন্যথা
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            ভঙ্গ;
          }

        পক্ষ HYPERLINK_OPTION:
          {
            পূর্ণাঙ্ক i;
            যদি (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            অন্যথা
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            ভঙ্গ;
          }

        পক্ষ INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          ভঙ্গ;

        পক্ষ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ভঙ্গ;

        পক্ষ TIME_STYLE_OPTION:
          time_style_option = optarg;
          ভঙ্গ;

        পক্ষ SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          ভঙ্গ;

        পক্ষ BLOCK_SIZE_OPTION:
          {
            গণনা strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            যদি (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_size = output_block_size;
          }
          ভঙ্গ;

        পক্ষ SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_size = output_block_size = 1;
          ভঙ্গ;

        পক্ষ 'Z':
          print_scontext = true;
          ভঙ্গ;

        পক্ষ ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          যদি (format_opt != long_format)
            format_opt = one_per_সাৰি;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (LS_FAILURE);
        }
    }

  যদি (! output_block_size)
    {
      ৱর্ণ ধ্রুৱ *ls_block_size = getenv ("LS_BLOCK_SIZE");
      human_options (ls_block_size,
                     &human_output_opts, &output_block_size);
      যদি (ls_block_size || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_size = output_block_size;
        }
      যদি (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_size = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_সাৰি : one_per_সাৰি)
            : ls_mode == LS_MULTI_COL ? many_per_সাৰি
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = width_opt;
  যদি (format == many_per_সাৰি || format == horizontal || format == with_commas
      || print_with_color)
    {
#ifdef TIOCGWINSZ
      যদি (linelen < 0)
        {
          সংরচনা winsize ws;
          যদি (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#endif
      যদি (linelen < 0)
        {
          ৱর্ণ ধ্রুৱ *p = getenv ("COLUMNS");
          যদি (p && *p)
            {
              linelen = decode_সাৰি_length (p);
              যদি (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  সাৰি_length = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = সাৰি_length / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += সাৰি_length % MIN_COLUMN_WIDTH != 0;

  যদি (format == many_per_সাৰি || format == horizontal || format == with_commas)
    {
      যদি (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      অন্যথা
        {
          tabsize = 8;
          ৱর্ণ ধ্রুৱ *p = getenv ("TABSIZE");
          যদি (p)
            {
              uintmax_t tmp;
              যদি (xstrtoumax (p, রিক্ত, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              অন্যথা
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
                  && ((সাৰি_length
                       && (format == with_commas
                           || format == many_per_সাৰি || format == horizontal))
                      || format == one_per_সাৰি || format == long_format);

  পূর্ণাঙ্ক qs = quoting_style_opt;
  যদি (qs < 0)
    qs = getenv_quoting_style ();
  যদি (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  যদি (0 <= qs)
    set_quoting_style (রিক্ত, qs);
  qs = get_quoting_style (রিক্ত);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_সাৰি || format == horizontal) && সাৰি_length))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (রিক্ত);
  যদি (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  যদি (file_type <= indicator_style)
    {
      বাবে (ৱর্ণ ধ্রুৱ *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  যদি (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (রিক্ত);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  যদি (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  যদি (format == long_format)
    {
      ৱর্ণ *envstyle = রিক্ত;
      ৱর্ণ *style = time_style_option;
      যদি (! style)
        style = envstyle = getenv ("TIME_STYLE");

      যদি (! style)
        style = (ৱর্ণ *) "locale";
      অন্যথা
        {
          স্থির ৱর্ণ ধ্রুৱ posix_prefix[] = "posix-";
          যেতিয়ালৈ (STREQ_LEN (style, posix_prefix, পরিমাণ posix_prefix - 1))
            {
              যদি (! hard_locale (LC_TIME))
                ঘূৰাই optind;
              style += পরিমাণ posix_prefix - 1;
            }
        }

      যদি (*style == '+')
        {
          ৱর্ণ *p0 = style + 1;
          ৱর্ণ *p0nl = strchr (p0, '\n');
          ৱর্ণ ধ্রুৱ *p1 = p0;
          যদি (p0nl)
            {
              p1 = p0nl + 1;
              যদি (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              যদি (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              অন্যথা
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      অন্যথা
        {
          ৱিকল্প (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (ৱর্ণ ধ্রুৱ *) time_style_types,
                                     পরিমাণ (*time_style_types), LS_FAILURE))
            {
            পক্ষ full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              ভঙ্গ;

            পক্ষ long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              ভঙ্গ;

            পক্ষ iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              ভঙ্গ;

            পক্ষ locale_time_style:
              যদি (hard_locale (LC_TIME))
                {
                  বাবে (পূর্ণাঙ্ক i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (রিক্ত, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  ঘূৰাই optind;
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

স্থির bool
get_funky_string (ৱর্ণ **dest, ৱর্ণ ধ্রুৱ **src, bool equals_end,
                  size_t *output_গুনতি)
{
  ৱর্ণ অঙ্ক;			/* For numerical codes */
  size_t গুনতি;			/* Something to count with */
  গণনা {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  ৱর্ণ ধ্রুৱ *p;
  ৱর্ণ *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  গুনতি = 0;			/* No characters counted in yet.  */
  অঙ্ক = 0;

  state = ST_GND;		/* Start in ground state.  */
  যেতিয়ালৈ (state < ST_END)
    {
      ৱিকল্প (state)
        {
        পক্ষ ST_GND:		/* Ground state (no escapes) */
          ৱিকল্প (*p)
            {
            পক্ষ ':':
            পক্ষ '\0':
              state = ST_END;	/* End of string */
              ভঙ্গ;
            পক্ষ '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              ভঙ্গ;
            পক্ষ '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              ভঙ্গ;
            পক্ষ '=':
              যদি (equals_end)
                {
                  state = ST_END; /* End */
                  ভঙ্গ;
                }
              FALLTHROUGH;
            স্ৱভাৱ:
              *(q++) = *(p++);
              ++গুনতি;
              ভঙ্গ;
            }
          ভঙ্গ;

        পক্ষ ST_BACKSLASH:	/* Backslash escaped character */
          ৱিকল্প (*p)
            {
            পক্ষ '0':
            পক্ষ '1':
            পক্ষ '2':
            পক্ষ '3':
            পক্ষ '4':
            পক্ষ '5':
            পক্ষ '6':
            পক্ষ '7':
              state = ST_OCTAL;	/* Octal sequence */
              অঙ্ক = *p - '0';
              ভঙ্গ;
            পক্ষ 'x':
            পক্ষ 'X':
              state = ST_HEX;	/* Hex sequence */
              অঙ্ক = 0;
              ভঙ্গ;
            পক্ষ 'a':		/* Bell */
              অঙ্ক = '\a';
              ভঙ্গ;
            পক্ষ 'b':		/* Backspace */
              অঙ্ক = '\b';
              ভঙ্গ;
            পক্ষ 'e':		/* Escape */
              অঙ্ক = 27;
              ভঙ্গ;
            পক্ষ 'f':		/* Form feed */
              অঙ্ক = '\f';
              ভঙ্গ;
            পক্ষ 'n':		/* Newline */
              অঙ্ক = '\n';
              ভঙ্গ;
            পক্ষ 'r':		/* Carriage return */
              অঙ্ক = '\r';
              ভঙ্গ;
            পক্ষ 't':		/* Tab */
              অঙ্ক = '\t';
              ভঙ্গ;
            পক্ষ 'v':		/* Vtab */
              অঙ্ক = '\v';
              ভঙ্গ;
            পক্ষ '?':		/* Delete */
              অঙ্ক = 127;
              ভঙ্গ;
            পক্ষ '_':		/* Space */
              অঙ্ক = ' ';
              ভঙ্গ;
            পক্ষ '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              ভঙ্গ;
            স্ৱভাৱ:		/* Escaped character like \ ^ : = */
              অঙ্ক = *p;
              ভঙ্গ;
            }
          যদি (state == ST_BACKSLASH)
            {
              *(q++) = অঙ্ক;
              ++গুনতি;
              state = ST_GND;
            }
          ++p;
          ভঙ্গ;

        পক্ষ ST_OCTAL:		/* Octal sequence */
          যদি (*p < '0' || *p > '7')
            {
              *(q++) = অঙ্ক;
              ++গুনতি;
              state = ST_GND;
            }
          অন্যথা
            অঙ্ক = (অঙ্ক << 3) + (*(p++) - '0');
          ভঙ্গ;

        পক্ষ ST_HEX:		/* Hex sequence */
          ৱিকল্প (*p)
            {
            পক্ষ '0':
            পক্ষ '1':
            পক্ষ '2':
            পক্ষ '3':
            পক্ষ '4':
            পক্ষ '5':
            পক্ষ '6':
            পক্ষ '7':
            পক্ষ '8':
            পক্ষ '9':
              অঙ্ক = (অঙ্ক << 4) + (*(p++) - '0');
              ভঙ্গ;
            পক্ষ 'a':
            পক্ষ 'b':
            পক্ষ 'c':
            পক্ষ 'd':
            পক্ষ 'e':
            পক্ষ 'f':
              অঙ্ক = (অঙ্ক << 4) + (*(p++) - 'a') + 10;
              ভঙ্গ;
            পক্ষ 'A':
            পক্ষ 'B':
            পক্ষ 'C':
            পক্ষ 'D':
            পক্ষ 'E':
            পক্ষ 'F':
              অঙ্ক = (অঙ্ক << 4) + (*(p++) - 'A') + 10;
              ভঙ্গ;
            স্ৱভাৱ:
              *(q++) = অঙ্ক;
              ++গুনতি;
              state = ST_GND;
              ভঙ্গ;
            }
          ভঙ্গ;

        পক্ষ ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          যদি (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++গুনতি;
            }
          অন্যথা যদি (*p == '?')
            {
              *(q++) = 127;
              ++গুনতি;
            }
          অন্যথা
            state = ST_ERROR;
          ভঙ্গ;

        পক্ষ ST_END: পক্ষ ST_ERROR: স্ৱভাৱ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_গুনতি = গুনতি;

  ঘূৰাই state != ST_ERROR;
}

গণনা parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

স্থির bool
known_term_type (শূন্য)
{
  ৱর্ণ ধ্রুৱ *term = getenv ("TERM");
  যদি (! term || ! *term)
    ঘূৰাই false;

  ৱর্ণ ধ্রুৱ *সাৰি = G_সাৰি;
  যেতিয়ালৈ (সাৰি - G_সাৰি < পরিমাণ (G_সাৰি))
    {
      যদি (STRNCMP_LIT (সাৰি, "TERM ") == 0)
        {
          যদি (fnmatch (সাৰি + 5, term, 0) == 0)
            ঘূৰাই true;
        }
      সাৰি += strlen (সাৰি) + 1;
    }

  ঘূৰাই false;
}

স্থির শূন্য
parse_ls_color (শূন্য)
{
  ৱর্ণ ধ্রুৱ *p;		/* Pointer to character being parsed */
  ৱর্ণ *buf;			/* color_buf buffer pointer */
  ৱর্ণ label0, label1;		/* Indicator label */
  সংরচনা color_ext_type *ext;	/* Extension we are working on */

  যদি ((p = getenv ("LS_COLORS")) == রিক্ত || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      ৱর্ণ ধ্রুৱ *colorterm = getenv ("COLORTERM");
      যদি (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      ঘূৰাই;
    }

  ext = রিক্ত;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  গণনা parse_state state = PS_START;
  যেতিয়ালৈ (true)
    {
      ৱিকল্প (state)
        {
        পক্ষ PS_START:		/* First label character */
          ৱিকল্প (*p)
            {
            পক্ষ ':':
              ++p;
              ভঙ্গ;

            পক্ষ '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (পরিমাণ *ext);
              ext->next = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              ভঙ্গ;

            পক্ষ '\0':
              state = PS_DONE;	/* Done! */
              গচ্ছ done;

            স্ৱভাৱ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              ভঙ্গ;
            }
          ভঙ্গ;

        পক্ষ PS_2:		/* Second label character */
          যদি (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          অন্যথা
            state = PS_FAIL;	/* Error */
          ভঙ্গ;

        পক্ষ PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          যদি (*(p++) == '=')/* It *should* be...  */
            {
              বাবে (পূর্ণাঙ্ক i = 0; i < countof (indicator_name); i++)
                {
                  যদি ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      ভঙ্গ;
                    }
                }
              যদি (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((ৱর্ণ []) {label0, label1, '\0'}));
            }
          ভঙ্গ;

        পক্ষ PS_4:		/* Equal sign after *.ext */
          যদি (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          অন্যথা
            state = PS_FAIL;
          ভঙ্গ;

        পক্ষ PS_FAIL:
          গচ্ছ done;

        পক্ষ PS_DONE: স্ৱভাৱ:
          affirm (false);
        }
    }
 done:

  যদি (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      মুক্ত (color_buf);
      বাবে (সংরচনা color_ext_type *e = color_ext_list; e != রিক্ত;
           /* empty */)
        {
          সংরচনা color_ext_type *e2 = e;
          e = e->next;
          মুক্ত (e2);
        }
      print_with_color = false;
      ঘূৰাই;
    }
  অন্যথা
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      বাবে (সংরচনা color_ext_type *e1 = color_ext_list; e1 != রিক্ত;
           e1 = e1->next)
        {
          bool case_ignored = false;

          বাবে (সংরচনা color_ext_type *e2 = e1->next; e2 != রিক্ত;
               e2 = e2->next)
            {
              যদি (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  যদি (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  অন্যথা যদি (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      যদি (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      অন্যথা যদি (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      অন্যথা
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  যদি (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

স্থির শূন্য
set_exit_status (bool serious)
{
  যদি (serious)
    exit_status = LS_FAILURE;
  অন্যথা যদি (exit_status == EXIT_SUCCESS)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

স্থির শূন্য
file_failure (bool serious, ৱর্ণ ধ্রুৱ *message, ৱর্ণ ধ্রুৱ *file)
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

স্থির শূন্য
queue_directory (ৱর্ণ ধ্রুৱ *name, ৱর্ণ ধ্রুৱ *realname, bool command_সাৰি_arg)
{
  সংরচনা pending *new = xmalloc (পরিমাণ *new);
  new->realname = realname ? xstrdup (realname) : রিক্ত;
  new->name = name ? xstrdup (name) : রিক্ত;
  new->command_সাৰি_arg = command_সাৰি_arg;
  new->next = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

স্থির শূন্য
print_dir (ৱর্ণ ধ্রুৱ *name, ৱর্ণ ধ্রুৱ *realname, bool command_সাৰি_arg)
{
  DIR *dirp;
  সংরচনা dirent *next;
  uintmax_t total_blocks = 0;
  স্থির bool first = true;

  errno = 0;
  dirp = opendir (name);
  যদি (!dirp)
    {
      যদি (command_সাৰি_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_সাৰি_arg, _("cannot open directory %s"), name);
      ঘূৰাই;
    }

  যদি (LOOP_DETECT)
    {
      সংরচনা stat dir_stat;
      পূর্ণাঙ্ক fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      যদি ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          যদি (! ignorable_traversal_errno (errno))
            file_failure (command_সাৰি_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          ঘূৰাই;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      যদি (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          set_exit_status (true);
          ঘূৰাই;
        }

      dev_ino_ঠেলো (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  যদি (recursive || print_dir_name)
    {
      যদি (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      ৱর্ণ *absolute_name = রিক্ত;
      যদি (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          যদি (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_সাৰি_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  রিক্ত, true, &subdired_obstack, absolute_name);

      মুক্ত (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  যেতিয়ালৈ (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      next = readdir (dirp);
      যদি (next)
        {
          যদি (! file_ignored (next->d_name))
            {
              গণনা filetype type;
#যদি HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[next->d_type];
#অন্যথা
              type = unknown;
#endif
              total_blocks += gobble_file (next->d_name, type,
                                           RELIABLE_D_INO (next),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              যদি (format == one_per_সাৰি && sort_type == sort_none
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
      অন্যথা
        {
          পূর্ণাঙ্ক err = errno;
          যদি (err == 0)
            ভঙ্গ;
          /* Ignore errors indicating that the directory was removed.  */
          যদি (ignorable_traversal_errno (err))
            ভঙ্গ;
          file_failure (command_সাৰি_arg, _("reading directory %s"), name);
          যদি (err != EOVERFLOW)
            ভঙ্গ;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  যদি (closedir (dirp) != 0)
    {
      file_failure (command_সাৰি_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  যদি (recursive)
    extract_dirs_from_files (name, false);

  যদি (format == long_format || print_block_size)
    {
      ৱর্ণ buf[LONGEST_HUMAN_READABLE + 3];
      ৱর্ণ *p = human_readable (total_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_size);
      ৱর্ণ *pend = p + strlen (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  যদি (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

স্থির শূন্য
add_ignore_pattern (ৱর্ণ ধ্রুৱ *pattern)
{
  সংরচনা ignore_pattern *ignore;

  ignore = xmalloc (পরিমাণ *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->next = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

স্থির bool
patterns_match (সংরচনা ignore_pattern ধ্রুৱ *patterns, ৱর্ণ ধ্রুৱ *file)
{
  বাবে (সংরচনা ignore_pattern ধ্রুৱ *p = patterns; p; p = p->next)
    যদি (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      ঘূৰাই true;
  ঘূৰাই false;
}

/* Return true if FILE should be ignored.  */

স্থির bool
file_ignored (ৱর্ণ ধ্রুৱ *name)
{
  ঘূৰাই ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

স্থির uintmax_t
unsigned_file_size (off_t size)
{
  ঘূৰাই size + (size < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#ifdef HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
স্থির bool
has_capability (ৱর্ণ ধ্রুৱ *name)
{
  ৱর্ণ *result;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  যদি (cap_d == রিক্ত)
    ঘূৰাই false;

  result = cap_to_text (cap_d, রিক্ত);
  cap_free (cap_d);
  যদি (!result)
    ঘূৰাই false;

  /* check if human-readable capability string is empty */
  has_cap = !!*result;

  cap_free (result);
  ঘূৰাই has_cap;
}
#অন্যথা
স্থির bool
has_capability (MAYBE_UNUSED ৱর্ণ ধ্রুৱ *name)
{
  errno = ENOTSUP;
  ঘূৰাই false;
}
#endif

/* Enter and remove entries in the table 'cwd_file'.  */

স্থির শূন্য
free_ent (সংরচনা fileinfo *f)
{
  মুক্ত (f->name);
  মুক্ত (f->linkname);
  মুক্ত (f->absolute_name);
  যদি (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
স্থির শূন্য
clear_files (শূন্য)
{
  বাবে (idx_t i = 0; i < cwd_n_used; i++)
    {
      সংরচনা fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_সংখ্যা_width = 0;
  block_size_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_সংখ্যা_width = 0;
  minor_device_সংখ্যা_width = 0;
  file_size_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
স্থির পূর্ণাঙ্ক
file_has_aclinfo_cache (ৱর্ণ ধ্রুৱ *file, সংরচনা fileinfo *f,
                        সংরচনা aclinfo *ai, পূর্ণাঙ্ক flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  স্থির পূর্ণাঙ্ক unsupported_return;
  স্থির ৱর্ণ *unsupported_scontext;
  স্থির পূর্ণাঙ্ক unsupported_scontext_err;
  স্থির dev_t unsupported_device;

  যদি (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->size = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      ঘূৰাই unsupported_return;
    }

  errno = 0;
  পূর্ণাঙ্ক n = file_has_aclinfo (file, ai, flags);
  পূর্ণাঙ্ক err = errno;
  যদি (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  যদি (f->stat_ok && n <= 0 && !acl_errno_valid (err)
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
  ঘূৰাই n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
স্থির bool
has_capability_cache (ৱর্ণ ধ্রুৱ *file, সংরচনা fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  স্থির bool unsupported_cached;
  স্থির dev_t unsupported_device;

  যদি (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      ঘূৰাই 0;
    }

  bool b = has_capability (file);
  যদি (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  ঘূৰাই b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
স্থির শূন্য
get_extended_info (ৱর্ণ ধ্রুৱ *full_name, সংরচনা fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  গণনা filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#যদি HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#endif
  bool check_capability = format_needs_capability & (type == normal);

  যদি (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      সংরচনা aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      পূর্ণাঙ্ক aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      পূর্ণাঙ্ক n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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

      যদি (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (full_name));
      অন্যথা
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          যদি (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      যদি (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = রিক্ত;
      aclinfo_free (&ai);
    }
}

স্থির bool
needs_quoting (ৱর্ণ ধ্রুৱ *name)
{
  ৱর্ণ test[2];
  size_t len = quotearg_buffer (test, পরিমাণ test , name, -1,
                                filename_quoting_options);
  ঘূৰাই *name != *test || strlen (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
স্থির uintmax_t
gobble_file (ৱর্ণ ধ্রুৱ *name, গণনা filetype type, ino_t inode,
             bool command_সাৰি_arg, ৱর্ণ ধ্রুৱ *dirname)
{
  uintmax_t blocks = 0;
  সংরচনা fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_সাৰি_arg || inode == NOT_AN_INODE_সংখ্যা);

  যদি (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, পরিমাণ *cwd_file);

  f = &cwd_file[cwd_n_used];
  memset (f, '\0', পরিমাণ *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  যদি ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      যদি (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_সাৰি_arg
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
      || (print_inode && inode == NOT_AN_INODE_সংখ্যা)
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
  ৱর্ণ ধ্রুৱ *full_name = name;
  যদি (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      ৱর্ণ *p = alloca (strlen (name) + strlen (dirname) + 2);
      attach (p, dirname, name);
      full_name = p;
    }

  bool do_deref;

  যদি (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  অন্যথা
    {
      পূর্ণাঙ্ক err;

      যদি (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          যদি (! f->absolute_name
              && (command_সাৰি_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_সাৰি_arg,
                          _("error canonicalizing %s"), full_name);
        }

      ৱিকল্প (dereference)
        {
        পক্ষ DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          ভঙ্গ;

        পক্ষ DEREF_COMMAND_সাৰি_ARGUMENTS:
        পক্ষ DEREF_COMMAND_সাৰি_SYMLINK_TO_DIR:
          যদি (command_সাৰি_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              যদি (dereference == DEREF_COMMAND_সাৰি_ARGUMENTS)
                ভঙ্গ;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              যদি (!need_lstat)
                ভঙ্গ;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        পক্ষ DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          ভঙ্গ;

        পক্ষ DEREF_UNDEFINED: স্ৱভাৱ:
          affirm (false);
        }

      যদি (err != 0)
        {
          যদি (! command_সাৰি_arg && ignorable_traversal_errno (errno))
            {
              পূর্ণাঙ্ক stat_errno = errno;
              সংরচনা stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              errno = stat_errno;
              যদি (! dangling_link)
                {
                  free_ent (f);
                  ঘূৰাই 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_সাৰি_arg,
                        _("cannot access %s"), full_name);

          যদি (command_সাৰি_arg)
            {
              free_ent (f);
              ঘূৰাই 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          ঘূৰাই 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  যদি (type == directory && command_সাৰি_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  যদি ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      সংরচনা stat linkstats;

      যদি (! get_link_name (full_name, f, command_সাৰি_arg))
        {
          free_ent (f);
          ঘূৰাই 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      যদি (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      যদি (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  যদি (format == long_format || print_block_size)
    {
      ৱর্ণ buf[LONGEST_HUMAN_READABLE + 1];
      পূর্ণাঙ্ক len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_size),
                          MBSWIDTH_FLAGS);
      যদি (block_size_width < len)
        block_size_width = len;
    }

  যদি (format == long_format)
    {
      যদি (print_owner)
        {
          পূর্ণাঙ্ক len = format_user_width (f->stat.st_uid);
          যদি (owner_width < len)
            owner_width = len;
        }

      যদি (print_group)
        {
          পূর্ণাঙ্ক len = format_group_width (f->stat.st_gid);
          যদি (group_width < len)
            group_width = len;
        }

      যদি (print_author)
        {
          পূর্ণাঙ্ক len = format_user_width (f->stat.st_author);
          যদি (author_width < len)
            author_width = len;
        }
    }

  যদি (print_scontext)
    {
      পূর্ণাঙ্ক len = strlen (f->scontext);
      যদি (scontext_width < len)
        scontext_width = len;
    }

  যদি (format == long_format)
    {
      ৱর্ণ b[INT_BUFSIZE_BOUND (uintmax_t)];
      পূর্ণাঙ্ক b_len = strlen (umaxtostr (f->stat.st_nlink, b));
      যদি (nlink_width < b_len)
        nlink_width = b_len;

      যদি ((type == chardev) | (type == blockdev))
        {
          ৱর্ণ buf[INT_BUFSIZE_BOUND (uintmax_t)];
          পূর্ণাঙ্ক len = strlen (umaxtostr (major (f->stat.st_rdev), buf));
          যদি (major_device_সংখ্যা_width < len)
            major_device_সংখ্যা_width = len;
          len = strlen (umaxtostr (minor (f->stat.st_rdev), buf));
          যদি (minor_device_সংখ্যা_width < len)
            minor_device_সংখ্যা_width = len;
          len = major_device_সংখ্যা_width + 2 + minor_device_সংখ্যা_width;
          যদি (file_size_width < len)
            file_size_width = len;
        }
      অন্যথা
        {
          ৱর্ণ buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t size = unsigned_file_size (f->stat.st_size);
          পূর্ণাঙ্ক len = mbswidth (human_readable (size, buf,
                                              file_human_output_opts,
                                              1, file_output_block_size),
                              MBSWIDTH_FLAGS);
          যদি (file_size_width < len)
            file_size_width = len;
        }
    }

  যদি (print_inode)
    {
      ৱর্ণ buf[INT_BUFSIZE_BOUND (uintmax_t)];
      পূর্ণাঙ্ক len = strlen (umaxtostr (f->stat.st_ino, buf));
      যদি (inode_সংখ্যা_width < len)
        inode_সংখ্যা_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  ঘূৰাই blocks;
}

/* Return true if F refers to a directory.  */
স্থির bool
is_directory (ধ্রুৱ সংরচনা fileinfo *f)
{
  ঘূৰাই f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
স্থির bool
is_linked_directory (ধ্রুৱ সংরচনা fileinfo *f)
{
  ঘূৰাই f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

স্থির bool
get_link_name (ৱর্ণ ধ্রুৱ *filename, সংরচনা fileinfo *f, bool command_সাৰি_arg)
{
  f->linkname = areadlink_with_size (filename, f->stat.st_size);
  যদি (f->linkname == রিক্ত)
    {
      যদি (! command_সাৰি_arg
          && ignorable_traversal_errno (errno))
        ঘূৰাই false;
      file_failure (command_সাৰি_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  ঘূৰাই true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

স্থির bool
basename_is_dot_or_dotdot (ৱর্ণ ধ্রুৱ *name)
{
  ৱর্ণ ধ্রুৱ *base = last_component (name);
  ঘূৰাই dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

স্থির শূন্য
extract_dirs_from_files (ৱর্ণ ধ্রুৱ *dirname, bool command_সাৰি_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != রিক্ত);

  যদি (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (রিক্ত, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  বাবে (i = cwd_n_used; 0 < i; )
    {
      i--;
      সংরচনা fileinfo *f = sorted_file[i];

      যদি (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          যদি (!dirname || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_সাৰি_arg);
          অন্যথা
            {
              ৱর্ণ *name = file_name_concat (dirname, f->name, রিক্ত);
              queue_directory (name, f->linkname, command_সাৰি_arg);
              মুক্ত (name);
            }
          যদি (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  বাবে (i = 0, j = 0; i < cwd_n_used; i++)
    {
      সংরচনা fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

স্থির jmp_buf failed_strcoll;

স্থির পূর্ণাঙ্ক
xstrcoll (ৱর্ণ ধ্রুৱ *a, ৱর্ণ ধ্রুৱ *b)
{
  পূর্ণাঙ্ক diff;
  errno = 0;
  diff = strcoll (a, b);
  যদি (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      longjmp (failed_strcoll, 1);
    }
  ঘূৰাই diff;
}

/* Comparison routines for sorting the files.  */

প্রকারনির্দেশ শূন্য ধ্রুৱ *V;
প্রকারনির্দেশ পূর্ণাঙ্ক (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
স্থির পূর্ণাঙ্ক
dirfirst_check (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
                পূর্ণাঙ্ক (*cmp) (V, V))
{
  পূর্ণাঙ্ক diff = is_linked_directory (b) - is_linked_directory (a);
  ঘূৰাই diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#define DEFINE_SORT_FUNCTIONS(চাবি_name, চাবি_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  স্থির পূর্ণাঙ্ক xstrcoll_##key_name (V a, V b)				\
  { ঘূৰাই চাবি_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE স্থির পূর্ণাঙ্ক strcmp_##key_name (V a, V b)		\
  { ঘূৰাই চাবি_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  স্থির পূর্ণাঙ্ক rev_xstrcoll_##key_name (V a, V b)				\
  { ঘূৰাই চাবি_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE স্থির পূর্ণাঙ্ক rev_strcmp_##key_name (V a, V b)	\
  { ঘূৰাই চাবি_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  স্থির পূর্ণাঙ্ক xstrcoll_df_##key_name (V a, V b)				\
  { ঘূৰাই dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE স্থির পূর্ণাঙ্ক strcmp_df_##key_name (V a, V b)		\
  { ঘূৰাই dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  স্থির পূর্ণাঙ্ক rev_xstrcoll_df_##key_name (V a, V b)			\
  { ঘূৰাই dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE স্থির পূর্ণাঙ্ক rev_strcmp_df_##key_name (V a, V b)	\
  { ঘূৰাই dirfirst_check (a, b, rev_strcmp_##key_name); }

স্থির পূর্ণাঙ্ক
cmp_ctime (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
           পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
cmp_mtime (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
           পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
cmp_atime (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
           পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
cmp_btime (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
           পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
off_cmp (off_t a, off_t b)
{
  ঘূৰাই _GL_CMP (a, b);
}

স্থির পূর্ণাঙ্ক
cmp_size (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
          পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = off_cmp (b->stat.st_size, a->stat.st_size);
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
cmp_name (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
          পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  ঘূৰাই cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

স্থির পূর্ণাঙ্ক
cmp_extension (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
               পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  ৱর্ণ ধ্রুৱ *base1 = strrchr (a->name, '.');
  ৱর্ণ ধ্রুৱ *base2 = strrchr (b->name, '.');
  পূর্ণাঙ্ক diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

স্থির size_t
fileinfo_name_width (সংরচনা fileinfo ধ্রুৱ *f)
{
  ঘূৰাই f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

স্থির পূর্ণাঙ্ক
cmp_width (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b,
          পূর্ণাঙ্ক (*cmp) (ৱর্ণ ধ্রুৱ *, ৱর্ণ ধ্রুৱ *))
{
  পূর্ণাঙ্ক diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  ঘূৰাই diff ? diff : cmp (a->name, b->name);
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
স্থির পূর্ণাঙ্ক
cmp_version (সংরচনা fileinfo ধ্রুৱ *a, সংরচনা fileinfo ধ্রুৱ *b)
{
  পূর্ণাঙ্ক diff = filevercmp (a->name, b->name);
  ঘূৰাই diff ? diff : strcmp (a->name, b->name);
}

স্থির পূর্ণাঙ্ক
xstrcoll_version (V a, V b)
{
  ঘূৰাই cmp_version (a, b);
}
স্থির পূর্ণাঙ্ক
rev_xstrcoll_version (V a, V b)
{
  ঘূৰাই cmp_version (b, a);
}
স্থির পূর্ণাঙ্ক
xstrcoll_df_version (V a, V b)
{
  ঘূৰাই dirfirst_check (a, b, xstrcoll_version);
}
স্থির পূর্ণাঙ্ক
rev_xstrcoll_df_version (V a, V b)
{
  ঘূৰাই dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#define LIST_SORTFUNCTION_VARIANTS(চাবি_name)                        \
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

স্থির qsortFunc ধ্রুৱ sort_functions[][2][2][2] =
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
        { রিক্ত, রিক্ত },
        { রিক্ত, রিক্ত },
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

স্থির শূন্য
initialize_ordering_vector (শূন্য)
{
  বাবে (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

স্থির শূন্য
update_current_files_info (শূন্য)
{
  /* Cache screen width of name, if needed multiple times.  */
  যদি (sort_type == sort_width
      || (সাৰি_length && (format == many_per_সাৰি || format == horizontal)))
    {
      বাবে (idx_t i = 0; i < cwd_n_used; i++)
        {
          সংরচনা fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

স্থির শূন্য
sort_files (শূন্য)
{
  bool use_strcmp;

  যদি (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      মুক্ত (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * পরিমাণ *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  যদি (sort_type == sort_none)
    ঘূৰাই;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  যদি (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  অন্যথা
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((শূন্য ধ্রুৱ **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

স্থির শূন্য
print_current_files (শূন্য)
{
  ৱিকল্প (format)
    {
    পক্ষ one_per_সাৰি:
      বাবে (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          putchar (eolbyte);
        }
      ভঙ্গ;

    পক্ষ many_per_সাৰি:
      যদি (! সাৰি_length)
        print_with_separator (' ');
      অন্যথা
        print_many_per_সাৰি ();
      ভঙ্গ;

    পক্ষ horizontal:
      যদি (! সাৰি_length)
        print_with_separator (' ');
      অন্যথা
        print_horizontal ();
      ভঙ্গ;

    পক্ষ with_commas:
      print_with_separator (',');
      ভঙ্গ;

    পক্ষ long_format:
      বাবে (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      ভঙ্গ;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

স্থির ptrdiff_t
align_nstrftime (ৱর্ণ *buf, size_t size, bool recent, সংরচনা tm ধ্রুৱ *tm,
                 timezone_t tz, পূর্ণাঙ্ক ns)
{
  ৱর্ণ ধ্রুৱ *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_time_format[recent]);
  ঘূৰাই nstrftime (buf, size, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

স্থির পূর্ণাঙ্ক
long_time_expected_width (শূন্য)
{
  স্থির পূর্ণাঙ্ক width = -1;

  যদি (width < 0)
    {
      time_t epoch = 0;
      সংরচনা tm tm;
      ৱর্ণ buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      যদি (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, পরিমাণ buf, false,
                                           &tm, localtz, 0);
          যদি (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      যদি (width < 0)
        width = 0;
    }

  ঘূৰাই width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

স্থির শূন্য
format_user_or_group (ৱর্ণ ধ্রুৱ *name, uintmax_t id, পূর্ণাঙ্ক width)
{
  যদি (name)
    {
      পূর্ণাঙ্ক name_width = mbswidth (name, MBSWIDTH_FLAGS);
      পূর্ণাঙ্ক width_gap = name_width < 0 ? 0 : width - name_width;
      পূর্ণাঙ্ক pad = MAX (0, width_gap);
      dired_outstring (name);

      কৰা
        dired_outbyte (' ');
      যেতিয়ালৈ (pad--);
    }
  অন্যথা
    dired_pos += printf ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

স্থির শূন্য
format_user (uid_t u, পূর্ণাঙ্ক width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? রিক্ত : getuser (u)), u, width);
}

/* Likewise, for groups.  */

স্থির শূন্য
format_group (gid_t g, পূর্ণাঙ্ক width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? রিক্ত : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

স্থির পূর্ণাঙ্ক
format_user_or_group_width (ৱর্ণ ধ্রুৱ *name, uintmax_t id)
{
  ঘূৰাই (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (রিক্ত, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

স্থির পূর্ণাঙ্ক
format_user_width (uid_t u)
{
  ঘূৰাই format_user_or_group_width (numeric_ids ? রিক্ত : getuser (u), u);
}

/* Likewise, for groups.  */

স্থির পূর্ণাঙ্ক
format_group_width (gid_t g)
{
  ঘূৰাই format_user_or_group_width (numeric_ids ? রিক্ত : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
স্থির ৱর্ণ *
format_inode (ৱর্ণ buf[INT_BUFSIZE_BOUND (uintmax_t)],
              ধ্রুৱ সংরচনা fileinfo *f)
{
  ঘূৰাই (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_সংখ্যা
          ? umaxtostr (f->stat.st_ino, buf)
          : (ৱর্ণ *) "?");
}

/* Print information about F in long format.  */
স্থির শূন্য
print_long_format (ধ্রুৱ সংরচনা fileinfo *f)
{
  ৱর্ণ modebuf[12];
  ৱর্ণ buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + পরিমাণ (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  ৱর্ণ *p;
  সংরচনা timespec when_timespec;
  সংরচনা tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  যদি (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  অন্যথা
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  যদি (! any_has_acl)
    modebuf[10] = '\0';
  অন্যথা যদি (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  অন্যথা যদি (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  অন্যথা যদি (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  ৱিকল্প (time_type)
    {
    পক্ষ time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      ভঙ্গ;
    পক্ষ time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      ভঙ্গ;
    পক্ষ time_atime:
      when_timespec = get_stat_atime (&f->stat);
      ভঙ্গ;
    পক্ষ time_btime:
      when_timespec = get_stat_btime (&f->stat);
      যদি (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      ভঙ্গ;
    পক্ষ time_numtypes: স্ৱভাৱ:
      unreachable ();
    }

  p = buf;

  যদি (print_inode)
    {
      ৱর্ণ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_সংখ্যা_width, format_inode (hbuf, f));
    }

  যদি (print_block_size)
    {
      ৱর্ণ hbuf[LONGEST_HUMAN_READABLE + 1];
      ৱর্ণ ধ্রুৱ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      পূর্ণাঙ্ক blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      বাবে (পূর্ণাঙ্ক pad = blocks_width < 0 ? 0 : block_size_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      যেতিয়ালৈ ((*p++ = *blocks++))
        অনুৱর্ত;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    ৱর্ণ hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  যদি (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      যদি (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      যদি (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      যদি (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      যদি (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  যদি (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      পূর্ণাঙ্ক blanks_width = (file_size_width
                          - (major_device_সংখ্যা_width + 2
                             + minor_device_সংখ্যা_width));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_সংখ্যা_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_সংখ্যা_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  অন্যথা
    {
      ৱর্ণ hbuf[LONGEST_HUMAN_READABLE + 1];
      ৱর্ণ ধ্রুৱ *size =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_size (f->stat.st_size),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_size));
      পূর্ণাঙ্ক size_width = mbswidth (size, MBSWIDTH_FLAGS);
      বাবে (পূর্ণাঙ্ক pad = size_width < 0 ? 0 : file_size_width - size_width;
           0 < pad; pad--)
        *p++ = ' ';
      যেতিয়ালৈ ((*p++ = *size++))
        অনুৱর্ত;
      p[-1] = ' ';
    }

  s = -1;

  যদি (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      সংরচনা timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      যদি (timespec_cmp (current_time, when_timespec) < 0)
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

  যদি (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  অন্যথা
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      ৱর্ণ hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  যদি (f->filetype == symbolic_link)
    {
      যদি (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, রিক্ত, (p - buf) + w + 4);
          যদি (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  অন্যথা যদি (indicator_style != none)
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

স্থির size_t
quote_name_buf (ৱর্ণ **inbuf, size_t bufsize, ৱর্ণ *name,
                সংরচনা quoting_options ধ্রুৱ *options,
                পূর্ণাঙ্ক needs_general_quoting, size_t *width, bool *pad)
{
  ৱর্ণ *buf = *inbuf;
  size_t displayed_width IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  গণনা quoting_style qs = get_quoting_style (options);
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

  যদি (needs_general_quoting != 0)
    {
      len = quotearg_buffer (buf, bufsize, name, -1, options);
      যদি (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_buffer (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || strlen (name) != len;
    }
  অন্যথা যদি (needs_further_quoting || extra_newline_protection)
    {
      len = strlen (name);
      যদি (bufsize <= len)
        buf = xmalloc (len + 1);
      memcpy (buf, name, len + 1);

      quoted = false;
    }
  অন্যথা
    {
      len = strlen (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  যদি (extra_newline_protection)
    বাবে (ৱর্ণ *p = buf; p < buf + len; p++)
      যদি (eolbyte && *p == '\n')
        *p = '?';

  যদি (needs_further_quoting)
    {
      যদি (MB_CUR_MAX > 1)
        {
          ৱর্ণ ধ্রুৱ *p = buf;
          ৱর্ণ ধ্রুৱ *plimit = buf + len;
          ৱর্ণ *q = buf;
          displayed_width = 0;

          যেতিয়ালৈ (p < plimit)
            ৱিকল্প (*p)
              {
                পক্ষ ' ': পক্ষ '!': পক্ষ '"': পক্ষ '#': পক্ষ '%':
                পক্ষ '&': পক্ষ '\'': পক্ষ '(': পক্ষ ')': পক্ষ '*':
                পক্ষ '+': পক্ষ ',': পক্ষ '-': পক্ষ '.': পক্ষ '/':
                পক্ষ '0': পক্ষ '1': পক্ষ '2': পক্ষ '3': পক্ষ '4':
                পক্ষ '5': পক্ষ '6': পক্ষ '7': পক্ষ '8': পক্ষ '9':
                পক্ষ ':': পক্ষ ';': পক্ষ '<': পক্ষ '=': পক্ষ '>':
                পক্ষ '?':
                পক্ষ 'A': পক্ষ 'B': পক্ষ 'C': পক্ষ 'D': পক্ষ 'E':
                পক্ষ 'F': পক্ষ 'G': পক্ষ 'H': পক্ষ 'I': পক্ষ 'J':
                পক্ষ 'K': পক্ষ 'L': পক্ষ 'M': পক্ষ 'N': পক্ষ 'O':
                পক্ষ 'P': পক্ষ 'Q': পক্ষ 'R': পক্ষ 'S': পক্ষ 'T':
                পক্ষ 'U': পক্ষ 'V': পক্ষ 'W': পক্ষ 'X': পক্ষ 'Y':
                পক্ষ 'Z':
                পক্ষ '[': পক্ষ '\\': পক্ষ ']': পক্ষ '^': পক্ষ '_':
                পক্ষ 'a': পক্ষ 'b': পক্ষ 'c': পক্ষ 'd': পক্ষ 'e':
                পক্ষ 'f': পক্ষ 'g': পক্ষ 'h': পক্ষ 'i': পক্ষ 'j':
                পক্ষ 'k': পক্ষ 'l': পক্ষ 'm': পক্ষ 'n': পক্ষ 'o':
                পক্ষ 'p': পক্ষ 'q': পক্ষ 'r': পক্ষ 's': পক্ষ 't':
                পক্ষ 'u': পক্ষ 'v': পক্ষ 'w': পক্ষ 'x': পক্ষ 'y':
                পক্ষ 'z': পক্ষ '{': পক্ষ '|': পক্ষ '}': পক্ষ '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  ভঙ্গ;
                স্ৱভাৱ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    কৰা
                      {
                        char32_t wc;
                        size_t bytes;
                        পূর্ণাঙ্ক w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        যদি (bytes == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            ভঙ্গ;
                          }

                        যদি (bytes == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            ভঙ্গ;
                          }

                        যদি (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        যদি (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            বাবে (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        অন্যথা
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    যেতিয়ালৈ (! mbsinit (&mbstate));
                  }
                  ভঙ্গ;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      অন্যথা
        {
          ৱর্ণ *p = buf;
          ৱর্ণ ধ্রুৱ *plimit = buf + len;

          যেতিয়ালৈ (p < plimit)
            {
              যদি (! isprint (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  অন্যথা যদি (width != রিক্ত)
    {
      যদি (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      অন্যথা
        {
          ৱর্ণ ধ্রুৱ *p = buf;
          ৱর্ণ ধ্রুৱ *plimit = buf + len;

          displayed_width = 0;
          যেতিয়ালৈ (p < plimit)
            {
              যদি (isprint (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  যদি (width != রিক্ত)
    *width = displayed_width;

  *inbuf = buf;

  ঘূৰাই len;
}

স্থির size_t
quote_name_width (ৱর্ণ ধ্রুৱ *name, সংরচনা quoting_options ধ্রুৱ *options,
                  পূর্ণাঙ্ক needs_general_quoting)
{
  ৱর্ণ smallbuf[BUFSIZ];
  ৱর্ণ *buf = smallbuf;
  size_t width;
  bool pad;

  quote_name_buf (&buf, পরিমাণ smallbuf, (ৱর্ণ *) name, options,
                  needs_general_quoting, &width, &pad);

  যদি (buf != smallbuf && buf != name)
    মুক্ত (buf);

  width += pad;

  ঘূৰাই width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
স্থির ৱর্ণ *
file_escape (ৱর্ণ ধ্রুৱ *str, bool path)
{
  ৱর্ণ *esc = xnmalloc (3, strlen (str) + 1);
  ৱর্ণ *p = esc;
  যেতিয়ালৈ (*str)
    {
      যদি (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      অন্যথা যদি (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      অন্যথা
        p += sprintf (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  ঘূৰাই esc;
}

স্থির size_t
quote_name (ৱর্ণ ধ্রুৱ *name, সংরচনা quoting_options ধ্রুৱ *options,
            পূর্ণাঙ্ক needs_general_quoting, ধ্রুৱ সংরচনা bin_str *color,
            bool allow_pad, সংরচনা obstack *স্তূপ, ৱর্ণ ধ্রুৱ *absolute_name)
{
  ৱর্ণ smallbuf[BUFSIZ];
  ৱর্ণ *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, পরিমাণ smallbuf, (ৱর্ণ *) name, options,
                        needs_general_quoting, রিক্ত, &pad);

  যদি (pad && allow_pad)
    dired_outbyte (' ');

  যদি (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  যদি (absolute_name)
    {
      যদি (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          putchar (*buf);
        }
      ৱর্ণ *h = file_escape (hostname, /* path= */ false);
      ৱর্ণ *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      printf ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      মুক্ত (h);
      মুক্ত (n);
    }

  যদি (স্তূপ)
    ঠেলো_current_dired_pos (স্তূপ);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  যদি (স্তূপ)
    ঠেলো_current_dired_pos (স্তূপ);

  যদি (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      যদি (skip_quotes)
        putchar (*(buf + len - 1));
    }

  যদি (buf != smallbuf && buf != name)
    মুক্ত (buf);

  ঘূৰাই len + pad;
}

স্থির size_t
print_name_with_quoting (ধ্রুৱ সংরচনা fileinfo *f,
                         bool symlink_target,
                         সংরচনা obstack *স্তূপ,
                         size_t start_col)
{
  ৱর্ণ ধ্রুৱ *name = symlink_target ? f->linkname : f->name;

  ধ্রুৱ সংরচনা bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : রিক্ত;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, স্তূপ, f->absolute_name);

  process_signals ();
  যদি (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      যদি (সাৰি_length
          && (start_col / সাৰি_length != (start_col + len - 1) / সাৰি_length))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  ঘূৰাই len;
}

স্থির শূন্য
prep_non_filename_text (শূন্য)
{
  যদি (color_indicator[C_END].string != রিক্ত)
    put_indicator (&color_indicator[C_END]);
  অন্যথা
    {
      put_indicator (&color_indicator[C_বাম]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_ডান]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

স্থির size_t
print_file_name_and_frills (ধ্রুৱ সংরচনা fileinfo *f, size_t start_col)
{
  ৱর্ণ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  যদি (print_inode)
    printf ("%*s ", format == with_commas ? 0 : inode_সংখ্যা_width,
            format_inode (buf, f));

  যদি (print_block_size)
    {
      ৱর্ণ ধ্রুৱ *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_size));
      পূর্ণাঙ্ক blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      পূর্ণাঙ্ক pad = 0;
      যদি (0 <= blocks_width && block_size_width && format != with_commas)
        pad = block_size_width - blocks_width;
      printf ("%*s%s ", pad, "", blocks);
    }

  যদি (print_scontext)
    printf ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  size_t width = print_name_with_quoting (f, false, রিক্ত, start_col);

  যদি (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  ঘূৰাই width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
স্থির ৱর্ণ
get_type_indicator (bool stat_ok, mode_t mode, গণনা filetype type)
{
  ৱর্ণ c;

  যদি (stat_ok ? S_ISREG (mode) : type == normal)
    {
      যদি (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      অন্যথা
        c = 0;
    }
  অন্যথা
    {
      যদি (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      অন্যথা যদি (indicator_style == slash)
        c = 0;
      অন্যথা যদি (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      অন্যথা যদি (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      অন্যথা যদি (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      অন্যথা যদি (stat_ok && S_ISDOOR (mode))
        c = '>';
      অন্যথা
        c = 0;
    }
  ঘূৰাই c;
}

স্থির bool
print_type_indicator (bool stat_ok, mode_t mode, গণনা filetype type)
{
  ৱর্ণ c = get_type_indicator (stat_ok, mode, type);
  যদি (c)
    dired_outbyte (c);
  ঘূৰাই !!c;
}

/* Returns if color sequence was printed.  */
স্থির bool
print_color_indicator (ধ্রুৱ সংরচনা bin_str *ind)
{
  যদি (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      যদি (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_বাম]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_ডান]);
    }

  ঘূৰাই ind != রিক্ত;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
স্থির ধ্রুৱ সংরচনা bin_str *
get_color_indicator (ধ্রুৱ সংরচনা fileinfo *f, bool symlink_target)
{
  গণনা indicator_no type;
  সংরচনা color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  ৱর্ণ ধ্রুৱ *name;
  mode_t mode;
  পূর্ণাঙ্ক linkok;
  যদি (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  অন্যথা
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  যদি (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  অন্যথা যদি (!f->stat_ok)
    {
      স্থির গণনা indicator_no ধ্রুৱ filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  অন্যথা
    {
      যদি (S_ISREG (mode))
        {
          type = C_FILE;

          যদি ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          অন্যথা যদি ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          অন্যথা যদি (f->has_capability)
            type = C_CAP;
          অন্যথা যদি ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          অন্যথা যদি ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      অন্যথা যদি (S_ISDIR (mode))
        {
          type = C_DIR;

          যদি ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          অন্যথা যদি ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          অন্যথা যদি ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      অন্যথা যদি (S_ISLNK (mode))
        type = C_LINK;
      অন্যথা যদি (S_ISFIFO (mode))
        type = C_FIFO;
      অন্যথা যদি (S_ISSOCK (mode))
        type = C_SOCK;
      অন্যথা যদি (S_ISBLK (mode))
        type = C_BLK;
      অন্যথা যদি (S_ISCHR (mode))
        type = C_CHR;
      অন্যথা যদি (S_ISDOOR (mode))
        type = C_DOOR;
      অন্যথা
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = রিক্ত;
  যদি (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = strlen (name);
      name += len;		/* Pointer to final \0.  */
      বাবে (ext = color_ext_list; ext != রিক্ত; ext = ext->next)
        {
          যদি (ext->ext.len <= len)
            {
              যদি (ext->exact_match)
                {
                  যদি (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    ভঙ্গ;
                }
              অন্যথা
                {
                  যদি (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    ভঙ্গ;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  যদি (type == C_LINK && !linkok)
    {
      যদি (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  ধ্রুৱ সংরচনা bin_str *ধ্রুৱ s
    = ext ? &(ext->seq) : &color_indicator[type];

  ঘূৰাই s->string ? s : রিক্ত;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
স্থির শূন্য
put_indicator (ধ্রুৱ সংরচনা bin_str *ind)
{
  যদি (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      যদি (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

স্থির size_t
length_of_file_name_and_frills (ধ্রুৱ সংরচনা fileinfo *f)
{
  size_t len = 0;
  ৱর্ণ buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  যদি (print_inode)
    len += 1 + (format == with_commas
                ? strlen (umaxtostr (f->stat.st_ino, buf))
                : inode_সংখ্যা_width);

  যদি (print_block_size)
    len += 1 + (format == with_commas
                ? strlen (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_size))
                : block_size_width);

  যদি (print_scontext)
    len += 1 + (format == with_commas ? strlen (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  যদি (indicator_style != none)
    {
      ৱর্ণ c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  ঘূৰাই len;
}

স্থির শূন্য
print_many_per_সাৰি (শূন্য)
{
  idx_t cols = calculate_columns (true);
  সংরচনা column_info ধ্রুৱ *সাৰি_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  বাবে (idx_t row = 0; row < rows; row++)
    {
      size_t col = 0;
      idx_t filesno = row;
      size_t pos = 0;

      /* Print the next row.  */
      যেতিয়ালৈ (true)
        {
          সংরচনা fileinfo ধ্রুৱ *f = sorted_file[filesno];
          size_t name_length = length_of_file_name_and_frills (f);
          size_t max_name_length = সাৰি_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          যদি (cwd_n_used - rows <= filesno)
            ভঙ্গ;
          filesno += rows;

          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }
      putchar (eolbyte);
    }
}

স্থির শূন্য
print_horizontal (শূন্য)
{
  size_t pos = 0;
  idx_t cols = calculate_columns (false);
  সংরচনা column_info ধ্রুৱ *সাৰি_fmt = &column_info[cols - 1];
  সংরচনা fileinfo ধ্রুৱ *f = sorted_file[0];
  size_t name_length = length_of_file_name_and_frills (f);
  size_t max_name_length = সাৰি_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  বাবে (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      যদি (col == 0)
        {
          putchar (eolbyte);
          pos = 0;
        }
      অন্যথা
        {
          indent (pos + name_length, pos + max_name_length);
          pos += max_name_length;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_length = length_of_file_name_and_frills (f);
      max_name_length = সাৰি_fmt->col_arr[col];
    }
  putchar (eolbyte);
}

/* Output name + SEP + ' '.  */

স্থির শূন্য
print_with_separator (ৱর্ণ sep)
{
  size_t pos = 0;

  বাবে (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      সংরচনা fileinfo ধ্রুৱ *f = sorted_file[filesno];
      size_t len = সাৰি_length ? length_of_file_name_and_frills (f) : 0;

      যদি (filesno != 0)
        {
          ৱর্ণ separator;
          size_t next_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          যদি (! সাৰি_length
              || (! ckd_add (&next_pos, next_pos, pos)
                  && ! ckd_add (&next_pos, next_pos, len)
                  && next_pos <= সাৰি_length))
            {
              pos += 2;
              separator = ' ';
            }
          অন্যথা
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

স্থির শূন্য
indent (size_t from, size_t to)
{
  যেতিয়ালৈ (from < to)
    {
      যদি (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          putchar ('\t');
          from += tabsize - from % tabsize;
        }
      অন্যথা
        {
          putchar (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

স্থির শূন্য
attach (ৱর্ণ *dest, ৱর্ণ ধ্রুৱ *dirname, ৱর্ণ ধ্রুৱ *name)
{
  ৱর্ণ ধ্রুৱ *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  যদি (dirname[0] != '.' || dirname[1] != 0)
    {
      যেতিয়ালৈ (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      যদি (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  যেতিয়ালৈ (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

স্থির শূন্য
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  স্থির idx_t column_info_alloc;

  যদি (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             পরিমাণ *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      যদি (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, পরিমাণ *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      বাবে (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  বাবে (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].সাৰি_len = i * MIN_COLUMN_WIDTH + 1;
      বাবে (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

স্থির idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  বাবে (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      সংরচনা fileinfo ধ্রুৱ *f = sorted_file[filesno];
      size_t name_length = length_of_file_name_and_frills (f);

      বাবে (idx_t i = 0; i < max_cols; ++i)
        {
          যদি (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_length = name_length + (idx == i ? 0 : 2);

              যদি (column_info[i].col_arr[idx] < real_length)
                {
                  column_info[i].সাৰি_len += (real_length
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_length;
                  column_info[i].valid_len = (column_info[i].সাৰি_len
                                              <= সাৰি_length);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  বাবে (cols = max_cols; 1 < cols; --cols)
    {
      যদি (column_info[cols - 1].valid_len)
        ভঙ্গ;
    }

  ঘূৰাই cols;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
List information about the FILEs (the current directory by স্ৱভাৱ).\n\
Sort entries alphabetically যদি none of -cftuvSUX nor --sort is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         কৰা not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         কৰা not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes বাবে nongraphic characters\n\
"));
      oputs (_("\
      --block-size=SIZE\n\
         with -l, scale sizes by SIZE when printing them;\n\
         e.g., '--block-size=M'; see SIZE format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         কৰা not list implied entries ending with ~\n\
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
         generate output designed বাবে Emacs' dired mode\n\
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
         like -F, except কৰা not append '*'\n\
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
         like -l, but কৰা not list owner\n\
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
  -H, --dereference-command-সাৰি\n\
         follow symbolic links listed on the command সাৰি\n\
"));
      oputs (_("\
      --dereference-command-সাৰি-symlink-to-dir\n\
         follow each command সাৰি symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         কৰা not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (স্ৱভাৱ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index সংখ্যা of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         কৰা not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         স্ৱভাৱ to 1024-byte blocks বাবে file system usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a long listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information বাবে a symbolic link,\n\
         show information বাবে the file the link references\n\
         rather than বাবে the link itself\n\
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
         like -l, but কৰা not list group information\n\
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
         the স্ৱভাৱ, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in দ্ৱিগুণ quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD বাবে entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order যেতিয়ালৈ sorting\n\
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
         change স্ৱভাৱ 'name' sort to WORD:\n\
           none (-U), size (-S), time (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --time=WORD\n\
         select which timestamp used to display or sort;\n\
           access time (-u): atime, access, use;\n\
           metadata change time (-c): ctime, status;\n\
           modified time (স্ৱভাৱ): mtime, modification;\n\
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
         কৰা not sort directory entries\n\
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
         list entries by সাৰিগুলি instead of by columns\n\
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
         end each output সাৰি with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per সাৰি\n\
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
Also the TIME_STYLE environment variable sets the স্ৱভাৱ style to use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color to distinguish file types is disabled both by স্ৱভাৱ and\n\
with --color=never.  With --color=auto, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), stdout);
      fputs (_("\
\n\
Exit status:\n\
 0  যদি OK,\n\
 1  যদি minor problems (e.g., cannot access subdirectory),\n\
 2  যদি serious trouble (e.g., cannot access command-সাৰি argument).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
