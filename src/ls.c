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

#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ <termios.h>
#ఒకవేళ HAVE_STROPTS_H
# include <stropts.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/ioctl.h>

#నిర్వచితమైతే WINSIZE_లోపల_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#అయితే_ముగింపు

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <setjmp.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <signal.h>

#ఒకవేళ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#అయితే_ముగింపు

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#నిర్వచితం_కాకపోతే SA_RESTART
# define SA_RESTART 0
#అయితే_ముగింపు

#అన్తర్భావ <fnmatch.h>

/* Gnulib includes.  */
#అన్తర్భావ "acl.h"
#అన్తర్భావ "areadlink.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "c-strcase.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "dev-ino.h"
#అన్తర్భావ "filemode.h"
#అన్తర్భావ "filenamecat.h"
#అన్తర్భావ "filevercmp.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "idcache.h"
#అన్తర్భావ "mbswidth.h"
#అన్తర్భావ "mpsort.h"
#అన్తర్భావ "obstack.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "strftime.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xgethostname.h"
#అన్తర్భావ "xmemdup0.h"
#అన్తర్భావ "xstrtol-error.h"
#అన్తర్భావ "xstrtol.h"

#అన్తర్భావ "system.h"

#అన్తర్భావ "dircolors.h"
#అన్తర్భావ "ls.h"
#అన్తర్భావ "statx.h"
#అన్తర్భావ "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#నిర్వచితమైతే HAVE_CAP
# include <sys/capability.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#అయితే_ముగింపు

#నిర్వచించు కార్యక్రమం_NAME (ls_బహుళకం == LS_LS ? "ls" \
                      : (ls_బహుళకం == LS_MULTI_నిలువు \
                         ? "dir" : "vdir"))

#నిర్వచించు AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#నిర్వచించు obstack_chunk_alloc జ్ఞాపకం_ఇవ్వు
#నిర్వచించు obstack_chunk_free విడుదల

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#నిర్వచితం_కాకపోతే READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#అయితే_ముగింపు

#ఒకవేళ READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_సంఖ్య
#లేకపోతే
# define RELIABLE_D_INO(dp) D_INO (dp)
#అయితే_ముగింపు

#ఒకవేళ ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#అయితే_ముగింపు

గణన filetype
  {
    unknown,
    fifo,
    chardev,
    directory,
    blockdev,
    సాధారణ,
    symbolic_link,
    sock,
    whiteout,
    arg_directory
  };
గణన { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
స్థిర అక్షరం స్థిరరాశి filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
స్థిర గుర్తులేని అక్షరం స్థిరరాశి filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
స్థిర అక్షరం స్థిరరాశి d_type_filetype[UCHAR_MAX + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = సాధారణ,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

గణన acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

నిర్మాణం fileinfo
  {
    /* The file name.  */
    అక్షరం *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    అక్షరం *linkname;

    /* For terminal hyperlinks. */
    అక్షరం *absolute_name;

    నిర్మాణం stat stat;

    గణన filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    అక్షరం *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    గణన acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    పూర్ణాంకం quoted;

    /* Cached screen width (including quoting).  */
    size_t వెడల్పు;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

నిర్మాణం bin_str
  {
    size_t len;			/* Number of bytes */
    అక్షరం స్థిరరాశి *string;		/* Pointer to the same */
  };

#ఒకవేళ ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#అయితే_ముగింపు

స్థిర size_t quote_name (అక్షరం స్థిరరాశి *name,
                          నిర్మాణం quoting_options స్థిరరాశి *options,
                          పూర్ణాంకం needs_general_quoting,
                          స్థిరరాశి నిర్మాణం bin_str *color,
                          bool allow_pad, నిర్మాణం obstack *దొంతర,
                          అక్షరం స్థిరరాశి *absolute_name);
స్థిర size_t quote_name_buf (అక్షరం **inbuf, size_t bufsize, అక్షరం *name,
                              నిర్మాణం quoting_options స్థిరరాశి *options,
                              పూర్ణాంకం needs_general_quoting, size_t *వెడల్పు,
                              bool *pad);
స్థిర పూర్ణాంకం decode_switches (పూర్ణాంకం argc, అక్షరం **argv);
స్థిర bool file_ignored (అక్షరం స్థిరరాశి *name);
స్థిర uintmax_t gobble_file (అక్షరం స్థిరరాశి *name, గణన filetype type,
                              ino_t inode, bool command_పంక్తి_arg,
                              అక్షరం స్థిరరాశి *dirname);
స్థిర స్థిరరాశి నిర్మాణం bin_str * get_color_indicator (స్థిరరాశి నిర్మాణం fileinfo *f,
                                                   bool symlink_లక్ష్యం);
స్థిర bool print_color_indicator (స్థిరరాశి నిర్మాణం bin_str *ind);
స్థిర శూన్య put_indicator (స్థిరరాశి నిర్మాణం bin_str *ind);
స్థిర శూన్య కూడు_ignore_నమూనా (అక్షరం స్థిరరాశి *నమూనా_2);
స్థిర శూన్య attach (అక్షరం *dest, అక్షరం స్థిరరాశి *dirname, అక్షరం స్థిరరాశి *name);
స్థిర శూన్య clear_files (శూన్య);
స్థిర శూన్య వెలికితీయి_dirs_from_files (అక్షరం స్థిరరాశి *dirname,
                                     bool command_పంక్తి_arg);
స్థిర bool get_link_name (అక్షరం స్థిరరాశి *filename, నిర్మాణం fileinfo *f,
                           bool command_పంక్తి_arg);
స్థిర శూన్య indent (size_t from, size_t గమ్యం);
స్థిర idx_t calculate_columns (bool by_columns);
స్థిర శూన్య print_current_files (శూన్య);
స్థిర శూన్య print_dir (అక్షరం స్థిరరాశి *name, అక్షరం స్థిరరాశి *realname,
                       bool command_పంక్తి_arg);
స్థిర size_t print_file_name_and_frills (స్థిరరాశి నిర్మాణం fileinfo *f,
                                          size_t ప్రారంభం_నిలువు);
స్థిర శూన్య print_horizontal (శూన్య);
స్థిర పూర్ణాంకం format_user_వెడల్పు (uid_t u);
స్థిర పూర్ణాంకం format_సమూహం_వెడల్పు (gid_t g);
స్థిర శూన్య print_long_format (స్థిరరాశి నిర్మాణం fileinfo *f);
స్థిర శూన్య print_many_చొప్పున_పంక్తి (శూన్య);
స్థిర size_t print_name_with_quoting (స్థిరరాశి నిర్మాణం fileinfo *f,
                                       bool symlink_లక్ష్యం,
                                       నిర్మాణం obstack *దొంతర,
                                       size_t ప్రారంభం_నిలువు);
స్థిర శూన్య prep_non_filename_పాఠ్యం (శూన్య);
స్థిర bool print_type_indicator (bool stat_ok, mode_t బహుళకం_2,
                                  గణన filetype type);
స్థిర శూన్య print_with_separator (అక్షరం sep);
స్థిర శూన్య వరుస_directory (అక్షరం స్థిరరాశి *name, అక్షరం స్థిరరాశి *realname,
                             bool command_పంక్తి_arg);
స్థిర శూన్య క్రమీకరించు_files (శూన్య);
స్థిర శూన్య parse_ls_color (శూన్య);

స్థిర size_t quote_name_వెడల్పు (అక్షరం స్థిరరాశి *name,
                                నిర్మాణం quoting_options స్థిరరాశి *options,
                                పూర్ణాంకం needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
గణన { INITIAL_పట్టిక_కొలత = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
స్థిర Hash_పట్టిక *చురుకైన_dir_సమితి;

#నిర్వచించు LOOP_DETECT (!!చురుకైన_dir_సమితి)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
స్థిర నిర్మాణం fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
స్థిర idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
స్థిర idx_t cwd_n_వాడిన;

/* Whether files needs may need padding due to quoting.  */
స్థిర bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
స్థిర bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
స్థిర శూన్య **sorted_file;
స్థిర size_t sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

స్థిర bool color_symlink_as_referent;

స్థిర అక్షరం స్థిరరాశి *hostname;

/* Mode of appropriate file for coloring.  */
స్థిర mode_t
file_or_link_బహుళకం (నిర్మాణం fileinfo స్థిరరాశి *file)
{
  తిరిగి (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_బహుళకం);
}


/* Record of one pending directory waiting to be listed.  */

నిర్మాణం pending
  {
    అక్షరం *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    అక్షరం *realname;
    bool command_పంక్తి_arg;
    నిర్మాణం pending *తదుపరి;
  };

స్థిర నిర్మాణం pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

స్థిర నిర్మాణం timespec current_సమయం;

స్థిర bool print_scontext;
స్థిర అక్షరం UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

స్థిర bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

స్థిర పూర్ణాంకం inode_సంఖ్య_వెడల్పు;
స్థిర పూర్ణాంకం ఖండం_కొలత_వెడల్పు;
స్థిర పూర్ణాంకం nlink_వెడల్పు;
స్థిర పూర్ణాంకం scontext_వెడల్పు;
స్థిర పూర్ణాంకం owner_వెడల్పు;
స్థిర పూర్ణాంకం సమూహం_వెడల్పు;
స్థిర పూర్ణాంకం author_వెడల్పు;
స్థిర పూర్ణాంకం major_device_సంఖ్య_వెడల్పు;
స్థిర పూర్ణాంకం minor_device_సంఖ్య_వెడల్పు;
స్థిర పూర్ణాంకం file_కొలత_వెడల్పు;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

గణన format
  {
    long_format,		/* -l and other options that imply -l */
    one_చొప్పున_పంక్తి,		/* -1 */
    many_చొప్పున_పంక్తి,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

స్థిర గణన format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
గణన సమయం_style
  {
    నిండిన_iso_సమయం_style,	/* --time-style=full-iso */
    long_iso_సమయం_style,	/* --time-style=long-iso */
    iso_సమయం_style,		/* --time-style=iso */
    locale_సమయం_style		/* --time-style=locale */
  };

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_style_వాదనలు[] =
{
  "full-iso", "long-iso", "iso", "locale", ఖాళీ
};
స్థిర గణన సమయం_style స్థిరరాశి సమయం_style_types[] =
{
  నిండిన_iso_సమయం_style, long_iso_సమయం_style, iso_సమయం_style,
  locale_సమయం_style
};
ARGMATCH_VERIFY (సమయం_style_వాదనలు, సమయం_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

గణన సమయం_type
  {
    సమయం_mtime = 0,		/* default */
    సమయం_ctime,			/* -c */
    సమయం_atime,			/* -u */
    సమయం_btime,                 /* birth time */
    సమయం_numtypes		/* the number of elements of this enum */
  };

స్థిర గణన సమయం_type సమయం_type;
స్థిర bool explicit_సమయం;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

గణన క్రమీకరించు_type
  {
    క్రమీకరించు_name = 0,		/* default */
    క్రమీకరించు_extension,		/* -X */
    క్రమీకరించు_వెడల్పు,
    క్రమీకరించు_కొలత,			/* -S */
    క్రమీకరించు_సంస్కరణ,		/* -v */
    క్రమీకరించు_సమయం,			/* -t; must be second to last */
    క్రమీకరించు_none,			/* -U; must be last */
    క్రమీకరించు_numtypes		/* the number of elements of this enum */
  };

స్థిర గణన క్రమీకరించు_type క్రమీకరించు_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

స్థిర bool క్రమీకరించు_తిరగేయి;

/* True means to display owner information.  -g turns this off.  */

స్థిర bool print_owner = true;

/* True means to display author information.  */

స్థిర bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

స్థిర bool print_సమూహం = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

స్థిర bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

స్థిర bool print_ఖండం_కొలత;

/* Human-readable options for output, when printing block counts.  */
స్థిర పూర్ణాంకం human_output_opts;

/* The units to use when printing block counts.  */
స్థిర uintmax_t output_ఖండం_కొలత;

/* Likewise, but for file sizes.  */
స్థిర పూర్ణాంకం file_human_output_opts;
స్థిర uintmax_t file_output_ఖండం_కొలత = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
స్థిర bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

గణన indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

స్థిర గణన indicator_style indicator_style;

/* Names of indicator styles.  */
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి indicator_style_వాదనలు[] =
{
  "none", "slash", "file-type", "classify", ఖాళీ
};
స్థిర గణన indicator_style స్థిరరాశి indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_వాదనలు, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

స్థిర bool print_with_color;

స్థిర bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

స్థిర bool వాడిన_color = false;

గణన when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

గణన Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_పంక్తి_ARGUMENTS,	/* -H */
    DEREF_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

గణన indicator_no
  {
    C_ఎడమ, C_కుడి, C_చివర, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_గమ్యం_EOL
  };

స్థిర అక్షరం స్థిరరాశి indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

నిర్మాణం color_ext_type
  {
    నిర్మాణం bin_str ext;		/* The extension we're looking for */
    నిర్మాణం bin_str seq;		/* The sequence to output when we do */
    bool   exact_సరిపోలిక;		/* Whether to compare case insensitively */
    నిర్మాణం color_ext_type *తదుపరి;	/* Next in list */
  };

స్థిర నిర్మాణం bin_str color_indicator[] =
  {
    { 2, (అక్షరం స్థిరరాశి []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (అక్షరం స్థిరరాశి []) {'m'} },	/* rc: Right of color sequence */
    { 0, ఖాళీ },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (అక్షరం స్థిరరాశి []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, ఖాళీ },			/* no: Normal */
    { 0, ఖాళీ },			/* fi: File: default */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (అక్షరం స్థిరరాశి []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, ఖాళీ },			/* mi: Missing file: undefined */
    { 0, ఖాళీ },			/* or: Orphaned symlink: undefined */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((అక్షరం స్థిరరాశి [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, ఖాళీ },			/* ca: disabled by default */
    { 0, ఖాళీ },			/* mh: disabled by default */
    { 3, ((అక్షరం స్థిరరాశి [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
స్థిర నిర్మాణం color_ext_type *color_ext_list = ఖాళీ;

/* Buffer for color sequences */
స్థిర అక్షరం *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

స్థిర bool check_symlink_బహుళకం;

/* True means mention the inode number of each file.  -i  */

స్థిర bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

స్థిర గణన Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

స్థిర bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

స్థిర bool తక్షణ_dirs;

/* True means that directories are grouped before files. */

స్థిర bool directories_మొదటి;

/* Which files to ignore.  */

స్థిర గణన
{
  /* Ignore files whose names start with '.', and files specified by
     --hide and --ignore.  */
  IGNORE_DEFAULT = 0,

  /* Ignore '.', '..', and files specified by --ignore.  */
  IGNORE_DOT_AND_DOTDOT,

  /* Ignore only files specified by --ignore.  */
  IGNORE_కనీస
} ignore_బహుళకం;

/* A linked list of shell-style globbing patterns.  If a non-argument
   file name matches any of these patterns, it is ignored.
   Controlled by -I.  Multiple -I options accumulate.
   The -B option adds '*~' and '.*~' to this list.  */

నిర్మాణం ignore_నమూనా
  {
    అక్షరం స్థిరరాశి *నమూనా_2;
    నిర్మాణం ignore_నమూనా *తదుపరి;
  };

స్థిర నిర్మాణం ignore_నమూనా *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
స్థిర నిర్మాణం ignore_నమూనా *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
స్థిర bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
స్థిర bool qmark_newline;

/* Quoting options for file and dir name output.  */

స్థిర నిర్మాణం quoting_options *filename_quoting_options;
స్థిర నిర్మాణం quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
స్థిర size_t tabsize;

/* True means print each directory name before listing it.  */

స్థిర bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

స్థిర size_t పంక్తి_నిడివి;

/* The local time zone rules, as per the TZ environment variable.  */

స్థిర timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

స్థిర bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

స్థిర bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

స్థిర bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

గణన { సమయం_STAMP_LEN_గరిష్ఠ = MAX (1000, INT_STRLEN_BOUND (time_t)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

స్థిర అక్షరం స్థిరరాశి *long_సమయం_format[2] =
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

స్థిర sigset_t caught_సంకేతాలు;

/* If nonzero, the value of the pending fatal signal.  */

స్థిర sig_atomic_t అస్థిర అంతరాయం_signal;

/* A count of the number of pending stop signals that have been received.  */

స్థిర sig_atomic_t అస్థిర stop_signal_లెక్క;

/* Desired exit status.  */

స్థిర పూర్ణాంకం exit_స్థితి;

/* Exit statuses.  */
గణన
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
గణన
{
  AUTHOR_OPTION = CHAR_MAX + 1,
  ఖండం_కొలత_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR_OPTION,
  FILE_TYPE_INDICATOR_OPTION,
  FORMAT_OPTION,
  నిండిన_సమయం_OPTION,
  సమూహం_DIRECTORIES_మొదటి_OPTION,
  HIDE_OPTION,
  HYPERLINK_OPTION,
  INDICATOR_STYLE_OPTION,
  QUOTING_STYLE_OPTION,
  SHOW_CONTROL_CHARS_OPTION,
  SI_OPTION,
  క్రమీకరించు_OPTION,
  సమయం_OPTION,
  సమయం_STYLE_OPTION,
  ZERO_OPTION,
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"escape", no_వాదం, ఖాళీ, 'b'},
  {"directory", no_వాదం, ఖాళీ, 'd'},
  {"dired", no_వాదం, ఖాళీ, 'D'},
  {"full-time", no_వాదం, ఖాళీ, నిండిన_సమయం_OPTION},
  {"group-directories-first", no_వాదం, ఖాళీ,
   సమూహం_DIRECTORIES_మొదటి_OPTION},
  {"human-readable", no_వాదం, ఖాళీ, 'h'},
  {"inode", no_వాదం, ఖాళీ, 'i'},
  {"kibibytes", no_వాదం, ఖాళీ, 'k'},
  {"numeric-uid-gid", no_వాదం, ఖాళీ, 'n'},
  {"no-group", no_వాదం, ఖాళీ, 'G'},
  {"hide-control-chars", no_వాదం, ఖాళీ, 'q'},
  {"reverse", no_వాదం, ఖాళీ, 'r'},
  {"size", no_వాదం, ఖాళీ, 's'},
  {"width", required_వాదం, ఖాళీ, 'w'},
  {"almost-all", no_వాదం, ఖాళీ, 'A'},
  {"ignore-backups", no_వాదం, ఖాళీ, 'B'},
  {"classify", optional_వాదం, ఖాళీ, 'F'},
  {"file-type", no_వాదం, ఖాళీ, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_వాదం, ఖాళీ, SI_OPTION},
  {"dereference-command-line", no_వాదం, ఖాళీ, 'H'},
  {"dereference-command-line-symlink-to-dir", no_వాదం, ఖాళీ,
   DEREFERENCE_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR_OPTION},
  {"hide", required_వాదం, ఖాళీ, HIDE_OPTION},
  {"ignore", required_వాదం, ఖాళీ, 'I'},
  {"indicator-style", required_వాదం, ఖాళీ, INDICATOR_STYLE_OPTION},
  {"dereference", no_వాదం, ఖాళీ, 'L'},
  {"literal", no_వాదం, ఖాళీ, 'N'},
  {"quote-name", no_వాదం, ఖాళీ, 'Q'},
  {"quoting-style", required_వాదం, ఖాళీ, QUOTING_STYLE_OPTION},
  {"recursive", no_వాదం, ఖాళీ, 'R'},
  {"format", required_వాదం, ఖాళీ, FORMAT_OPTION},
  {"show-control-chars", no_వాదం, ఖాళీ, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_వాదం, ఖాళీ, క్రమీకరించు_OPTION},
  {"tabsize", required_వాదం, ఖాళీ, 'T'},
  {"time", required_వాదం, ఖాళీ, సమయం_OPTION},
  {"time-style", required_వాదం, ఖాళీ, సమయం_STYLE_OPTION},
  {"zero", no_వాదం, ఖాళీ, ZERO_OPTION},
  {"color", optional_వాదం, ఖాళీ, COLOR_OPTION},
  {"hyperlink", optional_వాదం, ఖాళీ, HYPERLINK_OPTION},
  {"block-size", required_వాదం, ఖాళీ, ఖండం_కొలత_OPTION},
  {"context", no_వాదం, ఖాళీ, 'Z'},
  {"author", no_వాదం, ఖాళీ, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి format_వాదనలు[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", ఖాళీ
};
స్థిర గణన format స్థిరరాశి format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_చొప్పున_పంక్తి, one_చొప్పున_పంక్తి
};
ARGMATCH_VERIFY (format_వాదనలు, format_types);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి క్రమీకరించు_వాదనలు[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", ఖాళీ
};
స్థిర గణన క్రమీకరించు_type స్థిరరాశి క్రమీకరించు_types[] =
{
  క్రమీకరించు_none, క్రమీకరించు_కొలత, క్రమీకరించు_సమయం, క్రమీకరించు_సంస్కరణ, క్రమీకరించు_extension,
  క్రమీకరించు_name, క్రమీకరించు_వెడల్పు
};
ARGMATCH_VERIFY (క్రమీకరించు_వాదనలు, క్రమీకరించు_types);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_వాదనలు[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  ఖాళీ
};
స్థిర గణన సమయం_type స్థిరరాశి సమయం_types[] =
{
  సమయం_atime, సమయం_atime, సమయం_atime,
  సమయం_ctime, సమయం_ctime,
  సమయం_mtime, సమయం_mtime,
  సమయం_btime, సమయం_btime,
};
ARGMATCH_VERIFY (సమయం_వాదనలు, సమయం_types);

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి when_వాదనలు[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", ఖాళీ
};
స్థిర గణన when_type స్థిరరాశి when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_వాదనలు, when_types);

/* Information about filling a column.  */
నిర్మాణం స్తంభం_info
{
  bool valid_len;
  size_t పంక్తి_len;
  size_t *నిలువు_arr;
};

/* Array with information about column fullness.  */
స్థిర నిర్మాణం స్తంభం_info *స్తంభం_info;

/* Maximum number of columns ever possible for this display.  */
స్థిర size_t max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
గణన { MIN_స్తంభం_వెడల్పు = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
స్థిర off_t dired_pos;

స్థిర శూన్య
dired_outbyte (అక్షరం c)
{
  dired_pos++;
  అక్షరం_రాయి (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
స్థిర శూన్య
dired_outbuf (అక్షరం స్థిరరాశి *s, size_t s_len)
{
  dired_pos += s_len;
  fwrite (s, పరిమాణం *s, s_len, stdout);
}

/* Output the string S, and increment DIRED_POS by its length.  */
స్థిర శూన్య
dired_outstring (అక్షరం స్థిరరాశి *s)
{
  dired_outbuf (s, అక్షరశ్రేణి_పొడవు (s));
}

స్థిర శూన్య
dired_indent (శూన్య)
{
  ఒకవేళ (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
స్థిర నిర్మాణం obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
స్థిర నిర్మాణం obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
స్థిర శూన్య
నెట్టు_current_dired_pos (నిర్మాణం obstack *obs)
{
  ఒకవేళ (dired)
    obstack_grow (obs, &dired_pos, పరిమాణం dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
స్థిర నిర్మాణం obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
స్థిర శూన్య
dev_ino_నెట్టు (dev_t dev, ino_t ino)
{
  శూన్య *vdi;
  నిర్మాణం dev_ino *di;
  పూర్ణాంకం dev_ino_కొలత = పరిమాణం *di;
  obstack_blank (&dev_ino_obstack, dev_ino_కొలత);
  vdi = obstack_తదుపరి_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
స్థిర నిర్మాణం dev_ino
dev_ino_తీయి (శూన్య)
{
  శూన్య *vdi;
  నిర్మాణం dev_ino *di;
  పూర్ణాంకం dev_ino_కొలత = పరిమాణం *di;
  affirm (dev_ino_కొలత <= obstack_object_కొలత (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_కొలత);
  vdi = obstack_తదుపరి_free (&dev_ino_obstack);
  di = vdi;
  తిరిగి *di;
}

స్థిర శూన్య
assert_matching_dev_ino (అక్షరం స్థిరరాశి *name, నిర్మాణం dev_ino di)
{
  MAYBE_వాడని నిర్మాణం stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

స్థిర అక్షరం eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

స్థిర శూన్య
dired_dump_obstack (అక్షరం స్థిరరాశి *ఉపసర్గ, నిర్మాణం obstack *os)
{
  size_t n_pos;

  n_pos = obstack_object_కొలత (os) / పరిమాణం (dired_pos);
  ఒకవేళ (n_pos > 0)
    {
      off_t *pos = obstack_ముగింపు (os);
      fputs (ఉపసర్గ, stdout);
      కొరకు (size_t i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          రాయి (" %jd", p);
        }
      అక్షరం_రాయి ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
స్థిర నిర్మాణం timespec
get_stat_btime (నిర్మాణం stat స్థిరరాశి *st)
{
  నిర్మాణం timespec btimespec;

#ఒకవేళ HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#లేకపోతే
  btimespec = get_stat_birthtime (st);
#అయితే_ముగింపు

  తిరిగి btimespec;
}

#ఒకవేళ HAVE_STATX && defined STATX_INO
ATTRIBUTE_శుద్ధ
స్థిర గుర్తులేని పూర్ణాంకం
సమయం_type_గమ్యం_statx (శూన్య)
{
  ఎంపిక (సమయం_type)
    {
    స్థితి సమయం_ctime:
      తిరిగి STATX_CTIME;
    స్థితి సమయం_mtime:
      తిరిగి STATX_MTIME;
    స్థితి సమయం_atime:
      తిరిగి STATX_ATIME;
    స్థితి సమయం_btime:
      తిరిగి STATX_BTIME;
    స్థితి సమయం_numtypes: అప్రమేయ:
      unreachable ();
    }
    తిరిగి 0;
}

ATTRIBUTE_శుద్ధ
స్థిర గుర్తులేని పూర్ణాంకం
calc_req_ముసుగు (శూన్య)
{
  గుర్తులేని పూర్ణాంకం ముసుగు = STATX_బహుళకం;

  ఒకవేళ (print_inode)
    ముసుగు |= STATX_INO;

  ఒకవేళ (print_ఖండం_కొలత)
    ముసుగు |= STATX_ఖండాలు;

  ఒకవేళ (format == long_format) {
    ముసుగు |= STATX_NLINK | STATX_కొలత | సమయం_type_గమ్యం_statx ();
    ఒకవేళ (print_owner || print_author)
      ముసుగు |= STATX_UID;
    ఒకవేళ (print_సమూహం)
      ముసుగు |= STATX_GID;
  }

  ఎంపిక (క్రమీకరించు_type)
    {
    స్థితి క్రమీకరించు_none:
    స్థితి క్రమీకరించు_name:
    స్థితి క్రమీకరించు_సంస్కరణ:
    స్థితి క్రమీకరించు_extension:
    స్థితి క్రమీకరించు_వెడల్పు:
      ఆపు;
    స్థితి క్రమీకరించు_సమయం:
      ముసుగు |= సమయం_type_గమ్యం_statx ();
      ఆపు;
    స్థితి క్రమీకరించు_కొలత:
      ముసుగు |= STATX_కొలత;
      ఆపు;
    స్థితి క్రమీకరించు_numtypes: అప్రమేయ:
      unreachable ();
    }

  తిరిగి ముసుగు;
}

స్థిర పూర్ణాంకం
do_statx (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st, పూర్ణాంకం flags,
          గుర్తులేని పూర్ణాంకం ముసుగు)
{
  నిర్మాణం statx stx;
  bool want_btime = ముసుగు & STATX_BTIME;
  పూర్ణాంకం ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, ముసుగు, &stx);
  ఒకవేళ (ret >= 0)
    {
      statx_గమ్యం_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      ఒకవేళ (want_btime)
        {
          ఒకవేళ (stx.stx_ముసుగు & STATX_BTIME)
            st->st_mtim = statx_timestamp_గమ్యం_timespec (stx.stx_btime);
          లేకపోతే
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
do_stat (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి do_statx (AT_FDCWD, name, st, 0, calc_req_ముసుగు ());
}

స్థిర పూర్ణాంకం
do_lstat (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_ముసుగు ());
}

స్థిర పూర్ణాంకం
stat_for_బహుళకం (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి do_statx (AT_FDCWD, name, st, 0, STATX_బహుళకం);
}

/* dev+ino should be static, so no need to sync with backing store */
స్థిర పూర్ణాంకం
stat_for_ino (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

స్థిర పూర్ణాంకం
fstat_for_ino (పూర్ణాంకం fd, నిర్మాణం stat *st)
{
  తిరిగి do_statx (fd, "", st, AT_ఖాళీలు_మార్గం, STATX_INO);
}
#లేకపోతే
స్థిర పూర్ణాంకం
do_stat (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి stat (name, st);
}

స్థిర పూర్ణాంకం
do_lstat (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి lstat (name, st);
}

స్థిర పూర్ణాంకం
stat_for_బహుళకం (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి stat (name, st);
}

స్థిర పూర్ణాంకం
stat_for_ino (అక్షరం స్థిరరాశి *name, నిర్మాణం stat *st)
{
  తిరిగి stat (name, st);
}

స్థిర పూర్ణాంకం
fstat_for_ino (పూర్ణాంకం fd, నిర్మాణం stat *st)
{
  తిరిగి fstat (fd, st);
}
#అయితే_ముగింపు

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_శుద్ధ
స్థిర అక్షరం స్థిరరాశి *
మొదటి_percent_b (అక్షరం స్థిరరాశి *fmt)
{
  కొరకు (; *fmt; fmt++)
    ఒకవేళ (fmt[0] == '%')
      ఎంపిక (fmt[1])
        {
        స్థితి 'b': తిరిగి fmt;
        స్థితి '%': fmt++; ఆపు;
        }
  తిరిగి ఖాళీ;
}

స్థిర అక్షరం RFC3986[256];
స్థిర శూన్య
file_escape_init (శూన్య)
{
  కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

గణన { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
గణన { ABFORMAT_కొలత = 128 };
స్థిర అక్షరం abformat[2][12][ABFORMAT_కొలత];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
స్థిర bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

స్థిర bool
abmon_init (అక్షరం abmon[12][ABFORMAT_కొలత])
{
#నిర్వచితం_కాకపోతే HAVE_NL_LANGINFO
  తిరిగి false;
#లేకపోతే
  పూర్ణాంకం max_mon_వెడల్పు = 0;
  పూర్ణాంకం mon_వెడల్పు[12];
  పూర్ణాంకం mon_len[12];

  కొరకు (పూర్ణాంకం i = 0; i < 12; i++)
    {
      అక్షరం స్థిరరాశి *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_కొలత);
      ఒకవేళ (mon_len[i] == ABFORMAT_కొలత)
        తిరిగి false;
      ఒకవేళ (strchr (abbr, '%'))
        తిరిగి false;
      mon_వెడల్పు[i] = mbswidth (strcpy (abmon[i], abbr), MBSWIDTH_FLAGS);
      ఒకవేళ (mon_వెడల్పు[i] < 0)
        తిరిగి false;
      max_mon_వెడల్పు = MAX (max_mon_వెడల్పు, mon_వెడల్పు[i]);
    }

  కొరకు (పూర్ణాంకం i = 0; i < 12; i++)
    {
      పూర్ణాంకం fill = max_mon_వెడల్పు - mon_వెడల్పు[i];
      ఒకవేళ (ABFORMAT_కొలత - mon_len[i] <= fill)
        తిరిగి false;
      bool align_ఎడమ = !c_isdigit (abmon[i][0]);
      పూర్ణాంకం fill_స్థానభ్రంశం;
      ఒకవేళ (align_ఎడమ)
        fill_స్థానభ్రంశం = mon_len[i];
      లేకపోతే
        {
          memmove (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_స్థానభ్రంశం = 0;
        }
      memset (abmon[i] + fill_స్థానభ్రంశం, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  తిరిగి true;
#అయితే_ముగింపు
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

స్థిర శూన్య
abformat_init (శూన్య)
{
  అక్షరం స్థిరరాశి *pb[2];
  కొరకు (పూర్ణాంకం recent = 0; recent < 2; recent++)
    pb[recent] = మొదటి_percent_b (long_సమయం_format[recent]);
  ఒకవేళ (! (pb[0] || pb[1]))
    తిరిగి;

  అక్షరం abmon[12][ABFORMAT_కొలత];
  ఒకవేళ (! abmon_init (abmon))
    తిరిగి;

  కొరకు (పూర్ణాంకం recent = 0; recent < 2; recent++)
    {
      అక్షరం స్థిరరాశి *fmt = long_సమయం_format[recent];
      కొరకు (పూర్ణాంకం i = 0; i < 12; i++)
        {
          అక్షరం *nfmt = abformat[recent][i];
          పూర్ణాంకం nbytes;

          ఒకవేళ (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_కొలత, "%s", fmt);
          లేకపోతే
            {
              ఒకవేళ (! (pb[recent] - fmt <= MIN (ABFORMAT_కొలత, గరిష్ఠ_పూర్ణాంకం)))
                తిరిగి;
              పూర్ణాంకం ఉపసర్గ_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_కొలత, "%.*s%s%s",
                                 ఉపసర్గ_len, fmt, abmon[i], pb[recent] + 2);
            }

          ఒకవేళ (! (0 <= nbytes && nbytes < ABFORMAT_కొలత))
            తిరిగి;
        }
    }

  use_abformat = true;
}

స్థిర size_t
dev_ino_hash (శూన్య స్థిరరాశి *x, size_t పట్టిక_కొలత)
{
  నిర్మాణం dev_ino స్థిరరాశి *p = x;
  తిరిగి (uintmax_t) {p->st_ino} % పట్టిక_కొలత;
}

స్థిర bool
dev_ino_compare (శూన్య స్థిరరాశి *x, శూన్య స్థిరరాశి *y)
{
  నిర్మాణం dev_ino స్థిరరాశి *a = x;
  నిర్మాణం dev_ino స్థిరరాశి *b = y;
  తిరిగి PSAME_INODE (a, b);
}

స్థిర శూన్య
dev_ino_free (శూన్య *x)
{
  విడుదల (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

స్థిర bool
సందర్శించు_dir (dev_t dev, ino_t ino)
{
  నిర్మాణం dev_ino *ent;
  నిర్మాణం dev_ino *ent_from_పట్టిక;
  bool దొరికింది_సరిపోలిక;

  ent = xmalloc (పరిమాణం *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_పట్టిక = hash_చొప్పించు (చురుకైన_dir_సమితి, ent);

  ఒకవేళ (ent_from_పట్టిక == ఖాళీ)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  దొరికింది_సరిపోలిక = (ent_from_పట్టిక != ent);

  ఒకవేళ (దొరికింది_సరిపోలిక)
    {
      /* ent was not inserted, so free it.  */
      విడుదల (ent);
    }

  తిరిగి దొరికింది_సరిపోలిక;
}

స్థిర శూన్య
free_pending_ent (నిర్మాణం pending *p)
{
  విడుదల (p->name);
  విడుదల (p->realname);
  విడుదల (p);
}

స్థిర bool
is_colored (గణన indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  size_t len = color_indicator[type].len;
  ఒకవేళ (len == 0)
    తిరిగి false;
  ఒకవేళ (2 < len)
    తిరిగి true;
  అక్షరం స్థిరరాశి *s = color_indicator[type].string;
  తిరిగి (s[0] != '0') | (s[len - 1] != '0');
}

స్థిర శూన్య
restore_default_color (శూన్య)
{
  put_indicator (&color_indicator[C_ఎడమ]);
  put_indicator (&color_indicator[C_కుడి]);
}

స్థిర శూన్య
సమితి_సాధారణ_color (శూన్య)
{
  ఒకవేళ (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_ఎడమ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_కుడి]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

స్థిర శూన్య
sighandler (పూర్ణాంకం sig)
{
  ఒకవేళ (! అంతరాయం_signal)
    అంతరాయం_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

స్థిర శూన్య
stophandler (MAYBE_వాడని పూర్ణాంకం sig)
{
  ఒకవేళ (! అంతరాయం_signal)
    stop_signal_లెక్క++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

స్థిర శూన్య
process_సంకేతాలు (శూన్య)
{
  అయితే (అంతరాయం_signal || stop_signal_లెక్క)
    {
      పూర్ణాంకం sig;
      పూర్ణాంకం stops;
      sigset_t oldset;

      ఒకవేళ (వాడిన_color)
        restore_default_color ();
      fflush (stdout);

      sigprocmask (SIG_BLOCK, &caught_సంకేతాలు, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = అంతరాయం_signal;
      stops = stop_signal_లెక్క;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      ఒకవేళ (stops)
        {
          stop_signal_లెక్క = stops - 1;
          sig = SIGSTOP;
        }
      లేకపోతే
        సంకేతం (sig, SIG_DFL);

      /* Exit or suspend the program.  */
      లేపు (sig);
      sigprocmask (SIG_SETMASK, &oldset, ఖాళీ);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

స్థిర శూన్య
signal_అమరిక (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  స్థిర పూర్ణాంకం స్థిరరాశి stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  గణన { nsigs = countof (పదం_sig), nstop = countof (stop_sig) };

  ఒకవేళ (init)
    {
      నిర్మాణం sigaction act;

      sigemptyset (&caught_సంకేతాలు);
      కొరకు (పూర్ణాంకం j = 0; j < nsigs + nstop; j++)
        {
          పూర్ణాంకం sig = j < nsigs ? పదం_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, ఖాళీ, &act);
          ఒకవేళ (act.sa_handler != SIG_IGN)
            sigaddset (&caught_సంకేతాలు, sig);
        }

      act.sa_mask = caught_సంకేతాలు;
      act.sa_flags = SA_RESTART;

      కొరకు (పూర్ణాంకం j = 0; j < nsigs + nstop; j++)
        {
          పూర్ణాంకం sig = j < nsigs ? పదం_sig[j]: stop_sig[j - nsigs];
          ఒకవేళ (sigismember (&caught_సంకేతాలు, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, ఖాళీ);
            }
        }
    }
  లేకపోతే /* restore.  */
    {
      కొరకు (పూర్ణాంకం j = 0; j < nsigs + nstop; j++)
        {
          పూర్ణాంకం sig = j < nsigs ? పదం_sig[j]: stop_sig[j - nsigs];
          ఒకవేళ (sigismember (&caught_సంకేతాలు, sig))
            సంకేతం (sig, SIG_DFL);
        }
    }
}

స్థిర శూన్య
signal_init (శూన్య)
{
  signal_అమరిక (true);
}

స్థిర శూన్య
signal_restore (శూన్య)
{
  signal_అమరిక (false);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం i;
  నిర్మాణం pending *thispend;
  పూర్ణాంకం n_files;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  atexit (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_స్థితి = EXIT_SUCCESS;
  print_dir_name = true;
  pending_dirs = ఖాళీ;

  current_సమయం.tv_sec = TYPE_MINIMUM (time_t);
  current_సమయం.tv_nsec = -1;

  i = decode_switches (argc, argv);

  ఒకవేళ (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  ఒకవేళ (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  ఒకవేళ (directories_మొదటి)
    check_symlink_బహుళకం = true;
  లేకపోతే ఒకవేళ (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      ఒకవేళ (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_బహుళకం = true;
    }

  ఒకవేళ (dereference == DEREF_UNDEFINED)
    dereference = ((తక్షణ_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  ఒకవేళ (recursive)
    {
      చురుకైన_dir_సమితి = hash_initialize (INITIAL_పట్టిక_కొలత, ఖాళీ,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      ఒకవేళ (చురుకైన_dir_సమితి == ఖాళీ)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (getenv ("TZ"));

  format_needs_stat = ((క్రమీకరించు_type == క్రమీకరించు_సమయం) | (క్రమీకరించు_type == క్రమీకరించు_కొలత)
                       | (format == long_format)
                       | print_ఖండం_కొలత | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_మొదటి
                          | (indicator_style != none)));
  format_needs_capability = print_with_color && is_colored (C_CAP);

  ఒకవేళ (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  ఒకవేళ (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      ఒకవేళ (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * పరిమాణం *cwd_file);
  cwd_n_వాడిన = 0;

  clear_files ();

  n_files = argc - i;

  ఒకవేళ (n_files <= 0)
    {
      ఒకవేళ (తక్షణ_dirs)
        gobble_file (".", directory, NOT_AN_INODE_సంఖ్య, true, ఖాళీ);
      లేకపోతే
        వరుస_directory (".", ఖాళీ, true);
    }
  లేకపోతే
    చేయి
      gobble_file (argv[i++], unknown, NOT_AN_INODE_సంఖ్య, true, ఖాళీ);
    అయితే (i < argc);

  ఒకవేళ (cwd_n_వాడిన)
    {
      క్రమీకరించు_files ();
      ఒకవేళ (!తక్షణ_dirs)
        వెలికితీయి_dirs_from_files (ఖాళీ, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  ఒకవేళ (cwd_n_వాడిన)
    {
      print_current_files ();
      ఒకవేళ (pending_dirs)
        dired_outbyte ('\n');
    }
  లేకపోతే ఒకవేళ (n_files <= 1 && pending_dirs && !pending_dirs->తదుపరి)
    print_dir_name = false;

  అయితే (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->తదుపరి;

      ఒకవేళ (LOOP_DETECT)
        {
          ఒకవేళ (thispend->name == ఖాళీ)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              నిర్మాణం dev_ino di = dev_ino_తీయి ();
              నిర్మాణం dev_ino *దొరికింది = hash_remove (చురుకైన_dir_సమితి, &di);
              ఒకవేళ (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (దొరికింది);
              dev_ino_free (దొరికింది);
              free_pending_ent (thispend);
              కొనసాగు;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_పంక్తి_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  ఒకవేళ (print_with_color && వాడిన_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      ఒకవేళ (!(color_indicator[C_ఎడమ].len == 2
            && memeq (color_indicator[C_ఎడమ].string, "\033[", 2)
            && color_indicator[C_కుడి].len == 1
            && color_indicator[C_కుడి].string[0] == 'm'))
        restore_default_color ();

      fflush (stdout);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      కొరకు (పూర్ణాంకం j = stop_signal_లెక్క; j; j--)
        లేపు (SIGSTOP);
      పూర్ణాంకం j = అంతరాయం_signal;
      ఒకవేళ (j)
        లేపు (j);
    }

  ఒకవేళ (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      రాయి ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_వాదనలు[get_quoting_style (filename_quoting_options)]);
    }

  ఒకవేళ (LOOP_DETECT)
    {
      assure (hash_get_n_entries (చురుకైన_dir_సమితి) == 0);
      hash_free (చురుకైన_dir_సమితి);
    }

  తిరిగి exit_స్థితి;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

స్థిర ptrdiff_t
decode_పంక్తి_నిడివి (అక్షరం స్థిరరాశి *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  ఎంపిక (xstrtoumax (spec, ఖాళీ, 0, &val, ""))
    {
    స్థితి LONGINT_OK:
      తిరిగి val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    స్థితి LONGINT_అధికప్రవాహం:
      తిరిగి 0;

    స్థితి LONGINT_INVALID:
    స్థితి LONGINT_INVALID_SUFFIX_CHAR:
    స్థితి LONGINT_INVALID_SUFFIX_CHAR_WITH_అధికప్రవాహం:
      తిరిగి -1;

    అప్రమేయ:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

స్థిర bool
stdout_isatty (శూన్య)
{
  స్థిర గుర్తుగల అక్షరం బహిర్గతం_tty = -1;
  ఒకవేళ (బహిర్గతం_tty < 0)
    బహిర్గతం_tty = isatty (STDOUT_FILENO);
  assume (బహిర్గతం_tty == 0 || బహిర్గతం_tty == 1);
  తిరిగి బహిర్గతం_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

స్థిర పూర్ణాంకం
decode_switches (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *సమయం_style_option = ఖాళీ;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  పూర్ణాంకం format_opt = -1;
  పూర్ణాంకం hide_control_chars_opt = -1;
  పూర్ణాంకం quoting_style_opt = -1;
  పూర్ణాంకం క్రమీకరించు_opt = -1;
  ptrdiff_t tabsize_opt = -1;
  ptrdiff_t వెడల్పు_opt = -1;

  అయితే (true)
    {
      పూర్ణాంకం oi = -1;
      పూర్ణాంకం c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 'a':
          ignore_బహుళకం = IGNORE_కనీస;
          ఆపు;

        స్థితి 'b':
          quoting_style_opt = escape_quoting_style;
          ఆపు;

        స్థితి 'c':
          సమయం_type = సమయం_ctime;
          explicit_సమయం = true;
          ఆపు;

        స్థితి 'd':
          తక్షణ_dirs = true;
          ఆపు;

        స్థితి 'f':
          ignore_బహుళకం = IGNORE_కనీస; /* enable -a */
          క్రమీకరించు_opt = క్రమీకరించు_none;         /* enable -U */
          ఆపు;

        స్థితి FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          ఆపు;

        స్థితి 'g':
          format_opt = long_format;
          print_owner = false;
          ఆపు;

        స్థితి 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_పునాది_1024;
          file_output_ఖండం_కొలత = output_ఖండం_కొలత = 1;
          ఆపు;

        స్థితి 'i':
          print_inode = true;
          ఆపు;

        స్థితి 'k':
          kibibytes_specified = true;
          ఆపు;

        స్థితి 'l':
          format_opt = long_format;
          ఆపు;

        స్థితి 'm':
          format_opt = with_commas;
          ఆపు;

        స్థితి 'n':
          numeric_ids = true;
          format_opt = long_format;
          ఆపు;

        స్థితి 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_సమూహం = false;
          ఆపు;

        స్థితి 'p':
          indicator_style = slash;
          ఆపు;

        స్థితి 'q':
          hide_control_chars_opt = true;
          ఆపు;

        స్థితి 'r':
          క్రమీకరించు_తిరగేయి = true;
          ఆపు;

        స్థితి 's':
          print_ఖండం_కొలత = true;
          ఆపు;

        స్థితి 't':
          క్రమీకరించు_opt = క్రమీకరించు_సమయం;
          ఆపు;

        స్థితి 'u':
          సమయం_type = సమయం_atime;
          explicit_సమయం = true;
          ఆపు;

        స్థితి 'v':
          క్రమీకరించు_opt = క్రమీకరించు_సంస్కరణ;
          ఆపు;

        స్థితి 'w':
          వెడల్పు_opt = decode_పంక్తి_నిడివి (optarg);
          ఒకవేళ (వెడల్పు_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          ఆపు;

        స్థితి 'x':
          format_opt = horizontal;
          ఆపు;

        స్థితి 'A':
          ignore_బహుళకం = IGNORE_DOT_AND_DOTDOT;
          ఆపు;

        స్థితి 'B':
          కూడు_ignore_నమూనా ("*~");
          కూడు_ignore_నమూనా (".*~");
          ఆపు;

        స్థితి 'C':
          format_opt = many_చొప్పున_పంక్తి;
          ఆపు;

        స్థితి 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          ఆపు;

        స్థితి 'F':
          {
            పూర్ణాంకం i;
            ఒకవేళ (optarg)
              i = XARGMATCH ("--classify", optarg, when_వాదనలు, when_types);
            లేకపోతే
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            ఒకవేళ (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            ఆపు;
          }

        స్థితి 'G':		/* inhibit display of group info */
          print_సమూహం = false;
          ఆపు;

        స్థితి 'H':
          dereference = DEREF_COMMAND_పంక్తి_ARGUMENTS;
          ఆపు;

        స్థితి DEREFERENCE_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR_OPTION:
          dereference = DEREF_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR;
          ఆపు;

        స్థితి 'I':
          కూడు_ignore_నమూనా (optarg);
          ఆపు;

        స్థితి 'L':
          dereference = DEREF_ALWAYS;
          ఆపు;

        స్థితి 'N':
          quoting_style_opt = literal_quoting_style;
          ఆపు;

        స్థితి 'Q':
          quoting_style_opt = c_quoting_style;
          ఆపు;

        స్థితి 'R':
          recursive = true;
          ఆపు;

        స్థితి 'S':
          క్రమీకరించు_opt = క్రమీకరించు_కొలత;
          ఆపు;

        స్థితి 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          ఆపు;

        స్థితి 'U':
          క్రమీకరించు_opt = క్రమీకరించు_none;
          ఆపు;

        స్థితి 'X':
          క్రమీకరించు_opt = క్రమీకరించు_extension;
          ఆపు;

        స్థితి '1':
          /* -1 has no effect after -l.  */
          ఒకవేళ (format_opt != long_format)
            format_opt = one_చొప్పున_పంక్తి;
          ఆపు;

        స్థితి AUTHOR_OPTION:
          print_author = true;
          ఆపు;

        స్థితి HIDE_OPTION:
          {
            నిర్మాణం ignore_నమూనా *hide = xmalloc (పరిమాణం *hide);
            hide->నమూనా_2 = optarg;
            hide->తదుపరి = hide_patterns;
            hide_patterns = hide;
          }
          ఆపు;

        స్థితి క్రమీకరించు_OPTION:
          క్రమీకరించు_opt = XARGMATCH ("--sort", optarg, క్రమీకరించు_వాదనలు, క్రమీకరించు_types);
          ఆపు;

        స్థితి సమూహం_DIRECTORIES_మొదటి_OPTION:
          directories_మొదటి = true;
          ఆపు;

        స్థితి సమయం_OPTION:
          సమయం_type = XARGMATCH ("--time", optarg, సమయం_వాదనలు, సమయం_types);
          explicit_సమయం = true;
          ఆపు;

        స్థితి FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_వాదనలు,
                                  format_types);
          ఆపు;

        స్థితి నిండిన_సమయం_OPTION:
          format_opt = long_format;
          సమయం_style_option = (అక్షరం *) "full-iso";
          ఆపు;

        స్థితి COLOR_OPTION:
          {
            పూర్ణాంకం i;
            ఒకవేళ (optarg)
              i = XARGMATCH ("--color", optarg, when_వాదనలు, when_types);
            లేకపోతే
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            ఆపు;
          }

        స్థితి HYPERLINK_OPTION:
          {
            పూర్ణాంకం i;
            ఒకవేళ (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_వాదనలు, when_types);
            లేకపోతే
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            ఆపు;
          }

        స్థితి INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_వాదనలు,
                                       indicator_style_types);
          ఆపు;

        స్థితి QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_వాదనలు,
                                         quoting_style_vals);
          ఆపు;

        స్థితి సమయం_STYLE_OPTION:
          సమయం_style_option = optarg;
          ఆపు;

        స్థితి SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          ఆపు;

        స్థితి ఖండం_కొలత_OPTION:
          {
            గణన strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ఖండం_కొలత);
            ఒకవేళ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_ఖండం_కొలత = output_ఖండం_కొలత;
          }
          ఆపు;

        స్థితి SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_ఖండం_కొలత = output_ఖండం_కొలత = 1;
          ఆపు;

        స్థితి 'Z':
          print_scontext = true;
          ఆపు;

        స్థితి ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          ఒకవేళ (format_opt != long_format)
            format_opt = one_చొప్పున_పంక్తి;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (LS_FAILURE);
        }
    }

  ఒకవేళ (! output_ఖండం_కొలత)
    {
      అక్షరం స్థిరరాశి *ls_ఖండం_కొలత = getenv ("LS_BLOCK_SIZE");
      human_options (ls_ఖండం_కొలత,
                     &human_output_opts, &output_ఖండం_కొలత);
      ఒకవేళ (ls_ఖండం_కొలత || getenv ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_ఖండం_కొలత = output_ఖండం_కొలత;
        }
      ఒకవేళ (kibibytes_specified)
        {
          human_output_opts = 0;
          output_ఖండం_కొలత = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_బహుళకం == LS_LS ? (stdout_isatty ()
                                  ? many_చొప్పున_పంక్తి : one_చొప్పున_పంక్తి)
            : ls_బహుళకం == LS_MULTI_నిలువు ? many_చొప్పున_పంక్తి
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  ptrdiff_t linelen = వెడల్పు_opt;
  ఒకవేళ (format == many_చొప్పున_పంక్తి || format == horizontal || format == with_commas
      || print_with_color)
    {
#నిర్వచితమైతే TIOCGWINSZ
      ఒకవేళ (linelen < 0)
        {
          నిర్మాణం winsize ws;
          ఒకవేళ (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_నిలువు)
            linelen = ws.ws_నిలువు <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_నిలువు : 0;
        }
#అయితే_ముగింపు
      ఒకవేళ (linelen < 0)
        {
          అక్షరం స్థిరరాశి *p = getenv ("COLUMNS");
          ఒకవేళ (p && *p)
            {
              linelen = decode_పంక్తి_నిడివి (p);
              ఒకవేళ (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  పంక్తి_నిడివి = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = పంక్తి_నిడివి / MIN_స్తంభం_వెడల్పు;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += పంక్తి_నిడివి % MIN_స్తంభం_వెడల్పు != 0;

  ఒకవేళ (format == many_చొప్పున_పంక్తి || format == horizontal || format == with_commas)
    {
      ఒకవేళ (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      లేకపోతే
        {
          tabsize = 8;
          అక్షరం స్థిరరాశి *p = getenv ("TABSIZE");
          ఒకవేళ (p)
            {
              uintmax_t tmp;
              ఒకవేళ (xstrtoumax (p, ఖాళీ, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              లేకపోతే
                error (0, 0,
                       _("ignoring invalid tab size"
                         " in environment variable TABSIZE: %s"),
                       quote (p));
            }
        }
    }

  qmark_funny_chars = (hide_control_chars_opt < 0
                       ? ls_బహుళకం == LS_LS && stdout_isatty ()
                       : hide_control_chars_opt);

  qmark_newline = hide_control_chars_opt != false && eolbyte == '\n'
                  && ((పంక్తి_నిడివి
                       && (format == with_commas
                           || format == many_చొప్పున_పంక్తి || format == horizontal))
                      || format == one_చొప్పున_పంక్తి || format == long_format);

  పూర్ణాంకం qs = quoting_style_opt;
  ఒకవేళ (qs < 0)
    qs = getenv_quoting_style ();
  ఒకవేళ (qs < 0)
    qs = (ls_బహుళకం == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  ఒకవేళ (0 <= qs)
    సమితి_quoting_style (ఖాళీ, qs);
  qs = get_quoting_style (ఖాళీ);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_చొప్పున_పంక్తి || format == horizontal) && పంక్తి_నిడివి))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (ఖాళీ);
  ఒకవేళ (qs == escape_quoting_style)
    సమితి_char_quoting (filename_quoting_options, ' ', 1);
  ఒకవేళ (file_type <= indicator_style)
    {
      కొరకు (అక్షరం స్థిరరాశి *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        సమితి_char_quoting (filename_quoting_options, *p, 1);
    }
  ఒకవేళ (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    సమితి_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (ఖాళీ);
  సమితి_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  ఒకవేళ (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  క్రమీకరించు_type = (0 <= క్రమీకరించు_opt ? క్రమీకరించు_opt
               : (format != long_format && explicit_సమయం)
               ? క్రమీకరించు_సమయం : క్రమీకరించు_name);

  ఒకవేళ (format == long_format)
    {
      అక్షరం *envstyle = ఖాళీ;
      అక్షరం *style = సమయం_style_option;
      ఒకవేళ (! style)
        style = envstyle = getenv ("TIME_STYLE");

      ఒకవేళ (! style)
        style = (అక్షరం *) "locale";
      లేకపోతే
        {
          స్థిర అక్షరం స్థిరరాశి posix_ఉపసర్గ[] = "posix-";
          అయితే (STREQ_LEN (style, posix_ఉపసర్గ, పరిమాణం posix_ఉపసర్గ - 1))
            {
              ఒకవేళ (! hard_locale (LC_సమయం))
                తిరిగి optind;
              style += పరిమాణం posix_ఉపసర్గ - 1;
            }
        }

      ఒకవేళ (*style == '+')
        {
          అక్షరం *p0 = style + 1;
          అక్షరం *p0nl = strchr (p0, '\n');
          అక్షరం స్థిరరాశి *p1 = p0;
          ఒకవేళ (p0nl)
            {
              p1 = p0nl + 1;
              ఒకవేళ (strchr (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              ఒకవేళ (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              లేకపోతే
                *p0nl = '\0';
            }
          long_సమయం_format[0] = p0;
          long_సమయం_format[1] = p1;
        }
      లేకపోతే
        {
          ఎంపిక (x_timestyle_సరిపోలిక (style, /*allow_posix=*/ true,
                                     సమయం_style_వాదనలు,
                                     (అక్షరం స్థిరరాశి *) సమయం_style_types,
                                     పరిమాణం (*సమయం_style_types), LS_FAILURE))
            {
            స్థితి నిండిన_iso_సమయం_style:
              long_సమయం_format[0] = long_సమయం_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              ఆపు;

            స్థితి long_iso_సమయం_style:
              long_సమయం_format[0] = long_సమయం_format[1] = "%Y-%m-%d %H:%M";
              ఆపు;

            స్థితి iso_సమయం_style:
              long_సమయం_format[0] = "%Y-%m-%d ";
              long_సమయం_format[1] = "%m-%d %H:%M";
              ఆపు;

            స్థితి locale_సమయం_style:
              ఒకవేళ (hard_locale (LC_సమయం))
                {
                  కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
                    long_సమయం_format[i] =
                      dcgettext (ఖాళీ, long_సమయం_format[i], LC_సమయం);
                }
            }
        }

      abformat_init ();
    }

  తిరిగి optind;
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

స్థిర bool
get_funky_string (అక్షరం **dest, అక్షరం స్థిరరాశి **src, bool equals_చివర,
                  size_t *output_లెక్క)
{
  అక్షరం అంకె;			/* For numerical codes */
  size_t లెక్క;			/* Something to count with */
  గణన {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_చివర, ST_ERROR
  } state;
  అక్షరం స్థిరరాశి *p;
  అక్షరం *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  లెక్క = 0;			/* No characters counted in yet.  */
  అంకె = 0;

  state = ST_GND;		/* Start in ground state.  */
  అయితే (state < ST_చివర)
    {
      ఎంపిక (state)
        {
        స్థితి ST_GND:		/* Ground state (no escapes) */
          ఎంపిక (*p)
            {
            స్థితి ':':
            స్థితి '\0':
              state = ST_చివర;	/* End of string */
              ఆపు;
            స్థితి '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              ఆపు;
            స్థితి '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              ఆపు;
            స్థితి '=':
              ఒకవేళ (equals_చివర)
                {
                  state = ST_చివర; /*చివర*/
                  ఆపు;
                }
              FALLTHROUGH;
            అప్రమేయ:
              *(q++) = *(p++);
              ++లెక్క;
              ఆపు;
            }
          ఆపు;

        స్థితి ST_BACKSLASH:	/* Backslash escaped character */
          ఎంపిక (*p)
            {
            స్థితి '0':
            స్థితి '1':
            స్థితి '2':
            స్థితి '3':
            స్థితి '4':
            స్థితి '5':
            స్థితి '6':
            స్థితి '7':
              state = ST_OCTAL;	/* Octal sequence */
              అంకె = *p - '0';
              ఆపు;
            స్థితి 'x':
            స్థితి 'X':
              state = ST_HEX;	/* Hex sequence */
              అంకె = 0;
              ఆపు;
            స్థితి 'a':		/* Bell */
              అంకె = '\a';
              ఆపు;
            స్థితి 'b':		/* Backspace */
              అంకె = '\b';
              ఆపు;
            స్థితి 'e':		/* Escape */
              అంకె = 27;
              ఆపు;
            స్థితి 'f':		/* Form feed */
              అంకె = '\f';
              ఆపు;
            స్థితి 'n':		/* Newline */
              అంకె = '\n';
              ఆపు;
            స్థితి 'r':		/* Carriage return */
              అంకె = '\r';
              ఆపు;
            స్థితి 't':		/* Tab */
              అంకె = '\t';
              ఆపు;
            స్థితి 'v':		/* Vtab */
              అంకె = '\v';
              ఆపు;
            స్థితి '?':		/* Delete */
              అంకె = 127;
              ఆపు;
            స్థితి '_':		/* Space */
              అంకె = ' ';
              ఆపు;
            స్థితి '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              ఆపు;
            అప్రమేయ:		/* Escaped character like \ ^ : = */
              అంకె = *p;
              ఆపు;
            }
          ఒకవేళ (state == ST_BACKSLASH)
            {
              *(q++) = అంకె;
              ++లెక్క;
              state = ST_GND;
            }
          ++p;
          ఆపు;

        స్థితి ST_OCTAL:		/* Octal sequence */
          ఒకవేళ (*p < '0' || *p > '7')
            {
              *(q++) = అంకె;
              ++లెక్క;
              state = ST_GND;
            }
          లేకపోతే
            అంకె = (అంకె << 3) + (*(p++) - '0');
          ఆపు;

        స్థితి ST_HEX:		/* Hex sequence */
          ఎంపిక (*p)
            {
            స్థితి '0':
            స్థితి '1':
            స్థితి '2':
            స్థితి '3':
            స్థితి '4':
            స్థితి '5':
            స్థితి '6':
            స్థితి '7':
            స్థితి '8':
            స్థితి '9':
              అంకె = (అంకె << 4) + (*(p++) - '0');
              ఆపు;
            స్థితి 'a':
            స్థితి 'b':
            స్థితి 'c':
            స్థితి 'd':
            స్థితి 'e':
            స్థితి 'f':
              అంకె = (అంకె << 4) + (*(p++) - 'a') + 10;
              ఆపు;
            స్థితి 'A':
            స్థితి 'B':
            స్థితి 'C':
            స్థితి 'D':
            స్థితి 'E':
            స్థితి 'F':
              అంకె = (అంకె << 4) + (*(p++) - 'A') + 10;
              ఆపు;
            అప్రమేయ:
              *(q++) = అంకె;
              ++లెక్క;
              state = ST_GND;
              ఆపు;
            }
          ఆపు;

        స్థితి ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          ఒకవేళ (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++లెక్క;
            }
          లేకపోతే ఒకవేళ (*p == '?')
            {
              *(q++) = 127;
              ++లెక్క;
            }
          లేకపోతే
            state = ST_ERROR;
          ఆపు;

        స్థితి ST_చివర: స్థితి ST_ERROR: అప్రమేయ:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_లెక్క = లెక్క;

  తిరిగి state != ST_ERROR;
}

గణన parse_state
  {
    PS_ప్రారంభం = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_పూర్తి,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

స్థిర bool
తెలిసిన_పదం_type (శూన్య)
{
  అక్షరం స్థిరరాశి *పదం = getenv ("TERM");
  ఒకవేళ (! పదం || ! *పదం)
    తిరిగి false;

  అక్షరం స్థిరరాశి *పంక్తి = G_పంక్తి;
  అయితే (పంక్తి - G_పంక్తి < పరిమాణం (G_పంక్తి))
    {
      ఒకవేళ (STRNCMP_LIT (పంక్తి, "TERM ") == 0)
        {
          ఒకవేళ (fnmatch (పంక్తి + 5, పదం, 0) == 0)
            తిరిగి true;
        }
      పంక్తి += అక్షరశ్రేణి_పొడవు (పంక్తి) + 1;
    }

  తిరిగి false;
}

స్థిర శూన్య
parse_ls_color (శూన్య)
{
  అక్షరం స్థిరరాశి *p;		/* Pointer to character being parsed */
  అక్షరం *buf;			/* color_buf buffer pointer */
  అక్షరం label0, label1;		/* Indicator label */
  నిర్మాణం color_ext_type *ext;	/* Extension we are working on */

  ఒకవేళ ((p = getenv ("LS_COLORS")) == ఖాళీ || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      అక్షరం స్థిరరాశి *colorterm = getenv ("COLORTERM");
      ఒకవేళ (! (colorterm && *colorterm) && ! తెలిసిన_పదం_type ())
        print_with_color = false;
      తిరిగి;
    }

  ext = ఖాళీ;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  గణన parse_state state = PS_ప్రారంభం;
  అయితే (true)
    {
      ఎంపిక (state)
        {
        స్థితి PS_ప్రారంభం:		/* First label character */
          ఎంపిక (*p)
            {
            స్థితి ':':
              ++p;
              ఆపు;

            స్థితి '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (పరిమాణం *ext);
              ext->తదుపరి = color_ext_list;
              color_ext_list = ext;
              ext->exact_సరిపోలిక = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              ఆపు;

            స్థితి '\0':
              state = PS_పూర్తి;	/* Done! */
              వెళ్ళు పూర్తి;

            అప్రమేయ:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              ఆపు;
            }
          ఆపు;

        స్థితి PS_2:		/* Second label character */
          ఒకవేళ (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          లేకపోతే
            state = PS_FAIL;	/* Error */
          ఆపు;

        స్థితి PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          ఒకవేళ (*(p++) == '=')/* It *should* be...  */
            {
              కొరకు (పూర్ణాంకం i = 0; i < countof (indicator_name); i++)
                {
                  ఒకవేళ ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_ప్రారంభం : PS_FAIL);
                      ఆపు;
                    }
                }
              ఒకవేళ (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((అక్షరం []) {label0, label1, '\0'}));
            }
          ఆపు;

        స్థితి PS_4:		/* Equal sign after *.ext */
          ఒకవేళ (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_ప్రారంభం : PS_FAIL);
            }
          లేకపోతే
            state = PS_FAIL;
          ఆపు;

        స్థితి PS_FAIL:
          వెళ్ళు పూర్తి;

        స్థితి PS_పూర్తి: అప్రమేయ:
          affirm (false);
        }
    }
 పూర్తి:

  ఒకవేళ (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      విడుదల (color_buf);
      కొరకు (నిర్మాణం color_ext_type *e = color_ext_list; e != ఖాళీ;
           /*ఖాళీలు*/)
        {
          నిర్మాణం color_ext_type *e2 = e;
          e = e->తదుపరి;
          విడుదల (e2);
        }
      print_with_color = false;
      తిరిగి;
    }
  లేకపోతే
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      కొరకు (నిర్మాణం color_ext_type *e1 = color_ext_list; e1 != ఖాళీ;
           e1 = e1->తదుపరి)
        {
          bool case_ignored = false;

          కొరకు (నిర్మాణం color_ext_type *e2 = e1->తదుపరి; e2 != ఖాళీ;
               e2 = e2->తదుపరి)
            {
              ఒకవేళ (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  ఒకవేళ (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  లేకపోతే ఒకవేళ (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      ఒకవేళ (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      లేకపోతే ఒకవేళ (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      లేకపోతే
                        {
                          e1->exact_సరిపోలిక = true;
                          e2->exact_సరిపోలిక = true;
                        }
                    }
                }
            }
        }
    }

  ఒకవేళ (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

స్థిర శూన్య
సమితి_exit_స్థితి (bool serious)
{
  ఒకవేళ (serious)
    exit_స్థితి = LS_FAILURE;
  లేకపోతే ఒకవేళ (exit_స్థితి == EXIT_SUCCESS)
    exit_స్థితి = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

స్థిర శూన్య
file_failure (bool serious, అక్షరం స్థిరరాశి *సందేశం_2, అక్షరం స్థిరరాశి *file)
{
  error (0, errno, సందేశం_2, quoteaf (file));
  సమితి_exit_స్థితి (serious);
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

స్థిర శూన్య
వరుస_directory (అక్షరం స్థిరరాశి *name, అక్షరం స్థిరరాశి *realname, bool command_పంక్తి_arg)
{
  నిర్మాణం pending *new = xmalloc (పరిమాణం *new);
  new->realname = realname ? xstrdup (realname) : ఖాళీ;
  new->name = name ? xstrdup (name) : ఖాళీ;
  new->command_పంక్తి_arg = command_పంక్తి_arg;
  new->తదుపరి = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

స్థిర శూన్య
print_dir (అక్షరం స్థిరరాశి *name, అక్షరం స్థిరరాశి *realname, bool command_పంక్తి_arg)
{
  DIR *dirp;
  నిర్మాణం dirent *తదుపరి;
  uintmax_t మొత్తం_ఖండాలు = 0;
  స్థిర bool మొదటి = true;

  errno = 0;
  dirp = opendir (name);
  ఒకవేళ (!dirp)
    {
      ఒకవేళ (command_పంక్తి_arg || ! ignorable_traversal_errno (errno))
        file_failure (command_పంక్తి_arg, _("cannot open directory %s"), name);
      తిరిగి;
    }

  ఒకవేళ (LOOP_DETECT)
    {
      నిర్మాణం stat dir_stat;
      పూర్ణాంకం fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      ఒకవేళ ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          ఒకవేళ (! ignorable_traversal_errno (errno))
            file_failure (command_పంక్తి_arg,
                          _("cannot determine device and inode of %s"), name);
          closedir (dirp);
          తిరిగి;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      ఒకవేళ (సందర్శించు_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          closedir (dirp);
          సమితి_exit_స్థితి (true);
          తిరిగి;
        }

      dev_ino_నెట్టు (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  ఒకవేళ (recursive || print_dir_name)
    {
      ఒకవేళ (!మొదటి)
        dired_outbyte ('\n');
      మొదటి = false;
      dired_indent ();

      అక్షరం *absolute_name = ఖాళీ;
      ఒకవేళ (print_hyperlink)
        {
          absolute_name = canonicalize_filename_బహుళకం (name, CAN_MISSING);
          ఒకవేళ (! absolute_name
              && ! ignorable_traversal_errno (errno))
            file_failure (command_పంక్తి_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  ఖాళీ, true, &subdired_obstack, absolute_name);

      విడుదల (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  అయితే (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      errno = 0;
      తదుపరి = readdir (dirp);
      ఒకవేళ (తదుపరి)
        {
          ఒకవేళ (! file_ignored (తదుపరి->d_name))
            {
              గణన filetype type;
#ఒకవేళ HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[తదుపరి->d_type];
#లేకపోతే
              type = unknown;
#అయితే_ముగింపు
              మొత్తం_ఖండాలు += gobble_file (తదుపరి->d_name, type,
                                           RELIABLE_D_INO (తదుపరి),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              ఒకవేళ (format == one_చొప్పున_పంక్తి && క్రమీకరించు_type == క్రమీకరించు_none
                      && !print_ఖండం_కొలత && !recursive)
                {
                  /* We must call sort_files in spite of
                     "sort_type == sort_none" for its initialization
                     of the sorted_file vector.  */
                  క్రమీకరించు_files ();
                  print_current_files ();
                  clear_files ();
                }
            }
        }
      లేకపోతే
        {
          పూర్ణాంకం err = errno;
          ఒకవేళ (err == 0)
            ఆపు;
          /* Ignore errors indicating that the directory was removed.  */
          ఒకవేళ (ignorable_traversal_errno (err))
            ఆపు;
          file_failure (command_పంక్తి_arg, _("reading directory %s"), name);
          ఒకవేళ (err != EOVERFLOW)
            ఆపు;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_సంకేతాలు ();
    }

  ఒకవేళ (closedir (dirp) != 0)
    {
      file_failure (command_పంక్తి_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  క్రమీకరించు_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  ఒకవేళ (recursive)
    వెలికితీయి_dirs_from_files (name, false);

  ఒకవేళ (format == long_format || print_ఖండం_కొలత)
    {
      అక్షరం buf[LONGEST_HUMAN_READABLE + 3];
      అక్షరం *p = human_readable (మొత్తం_ఖండాలు, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_ఖండం_కొలత);
      అక్షరం *pend = p + అక్షరశ్రేణి_పొడవు (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  ఒకవేళ (cwd_n_వాడిన)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

స్థిర శూన్య
కూడు_ignore_నమూనా (అక్షరం స్థిరరాశి *నమూనా_2)
{
  నిర్మాణం ignore_నమూనా *ignore;

  ignore = xmalloc (పరిమాణం *ignore);
  ignore->నమూనా_2 = నమూనా_2;
  /* Add it to the head of the linked list.  */
  ignore->తదుపరి = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

స్థిర bool
patterns_సరిపోలిక (నిర్మాణం ignore_నమూనా స్థిరరాశి *patterns, అక్షరం స్థిరరాశి *file)
{
  కొరకు (నిర్మాణం ignore_నమూనా స్థిరరాశి *p = patterns; p; p = p->తదుపరి)
    ఒకవేళ (fnmatch (p->నమూనా_2, file, FNM_PERIOD) == 0)
      తిరిగి true;
  తిరిగి false;
}

/* Return true if FILE should be ignored.  */

స్థిర bool
file_ignored (అక్షరం స్థిరరాశి *name)
{
  తిరిగి ((ignore_బహుళకం != IGNORE_కనీస
           && name[0] == '.'
           && (ignore_బహుళకం == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_బహుళకం == IGNORE_DEFAULT
              && patterns_సరిపోలిక (hide_patterns, name))
          || patterns_సరిపోలిక (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

స్థిర uintmax_t
unsigned_file_కొలత (off_t కొలత)
{
  తిరిగి కొలత + (కొలత < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#నిర్వచితమైతే HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
స్థిర bool
has_capability (అక్షరం స్థిరరాశి *name)
{
  అక్షరం *ఫలితం;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  ఒకవేళ (cap_d == ఖాళీ)
    తిరిగి false;

  ఫలితం = cap_గమ్యం_పాఠ్యం (cap_d, ఖాళీ);
  cap_free (cap_d);
  ఒకవేళ (!ఫలితం)
    తిరిగి false;

  /* check if human-readable capability string is empty */
  has_cap = !!*ఫలితం;

  cap_free (ఫలితం);
  తిరిగి has_cap;
}
#లేకపోతే
స్థిర bool
has_capability (MAYBE_వాడని అక్షరం స్థిరరాశి *name)
{
  errno = ENOTSUP;
  తిరిగి false;
}
#అయితే_ముగింపు

/* Enter and remove entries in the table 'cwd_file'.  */

స్థిర శూన్య
free_ent (నిర్మాణం fileinfo *f)
{
  విడుదల (f->name);
  విడుదల (f->linkname);
  విడుదల (f->absolute_name);
  ఒకవేళ (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
స్థిర శూన్య
clear_files (శూన్య)
{
  కొరకు (idx_t i = 0; i < cwd_n_వాడిన; i++)
    {
      నిర్మాణం fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_వాడిన = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_సంఖ్య_వెడల్పు = 0;
  ఖండం_కొలత_వెడల్పు = 0;
  nlink_వెడల్పు = 0;
  owner_వెడల్పు = 0;
  సమూహం_వెడల్పు = 0;
  author_వెడల్పు = 0;
  scontext_వెడల్పు = 0;
  major_device_సంఖ్య_వెడల్పు = 0;
  minor_device_సంఖ్య_వెడల్పు = 0;
  file_కొలత_వెడల్పు = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
స్థిర పూర్ణాంకం
file_has_aclinfo_cache (అక్షరం స్థిరరాశి *file, నిర్మాణం fileinfo *f,
                        నిర్మాణం aclinfo *ai, పూర్ణాంకం flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  స్థిర పూర్ణాంకం unsupported_return;
  స్థిర అక్షరం *unsupported_scontext;
  స్థిర పూర్ణాంకం unsupported_scontext_err;
  స్థిర dev_t unsupported_device;

  ఒకవేళ (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->కొలత = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      errno = ENOTSUP;
      తిరిగి unsupported_return;
    }

  errno = 0;
  పూర్ణాంకం n = file_has_aclinfo (file, ai, flags);
  పూర్ణాంకం err = errno;
  ఒకవేళ (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  ఒకవేళ (f->stat_ok && n <= 0 && !acl_errno_valid (err)
      && ((!USE_ACL && !format_needs_capability)
          || (ai->కొలత < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  తిరిగి n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
స్థిర bool
has_capability_cache (అక్షరం స్థిరరాశి *file, నిర్మాణం fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  స్థిర bool unsupported_cached;
  స్థిర dev_t unsupported_device;

  ఒకవేళ (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      errno = ENOTSUP;
      తిరిగి 0;
    }

  bool b = has_capability (file);
  ఒకవేళ (f->stat_ok && !b && !acl_errno_valid (errno))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  తిరిగి b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
స్థిర శూన్య
get_extended_info (అక్షరం స్థిరరాశి *నిండిన_name, నిర్మాణం fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  గణన filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#ఒకవేళ HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#అయితే_ముగింపు
  bool check_capability = format_needs_capability & (type == సాధారణ);

  ఒకవేళ (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      నిర్మాణం aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      పూర్ణాంకం aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      పూర్ణాంకం n = file_has_aclinfo_cache (నిండిన_name, f, &ai, aclinfo_flags);
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

      ఒకవేళ (format == long_format && n < 0 && !cannot_access_acl)
        error (0, errno, "%s", quotef (నిండిన_name));
      లేకపోతే
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          ఒకవేళ (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (నిండిన_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      ఒకవేళ (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (నిండిన_name, f);

      f->scontext = ai.scontext;
      ai.scontext = ఖాళీ;
      aclinfo_free (&ai);
    }
}

స్థిర bool
needs_quoting (అక్షరం స్థిరరాశి *name)
{
  అక్షరం test[2];
  size_t len = quotearg_బఫరు (test, పరిమాణం test , name, -1,
                                filename_quoting_options);
  తిరిగి *name != *test || అక్షరశ్రేణి_పొడవు (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
స్థిర uintmax_t
gobble_file (అక్షరం స్థిరరాశి *name, గణన filetype type, ino_t inode,
             bool command_పంక్తి_arg, అక్షరం స్థిరరాశి *dirname)
{
  uintmax_t ఖండాలు = 0;
  నిర్మాణం fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_పంక్తి_arg || inode == NOT_AN_INODE_సంఖ్య);

  ఒకవేళ (cwd_n_వాడిన == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, పరిమాణం *cwd_file);

  f = &cwd_file[cwd_n_వాడిన];
  memset (f, '\0', పరిమాణం *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  ఒకవేళ ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      ఒకవేళ (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_పంక్తి_arg
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
              || color_symlink_as_referent || check_symlink_బహుళకం))
      /* Command line dereferences are already taken care of by the above
         assertion that the inode number is not yet known.  */
      || (print_inode && inode == NOT_AN_INODE_సంఖ్య)
      /* --indicator-style=classify (aka -F) requires statting each
         regular file to see whether it's executable.  */
      || ((type == సాధారణ || type == unknown)
          && (indicator_style == classify
              /* This is so that --color ends up highlighting files with these
                 mode bits set even when options like -F are not specified.  */
              || (print_with_color && (is_colored (C_EXEC)
                                       || is_colored (C_SETUID)
                                       || is_colored (C_SETGID))))));

  /* Absolute name of this file, if needed.  */
  అక్షరం స్థిరరాశి *నిండిన_name = name;
  ఒకవేళ (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && dirname)
    {
      అక్షరం *p = alloca (అక్షరశ్రేణి_పొడవు (name) + అక్షరశ్రేణి_పొడవు (dirname) + 2);
      attach (p, dirname, name);
      నిండిన_name = p;
    }

  bool do_deref;

  ఒకవేళ (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  లేకపోతే
    {
      పూర్ణాంకం err;

      ఒకవేళ (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_బహుళకం (నిండిన_name,
                                                         CAN_MISSING);
          ఒకవేళ (! f->absolute_name
              && (command_పంక్తి_arg
                  || ! ignorable_traversal_errno (errno)))
            file_failure (command_పంక్తి_arg,
                          _("error canonicalizing %s"), నిండిన_name);
        }

      ఎంపిక (dereference)
        {
        స్థితి DEREF_ALWAYS:
          err = do_stat (నిండిన_name, &f->stat);
          do_deref = true;
          ఆపు;

        స్థితి DEREF_COMMAND_పంక్తి_ARGUMENTS:
        స్థితి DEREF_COMMAND_పంక్తి_SYMLINK_గమ్యం_DIR:
          ఒకవేళ (command_పంక్తి_arg)
            {
              bool need_lstat;
              err = do_stat (నిండిన_name, &f->stat);
              do_deref = true;

              ఒకవేళ (dereference == DEREF_COMMAND_పంక్తి_ARGUMENTS)
                ఆపు;

              need_lstat = (err < 0
                            ? (errno == ENOENT || errno == ELOOP)
                            : ! S_ISDIR (f->stat.st_బహుళకం));
              ఒకవేళ (!need_lstat)
                ఆపు;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        స్థితి DEREF_NEVER:
          err = do_lstat (నిండిన_name, &f->stat);
          do_deref = false;
          ఆపు;

        స్థితి DEREF_UNDEFINED: అప్రమేయ:
          affirm (false);
        }

      ఒకవేళ (err != 0)
        {
          ఒకవేళ (! command_పంక్తి_arg && ignorable_traversal_errno (errno))
            {
              పూర్ణాంకం stat_errno = errno;
              నిర్మాణం stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (నిండిన_name, &linkstat) == 0);
              errno = stat_errno;
              ఒకవేళ (! dangling_link)
                {
                  free_ent (f);
                  తిరిగి 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_పంక్తి_arg,
                        _("cannot access %s"), నిండిన_name);

          ఒకవేళ (command_పంక్తి_arg)
            {
              free_ent (f);
              తిరిగి 0;
            }

          f->name = xstrdup (name);
          cwd_n_వాడిన++;

          తిరిగి 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_బహుళకం)];
    }

  ఒకవేళ (type == directory && command_పంక్తి_arg && !తక్షణ_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (నిండిన_name, f, do_deref);

  ఒకవేళ ((type == symbolic_link)
      & ((format == long_format) | check_symlink_బహుళకం))
    {
      నిర్మాణం stat linkstats;

      ఒకవేళ (! get_link_name (నిండిన_name, f, command_పంక్తి_arg))
        {
          free_ent (f);
          తిరిగి 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      ఒకవేళ (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      ఒకవేళ (f->linkname
          && (file_type <= indicator_style || check_symlink_బహుళకం)
          && stat_for_బహుళకం (నిండిన_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_బహుళకం;
        }
    }

  ఖండాలు = STP_NBLOCKS (&f->stat);
  ఒకవేళ (format == long_format || print_ఖండం_కొలత)
    {
      అక్షరం buf[LONGEST_HUMAN_READABLE + 1];
      పూర్ణాంకం len = mbswidth (human_readable (ఖండాలు, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_ఖండం_కొలత),
                          MBSWIDTH_FLAGS);
      ఒకవేళ (ఖండం_కొలత_వెడల్పు < len)
        ఖండం_కొలత_వెడల్పు = len;
    }

  ఒకవేళ (format == long_format)
    {
      ఒకవేళ (print_owner)
        {
          పూర్ణాంకం len = format_user_వెడల్పు (f->stat.st_uid);
          ఒకవేళ (owner_వెడల్పు < len)
            owner_వెడల్పు = len;
        }

      ఒకవేళ (print_సమూహం)
        {
          పూర్ణాంకం len = format_సమూహం_వెడల్పు (f->stat.st_gid);
          ఒకవేళ (సమూహం_వెడల్పు < len)
            సమూహం_వెడల్పు = len;
        }

      ఒకవేళ (print_author)
        {
          పూర్ణాంకం len = format_user_వెడల్పు (f->stat.st_author);
          ఒకవేళ (author_వెడల్పు < len)
            author_వెడల్పు = len;
        }
    }

  ఒకవేళ (print_scontext)
    {
      పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు (f->scontext);
      ఒకవేళ (scontext_వెడల్పు < len)
        scontext_వెడల్పు = len;
    }

  ఒకవేళ (format == long_format)
    {
      అక్షరం b[INT_BUFSIZE_BOUND (uintmax_t)];
      పూర్ణాంకం b_len = అక్షరశ్రేణి_పొడవు (umaxtostr (f->stat.st_nlink, b));
      ఒకవేళ (nlink_వెడల్పు < b_len)
        nlink_వెడల్పు = b_len;

      ఒకవేళ ((type == chardev) | (type == blockdev))
        {
          అక్షరం buf[INT_BUFSIZE_BOUND (uintmax_t)];
          పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు (umaxtostr (major (f->stat.st_rdev), buf));
          ఒకవేళ (major_device_సంఖ్య_వెడల్పు < len)
            major_device_సంఖ్య_వెడల్పు = len;
          len = అక్షరశ్రేణి_పొడవు (umaxtostr (minor (f->stat.st_rdev), buf));
          ఒకవేళ (minor_device_సంఖ్య_వెడల్పు < len)
            minor_device_సంఖ్య_వెడల్పు = len;
          len = major_device_సంఖ్య_వెడల్పు + 2 + minor_device_సంఖ్య_వెడల్పు;
          ఒకవేళ (file_కొలత_వెడల్పు < len)
            file_కొలత_వెడల్పు = len;
        }
      లేకపోతే
        {
          అక్షరం buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t కొలత = unsigned_file_కొలత (f->stat.st_కొలత);
          పూర్ణాంకం len = mbswidth (human_readable (కొలత, buf,
                                              file_human_output_opts,
                                              1, file_output_ఖండం_కొలత),
                              MBSWIDTH_FLAGS);
          ఒకవేళ (file_కొలత_వెడల్పు < len)
            file_కొలత_వెడల్పు = len;
        }
    }

  ఒకవేళ (print_inode)
    {
      అక్షరం buf[INT_BUFSIZE_BOUND (uintmax_t)];
      పూర్ణాంకం len = అక్షరశ్రేణి_పొడవు (umaxtostr (f->stat.st_ino, buf));
      ఒకవేళ (inode_సంఖ్య_వెడల్పు < len)
        inode_సంఖ్య_వెడల్పు = len;
    }

  f->name = xstrdup (name);
  cwd_n_వాడిన++;

  తిరిగి ఖండాలు;
}

/* Return true if F refers to a directory.  */
స్థిర bool
is_directory (స్థిరరాశి నిర్మాణం fileinfo *f)
{
  తిరిగి f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
స్థిర bool
is_linked_directory (స్థిరరాశి నిర్మాణం fileinfo *f)
{
  తిరిగి f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

స్థిర bool
get_link_name (అక్షరం స్థిరరాశి *filename, నిర్మాణం fileinfo *f, bool command_పంక్తి_arg)
{
  f->linkname = areadlink_with_కొలత (filename, f->stat.st_కొలత);
  ఒకవేళ (f->linkname == ఖాళీ)
    {
      ఒకవేళ (! command_పంక్తి_arg
          && ignorable_traversal_errno (errno))
        తిరిగి false;
      file_failure (command_పంక్తి_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  తిరిగి true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

స్థిర bool
basename_is_dot_or_dotdot (అక్షరం స్థిరరాశి *name)
{
  అక్షరం స్థిరరాశి *పునాది_2 = చివరి_component (name);
  తిరిగి dot_or_dotdot (పునాది_2);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

స్థిర శూన్య
వెలికితీయి_dirs_from_files (అక్షరం స్థిరరాశి *dirname, bool command_పంక్తి_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (dirname != ఖాళీ);

  ఒకవేళ (dirname && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      వరుస_directory (ఖాళీ, dirname, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  కొరకు (i = cwd_n_వాడిన; 0 < i; )
    {
      i--;
      నిర్మాణం fileinfo *f = sorted_file[i];

      ఒకవేళ (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          ఒకవేళ (!dirname || f->name[0] == '/')
            వరుస_directory (f->name, f->linkname, command_పంక్తి_arg);
          లేకపోతే
            {
              అక్షరం *name = file_name_concat (dirname, f->name, ఖాళీ);
              వరుస_directory (name, f->linkname, command_పంక్తి_arg);
              విడుదల (name);
            }
          ఒకవేళ (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  కొరకు (i = 0, j = 0; i < cwd_n_వాడిన; i++)
    {
      నిర్మాణం fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_వాడిన = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

స్థిర jmp_buf failed_strcoll;

స్థిర పూర్ణాంకం
xstrcoll (అక్షరం స్థిరరాశి *a, అక్షరం స్థిరరాశి *b)
{
  పూర్ణాంకం diff;
  errno = 0;
  diff = strcoll (a, b);
  ఒకవేళ (errno)
    {
      error (0, errno, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      సమితి_exit_స్థితి (false);
      longjmp (failed_strcoll, 1);
    }
  తిరిగి diff;
}

/* Comparison routines for sorting the files.  */

రకనిర్వచనం శూన్య స్థిరరాశి *V;
రకనిర్వచనం పూర్ణాంకం (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
స్థిర పూర్ణాంకం
dirfirst_check (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
                పూర్ణాంకం (*cmp) (V, V))
{
  పూర్ణాంకం diff = is_linked_directory (b) - is_linked_directory (a);
  తిరిగి diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#నిర్వచించు DEFINE_క్రమీకరించు_FUNCTIONS(కీలకం_name, కీలకం_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  స్థిర పూర్ణాంకం xstrcoll_##key_name (V a, V b)				\
  { తిరిగి కీలకం_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_శుద్ధ స్థిర పూర్ణాంకం strcmp_##key_name (V a, V b)		\
  { తిరిగి కీలకం_cmp_func (a, b, strcmp); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  స్థిర పూర్ణాంకం rev_xstrcoll_##key_name (V a, V b)				\
  { తిరిగి కీలకం_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_శుద్ధ స్థిర పూర్ణాంకం rev_strcmp_##key_name (V a, V b)	\
  { తిరిగి కీలకం_cmp_func (b, a, strcmp); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  స్థిర పూర్ణాంకం xstrcoll_df_##key_name (V a, V b)				\
  { తిరిగి dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_శుద్ధ స్థిర పూర్ణాంకం strcmp_df_##key_name (V a, V b)		\
  { తిరిగి dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  స్థిర పూర్ణాంకం rev_xstrcoll_df_##key_name (V a, V b)			\
  { తిరిగి dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_శుద్ధ స్థిర పూర్ణాంకం rev_strcmp_df_##key_name (V a, V b)	\
  { తిరిగి dirfirst_check (a, b, rev_strcmp_##key_name); }

స్థిర పూర్ణాంకం
cmp_ctime (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
           పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
cmp_mtime (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
           పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
cmp_atime (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
           పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
cmp_btime (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
           పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
off_cmp (off_t a, off_t b)
{
  తిరిగి _GL_CMP (a, b);
}

స్థిర పూర్ణాంకం
cmp_కొలత (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
          పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = off_cmp (b->stat.st_కొలత, a->stat.st_కొలత);
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
cmp_name (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
          పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  తిరిగి cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

స్థిర పూర్ణాంకం
cmp_extension (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
               పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  అక్షరం స్థిరరాశి *base1 = strrchr (a->name, '.');
  అక్షరం స్థిరరాశి *base2 = strrchr (b->name, '.');
  పూర్ణాంకం diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

స్థిర size_t
fileinfo_name_వెడల్పు (నిర్మాణం fileinfo స్థిరరాశి *f)
{
  తిరిగి f->వెడల్పు
         ? f->వెడల్పు
         : quote_name_వెడల్పు (f->name, filename_quoting_options, f->quoted);
}

స్థిర పూర్ణాంకం
cmp_వెడల్పు (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b,
          పూర్ణాంకం (*cmp) (అక్షరం స్థిరరాశి *, అక్షరం స్థిరరాశి *))
{
  పూర్ణాంకం diff = fileinfo_name_వెడల్పు (a) - fileinfo_name_వెడల్పు (b);
  తిరిగి diff ? diff : cmp (a->name, b->name);
}

DEFINE_క్రమీకరించు_FUNCTIONS (ctime, cmp_ctime)
DEFINE_క్రమీకరించు_FUNCTIONS (mtime, cmp_mtime)
DEFINE_క్రమీకరించు_FUNCTIONS (atime, cmp_atime)
DEFINE_క్రమీకరించు_FUNCTIONS (btime, cmp_btime)
DEFINE_క్రమీకరించు_FUNCTIONS (కొలత, cmp_కొలత)
DEFINE_క్రమీకరించు_FUNCTIONS (name, cmp_name)
DEFINE_క్రమీకరించు_FUNCTIONS (extension, cmp_extension)
DEFINE_క్రమీకరించు_FUNCTIONS (వెడల్పు, cmp_వెడల్పు)

/* Compare file versions.
   Unlike the other compare functions, cmp_version does not fail
   because filevercmp and strcmp do not fail; cmp_version uses strcmp
   instead of xstrcoll because filevercmp is locale-independent so
   strcmp is its appropriate secondary.

   All the other sort options need xstrcoll and strcmp variants,
   because they all use xstrcoll (either as the primary or secondary
   sort key), and xstrcoll has the ability to do a longjmp if strcoll fails for
   locale reasons.  */
స్థిర పూర్ణాంకం
cmp_సంస్కరణ (నిర్మాణం fileinfo స్థిరరాశి *a, నిర్మాణం fileinfo స్థిరరాశి *b)
{
  పూర్ణాంకం diff = filevercmp (a->name, b->name);
  తిరిగి diff ? diff : strcmp (a->name, b->name);
}

స్థిర పూర్ణాంకం
xstrcoll_సంస్కరణ (V a, V b)
{
  తిరిగి cmp_సంస్కరణ (a, b);
}
స్థిర పూర్ణాంకం
rev_xstrcoll_సంస్కరణ (V a, V b)
{
  తిరిగి cmp_సంస్కరణ (b, a);
}
స్థిర పూర్ణాంకం
xstrcoll_df_సంస్కరణ (V a, V b)
{
  తిరిగి dirfirst_check (a, b, xstrcoll_సంస్కరణ);
}
స్థిర పూర్ణాంకం
rev_xstrcoll_df_సంస్కరణ (V a, V b)
{
  తిరిగి dirfirst_check (a, b, rev_xstrcoll_సంస్కరణ);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#నిర్వచించు LIST_SORTFUNCTION_VARIANTS(కీలకం_name)                        \
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

స్థిర qsortFunc స్థిరరాశి క్రమీకరించు_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (వెడల్పు),
    LIST_SORTFUNCTION_VARIANTS (కొలత),

    {
      {
        { xstrcoll_సంస్కరణ, xstrcoll_df_సంస్కరణ },
        { rev_xstrcoll_సంస్కరణ, rev_xstrcoll_df_సంస్కరణ },
      },

      /* We use NULL for the strcmp variants of version comparison
         since as explained in cmp_version definition, version comparison
         does not rely on xstrcoll, so it will never longjmp, and never
         need to try the strcmp fallback. */
      {
        { ఖాళీ, ఖాళీ },
        { ఖాళీ, ఖాళీ },
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
static_assert (countof (క్రమీకరించు_functions) == క్రమీకరించు_numtypes - 2 + సమయం_numtypes);

/* Set up SORTED_FILE to point to the in-use entries in CWD_FILE, in order.  */

స్థిర శూన్య
initialize_ordering_సదిశ (శూన్య)
{
  కొరకు (idx_t i = 0; i < cwd_n_వాడిన; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

స్థిర శూన్య
update_current_files_info (శూన్య)
{
  /* Cache screen width of name, if needed multiple times.  */
  ఒకవేళ (క్రమీకరించు_type == క్రమీకరించు_వెడల్పు
      || (పంక్తి_నిడివి && (format == many_చొప్పున_పంక్తి || format == horizontal)))
    {
      కొరకు (idx_t i = 0; i < cwd_n_వాడిన; i++)
        {
          నిర్మాణం fileinfo *f = sorted_file[i];
          f->వెడల్పు = fileinfo_name_వెడల్పు (f);
        }
    }
}

/* Sort the files now in the table.  */

స్థిర శూన్య
క్రమీకరించు_files (శూన్య)
{
  bool use_strcmp;

  ఒకవేళ (sorted_file_alloc < cwd_n_వాడిన + (cwd_n_వాడిన >> 1))
    {
      విడుదల (sorted_file);
      sorted_file = xinmalloc (cwd_n_వాడిన, 3 * పరిమాణం *sorted_file);
      sorted_file_alloc = 3 * cwd_n_వాడిన;
    }

  initialize_ordering_సదిశ ();

  update_current_files_info ();

  ఒకవేళ (క్రమీకరించు_type == క్రమీకరించు_none)
    తిరిగి;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  ఒకవేళ (! setjmp (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  లేకపోతే
    {
      use_strcmp = true;
      affirm (క్రమీకరించు_type != క్రమీకరించు_సంస్కరణ);
      initialize_ordering_సదిశ ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((శూన్య స్థిరరాశి **) sorted_file, cwd_n_వాడిన,
          క్రమీకరించు_functions[క్రమీకరించు_type + (క్రమీకరించు_type == క్రమీకరించు_సమయం ? సమయం_type : 0)]
                        [use_strcmp][క్రమీకరించు_తిరగేయి]
                        [directories_మొదటి]);
}

/* List all the files now in the table.  */

స్థిర శూన్య
print_current_files (శూన్య)
{
  ఎంపిక (format)
    {
    స్థితి one_చొప్పున_పంక్తి:
      కొరకు (idx_t i = 0; i < cwd_n_వాడిన; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          అక్షరం_రాయి (eolbyte);
        }
      ఆపు;

    స్థితి many_చొప్పున_పంక్తి:
      ఒకవేళ (! పంక్తి_నిడివి)
        print_with_separator (' ');
      లేకపోతే
        print_many_చొప్పున_పంక్తి ();
      ఆపు;

    స్థితి horizontal:
      ఒకవేళ (! పంక్తి_నిడివి)
        print_with_separator (' ');
      లేకపోతే
        print_horizontal ();
      ఆపు;

    స్థితి with_commas:
      print_with_separator (',');
      ఆపు;

    స్థితి long_format:
      కొరకు (idx_t i = 0; i < cwd_n_వాడిన; i++)
        {
          సమితి_సాధారణ_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      ఆపు;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

స్థిర ptrdiff_t
align_nstrftime (అక్షరం *buf, size_t కొలత, bool recent, నిర్మాణం tm స్థిరరాశి *tm,
                 timezone_t tz, పూర్ణాంకం ns)
{
  అక్షరం స్థిరరాశి *nfmt = (use_abformat
                      ? abformat[recent][tm->tm_mon]
                      : long_సమయం_format[recent]);
  తిరిగి nstrftime (buf, కొలత, nfmt, tm, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

స్థిర పూర్ణాంకం
long_సమయం_expected_వెడల్పు (శూన్య)
{
  స్థిర పూర్ణాంకం వెడల్పు = -1;

  ఒకవేళ (వెడల్పు < 0)
    {
      time_t epoch = 0;
      నిర్మాణం tm tm;
      అక్షరం buf[సమయం_STAMP_LEN_గరిష్ఠ + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      ఒకవేళ (localtime_rz (localtz, &epoch, &tm))
        {
          ptrdiff_t len = align_nstrftime (buf, పరిమాణం buf, false,
                                           &tm, localtz, 0);
          ఒకవేళ (len > 0)
            వెడల్పు = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      ఒకవేళ (వెడల్పు < 0)
        వెడల్పు = 0;
    }

  తిరిగి వెడల్పు;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

స్థిర శూన్య
format_user_or_సమూహం (అక్షరం స్థిరరాశి *name, uintmax_t id, పూర్ణాంకం వెడల్పు)
{
  ఒకవేళ (name)
    {
      పూర్ణాంకం name_వెడల్పు = mbswidth (name, MBSWIDTH_FLAGS);
      పూర్ణాంకం వెడల్పు_gap = name_వెడల్పు < 0 ? 0 : వెడల్పు - name_వెడల్పు;
      పూర్ణాంకం pad = MAX (0, వెడల్పు_gap);
      dired_outstring (name);

      చేయి
        dired_outbyte (' ');
      అయితే (pad--);
    }
  లేకపోతే
    dired_pos += రాయి ("%*ju ", వెడల్పు, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

స్థిర శూన్య
format_user (uid_t u, పూర్ణాంకం వెడల్పు, bool stat_ok)
{
  format_user_or_సమూహం (! stat_ok ? "?" :
                        (numeric_ids ? ఖాళీ : getuser (u)), u, వెడల్పు);
}

/* Likewise, for groups.  */

స్థిర శూన్య
format_సమూహం (gid_t g, పూర్ణాంకం వెడల్పు, bool stat_ok)
{
  format_user_or_సమూహం (! stat_ok ? "?" :
                        (numeric_ids ? ఖాళీ : getgroup (g)), g, వెడల్పు);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

స్థిర పూర్ణాంకం
format_user_or_సమూహం_వెడల్పు (అక్షరం స్థిరరాశి *name, uintmax_t id)
{
  తిరిగి (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (ఖాళీ, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

స్థిర పూర్ణాంకం
format_user_వెడల్పు (uid_t u)
{
  తిరిగి format_user_or_సమూహం_వెడల్పు (numeric_ids ? ఖాళీ : getuser (u), u);
}

/* Likewise, for groups.  */

స్థిర పూర్ణాంకం
format_సమూహం_వెడల్పు (gid_t g)
{
  తిరిగి format_user_or_సమూహం_వెడల్పు (numeric_ids ? ఖాళీ : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
స్థిర అక్షరం *
format_inode (అక్షరం buf[INT_BUFSIZE_BOUND (uintmax_t)],
              స్థిరరాశి నిర్మాణం fileinfo *f)
{
  తిరిగి (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_సంఖ్య
          ? umaxtostr (f->stat.st_ino, buf)
          : (అక్షరం *) "?");
}

/* Print information about F in long format.  */
స్థిర శూన్య
print_long_format (స్థిరరాశి నిర్మాణం fileinfo *f)
{
  అక్షరం modebuf[12];
  అక్షరం buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + పరిమాణం (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + సమయం_STAMP_LEN_గరిష్ఠ + 1	/* max length of time/date */
     ];
  ptrdiff_t s;
  అక్షరం *p;
  నిర్మాణం timespec when_timespec;
  నిర్మాణం tm when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  ఒకవేళ (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  లేకపోతే
    {
      modebuf[0] = filetype_letter[f->filetype];
      memset (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  ఒకవేళ (! any_has_acl)
    modebuf[10] = '\0';
  లేకపోతే ఒకవేళ (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  లేకపోతే ఒకవేళ (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  లేకపోతే ఒకవేళ (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  ఎంపిక (సమయం_type)
    {
    స్థితి సమయం_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      ఆపు;
    స్థితి సమయం_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      ఆపు;
    స్థితి సమయం_atime:
      when_timespec = get_stat_atime (&f->stat);
      ఆపు;
    స్థితి సమయం_btime:
      when_timespec = get_stat_btime (&f->stat);
      ఒకవేళ (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      ఆపు;
    స్థితి సమయం_numtypes: అప్రమేయ:
      unreachable ();
    }

  p = buf;

  ఒకవేళ (print_inode)
    {
      అక్షరం hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += sprintf (p, "%*s ", inode_సంఖ్య_వెడల్పు, format_inode (hbuf, f));
    }

  ఒకవేళ (print_ఖండం_కొలత)
    {
      అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
      అక్షరం స్థిరరాశి *ఖండాలు =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_ఖండం_కొలత));
      పూర్ణాంకం ఖండాలు_వెడల్పు = mbswidth (ఖండాలు, MBSWIDTH_FLAGS);
      కొరకు (పూర్ణాంకం pad = ఖండాలు_వెడల్పు < 0 ? 0 : ఖండం_కొలత_వెడల్పు - ఖండాలు_వెడల్పు;
           0 < pad; pad--)
        *p++ = ' ';
      అయితే ((*p++ = *ఖండాలు++))
        కొనసాగు;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    అక్షరం hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += sprintf (p, "%s %*s ", modebuf, nlink_వెడల్పు,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  ఒకవేళ (print_owner || print_సమూహం || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      ఒకవేళ (print_owner)
        format_user (f->stat.st_uid, owner_వెడల్పు, f->stat_ok);

      ఒకవేళ (print_సమూహం)
        format_సమూహం (f->stat.st_gid, సమూహం_వెడల్పు, f->stat_ok);

      ఒకవేళ (print_author)
        format_user (f->stat.st_author, author_వెడల్పు, f->stat_ok);

      ఒకవేళ (print_scontext)
        format_user_or_సమూహం (f->scontext, 0, scontext_వెడల్పు);

      p = buf;
    }

  ఒకవేళ (f->stat_ok
      && (S_ISCHR (f->stat.st_బహుళకం) || S_ISBLK (f->stat.st_బహుళకం)))
    {
      పూర్ణాంకం blanks_వెడల్పు = (file_కొలత_వెడల్పు
                          - (major_device_సంఖ్య_వెడల్పు + 2
                             + minor_device_సంఖ్య_వెడల్పు));
      p += sprintf (p, "%*ju, %*ju ",
                    major_device_సంఖ్య_వెడల్పు + MAX (0, blanks_వెడల్పు),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_సంఖ్య_వెడల్పు,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  లేకపోతే
    {
      అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
      అక్షరం స్థిరరాశి *కొలత =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_కొలత (f->stat.st_కొలత),
                           hbuf, file_human_output_opts, 1,
                           file_output_ఖండం_కొలత));
      పూర్ణాంకం కొలత_వెడల్పు = mbswidth (కొలత, MBSWIDTH_FLAGS);
      కొరకు (పూర్ణాంకం pad = కొలత_వెడల్పు < 0 ? 0 : file_కొలత_వెడల్పు - కొలత_వెడల్పు;
           0 < pad; pad--)
        *p++ = ' ';
      అయితే ((*p++ = *కొలత++))
        కొనసాగు;
      p[-1] = ' ';
    }

  s = -1;

  ఒకవేళ (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      నిర్మాణం timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      ఒకవేళ (timespec_cmp (current_సమయం, when_timespec) < 0)
        gettime (&current_సమయం);

      /* Consider a time to be recent if it is within the past six months.
         A Gregorian year has 365.2425 * 24 * 60 * 60 == 31556952 seconds
         on the average.  Write this value as an integer constant to
         avoid floating point hassles.  */
      six_months_ago.tv_sec = current_సమయం.tv_sec - 31556952 / 2;
      six_months_ago.tv_nsec = current_సమయం.tv_nsec;

      recent = (timespec_cmp (six_months_ago, when_timespec) < 0
                && timespec_cmp (when_timespec, current_సమయం) < 0);

      /* We assume here that all time zones are offset from UTC by a
         whole number of seconds.  */
      s = align_nstrftime (p, సమయం_STAMP_LEN_గరిష్ఠ + 1, recent,
                           &when_local, localtz, when_timespec.tv_nsec);
    }

  ఒకవేళ (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  లేకపోతే
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      అక్షరం hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += sprintf (p, "%*s ", long_సమయం_expected_వెడల్పు (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  size_t w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  ఒకవేళ (f->filetype == symbolic_link)
    {
      ఒకవేళ (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, ఖాళీ, (p - buf) + w + 4);
          ఒకవేళ (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  లేకపోతే ఒకవేళ (indicator_style != none)
    print_type_indicator (f->stat_ok, f->stat.st_బహుళకం, f->filetype);
}

/* Write to *BUF a quoted representation of the file name NAME, if non-null,
   using OPTIONS to control quoting.  *BUF is set to NAME if no quoting
   is required.  *BUF is allocated if more space required (and the original
   *BUF is not deallocated).
   Store the number of screen columns occupied by NAME's quoted
   representation into WIDTH, if non-null.
   Store into PAD whether an initial space is needed for padding.
   Return the number of bytes in *BUF.  */

స్థిర size_t
quote_name_buf (అక్షరం **inbuf, size_t bufsize, అక్షరం *name,
                నిర్మాణం quoting_options స్థిరరాశి *options,
                పూర్ణాంకం needs_general_quoting, size_t *వెడల్పు, bool *pad)
{
  అక్షరం *buf = *inbuf;
  size_t displayed_వెడల్పు IF_LINT ( = 0);
  size_t len = 0;
  bool quoted;

  గణన quoting_style qs = get_quoting_style (options);
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

  ఒకవేళ (needs_general_quoting != 0)
    {
      len = quotearg_బఫరు (buf, bufsize, name, -1, options);
      ఒకవేళ (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_బఫరు (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || అక్షరశ్రేణి_పొడవు (name) != len;
    }
  లేకపోతే ఒకవేళ (needs_further_quoting || extra_newline_protection)
    {
      len = అక్షరశ్రేణి_పొడవు (name);
      ఒకవేళ (bufsize <= len)
        buf = xmalloc (len + 1);
      జ్ఞాపకం_నకలు (buf, name, len + 1);

      quoted = false;
    }
  లేకపోతే
    {
      len = అక్షరశ్రేణి_పొడవు (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  ఒకవేళ (extra_newline_protection)
    కొరకు (అక్షరం *p = buf; p < buf + len; p++)
      ఒకవేళ (eolbyte && *p == '\n')
        *p = '?';

  ఒకవేళ (needs_further_quoting)
    {
      ఒకవేళ (MB_CUR_MAX > 1)
        {
          అక్షరం స్థిరరాశి *p = buf;
          అక్షరం స్థిరరాశి *plimit = buf + len;
          అక్షరం *q = buf;
          displayed_వెడల్పు = 0;

          అయితే (p < plimit)
            ఎంపిక (*p)
              {
                స్థితి ' ': స్థితి '!': స్థితి '"': స్థితి '#': స్థితి '%':
                స్థితి '&': స్థితి '\'': స్థితి '(': స్థితి ')': స్థితి '*':
                స్థితి '+': స్థితి ',': స్థితి '-': స్థితి '.': స్థితి '/':
                స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3': స్థితి '4':
                స్థితి '5': స్థితి '6': స్థితి '7': స్థితి '8': స్థితి '9':
                స్థితి ':': స్థితి ';': స్థితి '<': స్థితి '=': స్థితి '>':
                స్థితి '?':
                స్థితి 'A': స్థితి 'B': స్థితి 'C': స్థితి 'D': స్థితి 'E':
                స్థితి 'F': స్థితి 'G': స్థితి 'H': స్థితి 'I': స్థితి 'J':
                స్థితి 'K': స్థితి 'L': స్థితి 'M': స్థితి 'N': స్థితి 'O':
                స్థితి 'P': స్థితి 'Q': స్థితి 'R': స్థితి 'S': స్థితి 'T':
                స్థితి 'U': స్థితి 'V': స్థితి 'W': స్థితి 'X': స్థితి 'Y':
                స్థితి 'Z':
                స్థితి '[': స్థితి '\\': స్థితి ']': స్థితి '^': స్థితి '_':
                స్థితి 'a': స్థితి 'b': స్థితి 'c': స్థితి 'd': స్థితి 'e':
                స్థితి 'f': స్థితి 'g': స్థితి 'h': స్థితి 'i': స్థితి 'j':
                స్థితి 'k': స్థితి 'l': స్థితి 'm': స్థితి 'n': స్థితి 'o':
                స్థితి 'p': స్థితి 'q': స్థితి 'r': స్థితి 's': స్థితి 't':
                స్థితి 'u': స్థితి 'v': స్థితి 'w': స్థితి 'x': స్థితి 'y':
                స్థితి 'z': స్థితి '{': స్థితి '|': స్థితి '}': స్థితి '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_వెడల్పు += 1;
                  ఆపు;
                అప్రమేయ:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    చేయి
                      {
                        char32_t wc;
                        size_t బైట్లు;
                        పూర్ణాంకం w;

                        బైట్లు = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        ఒకవేళ (బైట్లు == (size_t) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_వెడల్పు += 1;
                            ఆపు;
                          }

                        ఒకవేళ (బైట్లు == (size_t) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_వెడల్పు += 1;
                            ఆపు;
                          }

                        ఒకవేళ (బైట్లు == 0)
                          /* A null wide character was encountered.  */
                          బైట్లు = 1;

                        w = c32width (wc);
                        ఒకవేళ (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            కొరకు (; బైట్లు > 0; --బైట్లు)
                              *q++ = *p++;
                            displayed_వెడల్పు += w;
                          }
                        లేకపోతే
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += బైట్లు;
                            *q++ = '?';
                            displayed_వెడల్పు += 1;
                          }
                      }
                    అయితే (! mbsinit (&mbstate));
                  }
                  ఆపు;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      లేకపోతే
        {
          అక్షరం *p = buf;
          అక్షరం స్థిరరాశి *plimit = buf + len;

          అయితే (p < plimit)
            {
              ఒకవేళ (! isprint (గమ్యం_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_వెడల్పు = len;
        }
    }
  లేకపోతే ఒకవేళ (వెడల్పు != ఖాళీ)
    {
      ఒకవేళ (MB_CUR_MAX > 1)
        {
          displayed_వెడల్పు = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_వెడల్పు = MAX (0, displayed_వెడల్పు);
        }
      లేకపోతే
        {
          అక్షరం స్థిరరాశి *p = buf;
          అక్షరం స్థిరరాశి *plimit = buf + len;

          displayed_వెడల్పు = 0;
          అయితే (p < plimit)
            {
              ఒకవేళ (isprint (గమ్యం_uchar (*p)))
                displayed_వెడల్పు++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  ఒకవేళ (వెడల్పు != ఖాళీ)
    *వెడల్పు = displayed_వెడల్పు;

  *inbuf = buf;

  తిరిగి len;
}

స్థిర size_t
quote_name_వెడల్పు (అక్షరం స్థిరరాశి *name, నిర్మాణం quoting_options స్థిరరాశి *options,
                  పూర్ణాంకం needs_general_quoting)
{
  అక్షరం smallbuf[BUFSIZ];
  అక్షరం *buf = smallbuf;
  size_t వెడల్పు;
  bool pad;

  quote_name_buf (&buf, పరిమాణం smallbuf, (అక్షరం *) name, options,
                  needs_general_quoting, &వెడల్పు, &pad);

  ఒకవేళ (buf != smallbuf && buf != name)
    విడుదల (buf);

  వెడల్పు += pad;

  తిరిగి వెడల్పు;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
స్థిర అక్షరం *
file_escape (అక్షరం స్థిరరాశి *str, bool మార్గం)
{
  అక్షరం *esc = xnmalloc (3, అక్షరశ్రేణి_పొడవు (str) + 1);
  అక్షరం *p = esc;
  అయితే (*str)
    {
      ఒకవేళ (మార్గం && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      లేకపోతే ఒకవేళ (RFC3986[గమ్యం_uchar (*str)])
        *p++ = *str++;
      లేకపోతే
        p += sprintf (p, "%%%02x", గమ్యం_uchar (*str++));
    }
  *p = '\0';
  తిరిగి esc;
}

స్థిర size_t
quote_name (అక్షరం స్థిరరాశి *name, నిర్మాణం quoting_options స్థిరరాశి *options,
            పూర్ణాంకం needs_general_quoting, స్థిరరాశి నిర్మాణం bin_str *color,
            bool allow_pad, నిర్మాణం obstack *దొంతర, అక్షరం స్థిరరాశి *absolute_name)
{
  అక్షరం smallbuf[BUFSIZ];
  అక్షరం *buf = smallbuf;
  size_t len;
  bool pad;

  len = quote_name_buf (&buf, పరిమాణం smallbuf, (అక్షరం *) name, options,
                        needs_general_quoting, ఖాళీ, &pad);

  ఒకవేళ (pad && allow_pad)
    dired_outbyte (' ');

  ఒకవేళ (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  ఒకవేళ (absolute_name)
    {
      ఒకవేళ (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          అక్షరం_రాయి (*buf);
        }
      అక్షరం *h = file_escape (hostname, /* path= */ false);
      అక్షరం *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      రాయి ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      విడుదల (h);
      విడుదల (n);
    }

  ఒకవేళ (దొంతర)
    నెట్టు_current_dired_pos (దొంతర);

  fwrite (buf + skip_quotes, 1, len - (skip_quotes * 2), stdout);

  dired_pos += len;

  ఒకవేళ (దొంతర)
    నెట్టు_current_dired_pos (దొంతర);

  ఒకవేళ (absolute_name)
    {
      fputs ("\033]8;;\033\\", stdout);
      ఒకవేళ (skip_quotes)
        అక్షరం_రాయి (*(buf + len - 1));
    }

  ఒకవేళ (buf != smallbuf && buf != name)
    విడుదల (buf);

  తిరిగి len + pad;
}

స్థిర size_t
print_name_with_quoting (స్థిరరాశి నిర్మాణం fileinfo *f,
                         bool symlink_లక్ష్యం,
                         నిర్మాణం obstack *దొంతర,
                         size_t ప్రారంభం_నిలువు)
{
  అక్షరం స్థిరరాశి *name = symlink_లక్ష్యం ? f->linkname : f->name;

  స్థిరరాశి నిర్మాణం bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_లక్ష్యం) : ఖాళీ;

  bool వాడిన_color_this_సమయం = (print_with_color
                               && (color || is_colored (C_NORM)));

  size_t len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_లక్ష్యం, దొంతర, f->absolute_name);

  process_సంకేతాలు ();
  ఒకవేళ (వాడిన_color_this_సమయం)
    {
      prep_non_filename_పాఠ్యం ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      ఒకవేళ (పంక్తి_నిడివి
          && (ప్రారంభం_నిలువు / పంక్తి_నిడివి != (ప్రారంభం_నిలువు + len - 1) / పంక్తి_నిడివి))
        put_indicator (&color_indicator[C_CLR_గమ్యం_EOL]);
    }

  తిరిగి len;
}

స్థిర శూన్య
prep_non_filename_పాఠ్యం (శూన్య)
{
  ఒకవేళ (color_indicator[C_చివర].string != ఖాళీ)
    put_indicator (&color_indicator[C_చివర]);
  లేకపోతే
    {
      put_indicator (&color_indicator[C_ఎడమ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_కుడి]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

స్థిర size_t
print_file_name_and_frills (స్థిరరాశి నిర్మాణం fileinfo *f, size_t ప్రారంభం_నిలువు)
{
  అక్షరం buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  సమితి_సాధారణ_color ();

  ఒకవేళ (print_inode)
    రాయి ("%*s ", format == with_commas ? 0 : inode_సంఖ్య_వెడల్పు,
            format_inode (buf, f));

  ఒకవేళ (print_ఖండం_కొలత)
    {
      అక్షరం స్థిరరాశి *ఖండాలు =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_ఖండం_కొలత));
      పూర్ణాంకం ఖండాలు_వెడల్పు = mbswidth (ఖండాలు, MBSWIDTH_FLAGS);
      పూర్ణాంకం pad = 0;
      ఒకవేళ (0 <= ఖండాలు_వెడల్పు && ఖండం_కొలత_వెడల్పు && format != with_commas)
        pad = ఖండం_కొలత_వెడల్పు - ఖండాలు_వెడల్పు;
      రాయి ("%*s%s ", pad, "", ఖండాలు);
    }

  ఒకవేళ (print_scontext)
    రాయి ("%*s ", format == with_commas ? 0 : scontext_వెడల్పు, f->scontext);

  size_t వెడల్పు = print_name_with_quoting (f, false, ఖాళీ, ప్రారంభం_నిలువు);

  ఒకవేళ (indicator_style != none)
    వెడల్పు += print_type_indicator (f->stat_ok, f->stat.st_బహుళకం, f->filetype);

  తిరిగి వెడల్పు;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
స్థిర అక్షరం
get_type_indicator (bool stat_ok, mode_t బహుళకం_2, గణన filetype type)
{
  అక్షరం c;

  ఒకవేళ (stat_ok ? S_ISREG (బహుళకం_2) : type == సాధారణ)
    {
      ఒకవేళ (stat_ok && indicator_style == classify && (బహుళకం_2 & S_IXUGO))
        c = '*';
      లేకపోతే
        c = 0;
    }
  లేకపోతే
    {
      ఒకవేళ (stat_ok ? S_ISDIR (బహుళకం_2) : type == directory || type == arg_directory)
        c = '/';
      లేకపోతే ఒకవేళ (indicator_style == slash)
        c = 0;
      లేకపోతే ఒకవేళ (stat_ok ? S_ISLNK (బహుళకం_2) : type == symbolic_link)
        c = '@';
      లేకపోతే ఒకవేళ (stat_ok ? S_ISFIFO (బహుళకం_2) : type == fifo)
        c = '|';
      లేకపోతే ఒకవేళ (stat_ok ? S_ISSOCK (బహుళకం_2) : type == sock)
        c = '=';
      లేకపోతే ఒకవేళ (stat_ok && S_ISDOOR (బహుళకం_2))
        c = '>';
      లేకపోతే
        c = 0;
    }
  తిరిగి c;
}

స్థిర bool
print_type_indicator (bool stat_ok, mode_t బహుళకం_2, గణన filetype type)
{
  అక్షరం c = get_type_indicator (stat_ok, బహుళకం_2, type);
  ఒకవేళ (c)
    dired_outbyte (c);
  తిరిగి !!c;
}

/* Returns if color sequence was printed.  */
స్థిర bool
print_color_indicator (స్థిరరాశి నిర్మాణం bin_str *ind)
{
  ఒకవేళ (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      ఒకవేళ (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_ఎడమ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_కుడి]);
    }

  తిరిగి ind != ఖాళీ;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_శుద్ధ
స్థిర స్థిరరాశి నిర్మాణం bin_str *
get_color_indicator (స్థిరరాశి నిర్మాణం fileinfo *f, bool symlink_లక్ష్యం)
{
  గణన indicator_no type;
  నిర్మాణం color_ext_type *ext;	/* Color extension */
  size_t len;			/* Length of name */

  అక్షరం స్థిరరాశి *name;
  mode_t బహుళకం_2;
  పూర్ణాంకం linkok;
  ఒకవేళ (symlink_లక్ష్యం)
    {
      name = f->linkname;
      బహుళకం_2 = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  లేకపోతే
    {
      name = f->name;
      బహుళకం_2 = file_or_link_బహుళకం (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  ఒకవేళ (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  లేకపోతే ఒకవేళ (!f->stat_ok)
    {
      స్థిర గణన indicator_no స్థిరరాశి filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  లేకపోతే
    {
      ఒకవేళ (S_ISREG (బహుళకం_2))
        {
          type = C_FILE;

          ఒకవేళ ((బహుళకం_2 & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          లేకపోతే ఒకవేళ ((బహుళకం_2 & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          లేకపోతే ఒకవేళ (f->has_capability)
            type = C_CAP;
          లేకపోతే ఒకవేళ ((బహుళకం_2 & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          లేకపోతే ఒకవేళ ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      లేకపోతే ఒకవేళ (S_ISDIR (బహుళకం_2))
        {
          type = C_DIR;

          ఒకవేళ ((బహుళకం_2 & S_ISVTX) && (బహుళకం_2 & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          లేకపోతే ఒకవేళ ((బహుళకం_2 & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          లేకపోతే ఒకవేళ ((బహుళకం_2 & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      లేకపోతే ఒకవేళ (S_ISLNK (బహుళకం_2))
        type = C_LINK;
      లేకపోతే ఒకవేళ (S_ISFIFO (బహుళకం_2))
        type = C_FIFO;
      లేకపోతే ఒకవేళ (S_ISSOCK (బహుళకం_2))
        type = C_SOCK;
      లేకపోతే ఒకవేళ (S_ISBLK (బహుళకం_2))
        type = C_BLK;
      లేకపోతే ఒకవేళ (S_ISCHR (బహుళకం_2))
        type = C_CHR;
      లేకపోతే ఒకవేళ (S_ISDOOR (బహుళకం_2))
        type = C_DOOR;
      లేకపోతే
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = ఖాళీ;
  ఒకవేళ (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = అక్షరశ్రేణి_పొడవు (name);
      name += len;		/* Pointer to final \0.  */
      కొరకు (ext = color_ext_list; ext != ఖాళీ; ext = ext->తదుపరి)
        {
          ఒకవేళ (ext->ext.len <= len)
            {
              ఒకవేళ (ext->exact_సరిపోలిక)
                {
                  ఒకవేళ (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    ఆపు;
                }
              లేకపోతే
                {
                  ఒకవేళ (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    ఆపు;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  ఒకవేళ (type == C_LINK && !linkok)
    {
      ఒకవేళ (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  స్థిరరాశి నిర్మాణం bin_str *స్థిరరాశి s
    = ext ? &(ext->seq) : &color_indicator[type];

  తిరిగి s->string ? s : ఖాళీ;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
స్థిర శూన్య
put_indicator (స్థిరరాశి నిర్మాణం bin_str *ind)
{
  ఒకవేళ (! వాడిన_color)
    {
      వాడిన_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      ఒకవేళ (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_పాఠ్యం ();
    }

  fwrite (ind->string, ind->len, 1, stdout);
}

స్థిర size_t
నిడివి_యొక్క_file_name_and_frills (స్థిరరాశి నిర్మాణం fileinfo *f)
{
  size_t len = 0;
  అక్షరం buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  ఒకవేళ (print_inode)
    len += 1 + (format == with_commas
                ? అక్షరశ్రేణి_పొడవు (umaxtostr (f->stat.st_ino, buf))
                : inode_సంఖ్య_వెడల్పు);

  ఒకవేళ (print_ఖండం_కొలత)
    len += 1 + (format == with_commas
                ? అక్షరశ్రేణి_పొడవు (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_ఖండం_కొలత))
                : ఖండం_కొలత_వెడల్పు);

  ఒకవేళ (print_scontext)
    len += 1 + (format == with_commas ? అక్షరశ్రేణి_పొడవు (f->scontext) : scontext_వెడల్పు);

  len += fileinfo_name_వెడల్పు (f);

  ఒకవేళ (indicator_style != none)
    {
      అక్షరం c = get_type_indicator (f->stat_ok, f->stat.st_బహుళకం, f->filetype);
      len += (c != 0);
    }

  తిరిగి len;
}

స్థిర శూన్య
print_many_చొప్పున_పంక్తి (శూన్య)
{
  idx_t నిలువుల_సంఖ్య_2 = calculate_columns (true);
  నిర్మాణం స్తంభం_info స్థిరరాశి *పంక్తి_fmt = &స్తంభం_info[నిలువుల_సంఖ్య_2 - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t అడ్డువరుసలు = cwd_n_వాడిన / నిలువుల_సంఖ్య_2 + (cwd_n_వాడిన % నిలువుల_సంఖ్య_2 != 0);

  కొరకు (idx_t అడ్డువరుస = 0; అడ్డువరుస < అడ్డువరుసలు; అడ్డువరుస++)
    {
      size_t నిలువు = 0;
      idx_t filesno = అడ్డువరుస;
      size_t pos = 0;

      /* Print the next row.  */
      అయితే (true)
        {
          నిర్మాణం fileinfo స్థిరరాశి *f = sorted_file[filesno];
          size_t name_నిడివి = నిడివి_యొక్క_file_name_and_frills (f);
          size_t max_name_నిడివి = పంక్తి_fmt->నిలువు_arr[నిలువు++];
          print_file_name_and_frills (f, pos);

          ఒకవేళ (cwd_n_వాడిన - అడ్డువరుసలు <= filesno)
            ఆపు;
          filesno += అడ్డువరుసలు;

          indent (pos + name_నిడివి, pos + max_name_నిడివి);
          pos += max_name_నిడివి;
        }
      అక్షరం_రాయి (eolbyte);
    }
}

స్థిర శూన్య
print_horizontal (శూన్య)
{
  size_t pos = 0;
  idx_t నిలువుల_సంఖ్య_2 = calculate_columns (false);
  నిర్మాణం స్తంభం_info స్థిరరాశి *పంక్తి_fmt = &స్తంభం_info[నిలువుల_సంఖ్య_2 - 1];
  నిర్మాణం fileinfo స్థిరరాశి *f = sorted_file[0];
  size_t name_నిడివి = నిడివి_యొక్క_file_name_and_frills (f);
  size_t max_name_నిడివి = పంక్తి_fmt->నిలువు_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  కొరకు (idx_t filesno = 1; filesno < cwd_n_వాడిన; filesno++)
    {
      idx_t నిలువు = filesno % నిలువుల_సంఖ్య_2;

      ఒకవేళ (నిలువు == 0)
        {
          అక్షరం_రాయి (eolbyte);
          pos = 0;
        }
      లేకపోతే
        {
          indent (pos + name_నిడివి, pos + max_name_నిడివి);
          pos += max_name_నిడివి;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_నిడివి = నిడివి_యొక్క_file_name_and_frills (f);
      max_name_నిడివి = పంక్తి_fmt->నిలువు_arr[నిలువు];
    }
  అక్షరం_రాయి (eolbyte);
}

/* Output name + SEP + ' '.  */

స్థిర శూన్య
print_with_separator (అక్షరం sep)
{
  size_t pos = 0;

  కొరకు (idx_t filesno = 0; filesno < cwd_n_వాడిన; filesno++)
    {
      నిర్మాణం fileinfo స్థిరరాశి *f = sorted_file[filesno];
      size_t len = పంక్తి_నిడివి ? నిడివి_యొక్క_file_name_and_frills (f) : 0;

      ఒకవేళ (filesno != 0)
        {
          అక్షరం separator;
          size_t తదుపరి_pos = 2 + (sep == ',' && filesno < cwd_n_వాడిన - 1);

          ఒకవేళ (! పంక్తి_నిడివి
              || (! ckd_కూడు (&తదుపరి_pos, తదుపరి_pos, pos)
                  && ! ckd_కూడు (&తదుపరి_pos, తదుపరి_pos, len)
                  && తదుపరి_pos <= పంక్తి_నిడివి))
            {
              pos += 2;
              separator = ' ';
            }
          లేకపోతే
            {
              pos = 0;
              separator = eolbyte;
            }

          అక్షరం_రాయి (sep);
          అక్షరం_రాయి (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  అక్షరం_రాయి (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

స్థిర శూన్య
indent (size_t from, size_t గమ్యం)
{
  అయితే (from < గమ్యం)
    {
      ఒకవేళ (tabsize != 0 && గమ్యం / tabsize > (from + 1) / tabsize)
        {
          అక్షరం_రాయి ('\t');
          from += tabsize - from % tabsize;
        }
      లేకపోతే
        {
          అక్షరం_రాయి (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

స్థిర శూన్య
attach (అక్షరం *dest, అక్షరం స్థిరరాశి *dirname, అక్షరం స్థిరరాశి *name)
{
  అక్షరం స్థిరరాశి *dirnamep = dirname;

  /* Copy dirname if it is not ".".  */
  ఒకవేళ (dirname[0] != '.' || dirname[1] != 0)
    {
      అయితే (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      ఒకవేళ (dirnamep > dirname && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  అయితే (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

స్థిర శూన్య
init_స్తంభం_info (idx_t max_నిలువుల_సంఖ్య)
{
  /* Currently allocated columns in column_info.  */
  స్థిర idx_t స్తంభం_info_alloc;

  ఒకవేళ (స్తంభం_info_alloc < max_నిలువుల_సంఖ్య)
    {
      idx_t old_స్తంభం_info_alloc = స్తంభం_info_alloc;
      స్తంభం_info = xpalloc (స్తంభం_info, &స్తంభం_info_alloc,
                             max_నిలువుల_సంఖ్య - స్తంభం_info_alloc, -1,
                             పరిమాణం *స్తంభం_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t స్తంభం_info_growth = స్తంభం_info_alloc - old_స్తంభం_info_alloc, s;
      ఒకవేళ (ckd_కూడు (&s, old_స్తంభం_info_alloc + 1, స్తంభం_info_alloc)
          || ckd_mul (&s, s, స్తంభం_info_growth))
        xalloc_die ();
      size_t *p = xinmalloc (s >> 1, పరిమాణం *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      కొరకు (idx_t i = old_స్తంభం_info_alloc; i < స్తంభం_info_alloc; i++)
        {
          స్తంభం_info[i].నిలువు_arr = p;
          p += i + 1;
        }
    }

  కొరకు (idx_t i = 0; i < max_నిలువుల_సంఖ్య; ++i)
    {
      స్తంభం_info[i].valid_len = true;
      స్తంభం_info[i].పంక్తి_len = i * MIN_స్తంభం_వెడల్పు + 1;
      కొరకు (idx_t j = 0; j < i; ++j)
        స్తంభం_info[i].నిలువు_arr[j] = MIN_స్తంభం_వెడల్పు;
      స్తంభం_info[i].నిలువు_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

స్థిర idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_నిలువుల_సంఖ్య = 0 < max_idx && max_idx < cwd_n_వాడిన ? max_idx : cwd_n_వాడిన;

  init_స్తంభం_info (max_నిలువుల_సంఖ్య);

  /* Compute the maximum number of possible columns.  */
  కొరకు (idx_t filesno = 0; filesno < cwd_n_వాడిన; ++filesno)
    {
      నిర్మాణం fileinfo స్థిరరాశి *f = sorted_file[filesno];
      size_t name_నిడివి = నిడివి_యొక్క_file_name_and_frills (f);

      కొరకు (idx_t i = 0; i < max_నిలువుల_సంఖ్య; ++i)
        {
          ఒకవేళ (స్తంభం_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_వాడిన + i) / (i + 1))
                           : filesno % (i + 1));
              size_t real_నిడివి = name_నిడివి + (idx == i ? 0 : 2);

              ఒకవేళ (స్తంభం_info[i].నిలువు_arr[idx] < real_నిడివి)
                {
                  స్తంభం_info[i].పంక్తి_len += (real_నిడివి
                                              - స్తంభం_info[i].నిలువు_arr[idx]);
                  స్తంభం_info[i].నిలువు_arr[idx] = real_నిడివి;
                  స్తంభం_info[i].valid_len = (స్తంభం_info[i].పంక్తి_len
                                              <= పంక్తి_నిడివి);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t నిలువుల_సంఖ్య_2;
  కొరకు (నిలువుల_సంఖ్య_2 = max_నిలువుల_సంఖ్య; 1 < నిలువుల_సంఖ్య_2; --నిలువుల_సంఖ్య_2)
    {
      ఒకవేళ (స్తంభం_info[నిలువుల_సంఖ్య_2 - 1].valid_len)
        ఆపు;
    }

  తిరిగి నిలువుల_సంఖ్య_2;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]...\n"), కార్యక్రమం_name);
      fputs (_("\
List information about the FILEs (the current directory by అప్రమేయ).\n\
క్రమీకరించు_2 entries alphabetically ఒకవేళ none యొక్క -cftuvSUX nor --క్రమీకరించు is specified.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         చేయి not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         చేయి not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author యొక్క each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes కొరకు nongraphic characters\n\
"));
      oputs (_("\
      --ఖండం-కొలత=కొలత_2\n\
         with -l, scale sizes by కొలత_2 when printing them;\n\
         e.g., '--block-size=M'; see కొలత_2 format క్రింద\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         చేయి not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: క్రమీకరించు by, and show, ctime\n\
           (సమయం యొక్క చివరి change యొక్క file స్థితి_2 information);\n\
         with -l: show ctime and క్రమీకరించు by name;\n\
         otherwise: క్రమీకరించు by ctime, newest మొదటి\n\
"));
      oputs (_("\
  -C\n\
         list entries by columns\n\
"));
      oputs (_("\
      --color[=WHEN]\n\
         color the output WHEN; మరిన్ని info క్రింద\n\
"));
      oputs (_("\
  -d, --directory\n\
         list directories themselves, not their contents\n\
"));
      oputs (_("\
  -D, --dired\n\
         generate output designed కొరకు Emacs' dired బహుళకం_2\n\
"));
      oputs (_("\
  -f\n\
         same as -a -U\n\
"));
      oputs (_("\
  -F, --classify[=WHEN]\n\
         append indicator (one యొక్క */=>@|) గమ్యం entries WHEN\n\
"));
      oputs (_("\
      --file-type\n\
         like -F, except చేయి not append '*'\n\
"));
      oputs (_("\
      --format=మాట\n\
         across,horizontal (-x), commas (-m), దీర్ఘ (-l),\n\
         single-స్తంభం (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --నిండిన-సమయం\n\
         like -l --సమయం-style=నిండిన-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but చేయి not list owner\n\
"));
      oputs (_("\
      --సమూహం-directories-మొదటి\n\
         సమూహం directories before files\n\
"));
      oputs (_("\
  -G, --no-సమూహం\n\
         లోపల a దీర్ఘ listing, don't print సమూహం పేర్లు\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         with -l and -s, print sizes like 1K 234M 2G etc.\n\
"));
      oputs (_("\
      --si\n\
         likewise, but use powers యొక్క 1000 not 1024\n\
"));
      oputs (_("\
  -H, --dereference-command-పంక్తి\n\
         follow symbolic links listed on the command పంక్తి\n\
"));
      oputs (_("\
      --dereference-command-పంక్తి-symlink-గమ్యం-dir\n\
         follow each command పంక్తి symbolic link that points గమ్యం a directory\n\
"));
      oputs (_("\
      --hide=నమూనా\n\
         చేయి not list implied entries matching shell నమూనా\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file పేర్లు WHEN\n\
"));
      oputs (_("\
      --indicator-style=మాట\n\
         append indicator with style మాట గమ్యం నమోదు_2 పేర్లు:\n\
           none (అప్రమేయ), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index సంఖ్య యొక్క each file\n\
"));
      oputs (_("\
  -I, --ignore=నమూనా\n\
         చేయి not list implied entries matching shell నమూనా\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         అప్రమేయ గమ్యం 1024-byte ఖండాలు కొరకు file system usage;\n\
         వాడిన only with -s and చొప్పున directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a దీర్ఘ listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information కొరకు a symbolic link,\n\
         show information కొరకు the file the link references\n\
         rather than కొరకు the link itself\n\
"));
      oputs (_("\
  -m\n\
         fill వెడల్పు with a comma separated list యొక్క entries\n\
"));
      oputs (_("\
  -n, --numeric-uid-gid\n\
         like -l, but list numeric user and సమూహం IDs\n\
"));
      oputs (_("\
  -N, --literal\n\
         print నమోదు_2 పేర్లు without quoting\n\
"));
      oputs (_("\
  -o\n\
         like -l, but చేయి not list సమూహం information\n\
"));
      oputs (_("\
  -p, --indicator-style=slash\n\
         append / indicator గమ్యం directories\n\
"));
      oputs (_("\
  -q, --hide-control-chars\n\
         print ? instead యొక్క nongraphic characters\n\
"));
      oputs (_("\
      --show-control-chars\n\
         show nongraphic characters as-is;\n\
         the అప్రమేయ, unless కార్యక్రమం is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose నమోదు_2 పేర్లు లోపల ద్విగుణ quotes\n\
"));
      oputs (_("\
      --quoting-style=మాట\n\
         use quoting style మాట కొరకు నమోదు_2 పేర్లు:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --తిరగేయి\n\
         తిరగేయి క్రమం అయితే క్రమీకరణ\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --కొలత\n\
         print the allocated కొలత యొక్క each file, లోపల ఖండాలు\n\
"));
      oputs (_("\
  -S\n\
         క్రమీకరించు by file కొలత, అతిపెద్ద మొదటి\n\
"));
      oputs (_("\
      --క్రమీకరించు=మాట\n\
         change అప్రమేయ 'name' క్రమీకరించు గమ్యం మాట:\n\
           none (-U), కొలత (-S), సమయం (-t),\n\
           సంస్కరణ_3 (-v), extension (-X), name, వెడల్పు\n\
"));
      oputs (_("\
      --సమయం=మాట\n\
         select which timestamp వాడిన గమ్యం display or క్రమీకరించు;\n\
           access సమయం (-u): atime, access, use;\n\
           metadata change సమయం (-c): ctime, స్థితి_2;\n\
           modified సమయం (అప్రమేయ): mtime, modification;\n\
           birth సమయం: birth, creation;\n\
         with -l, మాట determines which సమయం గమ్యం show;\n\
         with --క్రమీకరించు=సమయం, క్రమీకరించు by మాట (newest మొదటి)\n\
"));
      oputs (_("\
      --సమయం-style=సమయం_STYLE\n\
         సమయం/date format with -l; see సమయం_STYLE క్రింద\n\
"));
      oputs (_("\
  -t\n\
         క్రమీకరించు by సమయం, newest మొదటి; see --సమయం\n\
"));
      oputs (_("\
  -T, --tabsize=నిలువుల_సంఖ్య\n\
         assume tab stops at each నిలువుల_సంఖ్య instead యొక్క 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: క్రమీకరించు by, and show, access సమయం;\n\
         with -l: show access సమయం and క్రమీకరించు by name;\n\
         otherwise: క్రమీకరించు by access సమయం, newest మొదటి\n\
"));
      oputs (_("\
  -U\n\
         చేయి not క్రమీకరించు directory entries\n\
"));
      oputs (_("\
  -v\n\
         natural క్రమీకరించు యొక్క (సంస్కరణ_3) numbers within పాఠ్యం\n\
"));
      oputs (_("\
  -w, --వెడల్పు=నిలువుల_సంఖ్య\n\
         సమితి_2 output వెడల్పు గమ్యం నిలువుల_సంఖ్య.  0 means no హద్దు\n\
"));
      oputs (_("\
  -x\n\
         list entries by పంక్తులు instead యొక్క by columns\n\
"));
      oputs (_("\
  -X\n\
         క్రమీకరించు alphabetically by నమోదు_2 extension\n\
"));
      oputs (_("\
  -Z, --context\n\
         print any security context యొక్క each file\n\
"));
      oputs (_("\
      --zero\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file చొప్పున పంక్తి\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_కొలత_note ();
      fputs (_("\
\n\
The సమయం_STYLE వాదం can be నిండిన-iso, దీర్ఘ-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like లోపల date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies గమ్యం non-recent files and FORMAT2 గమ్యం recent files.\n\
సమయం_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the సమయం_STYLE environment variable sets the అప్రమేయ style గమ్యం use.\n\
"), stdout);
      fputs (_("\
\n\
The WHEN వాదం defaults గమ్యం 'always' and can also be 'auto' or 'never'.\n\
"), stdout);
      fputs (_("\
\n\
Using color గమ్యం distinguish file types is disabled both by అప్రమేయ and\n\
with --color=never.  With --color=స్వయంచాలక, ls emits color codes only when\n\
standard output is connected గమ్యం a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command గమ్యం సమితి_2 it.\n\
"), stdout);
      fputs (_("\
\n\
Exit స్థితి_2:\n\
 0  ఒకవేళ OK,\n\
 1  ఒకవేళ minor problems (e.g., cannot access subdirectory),\n\
 2  ఒకవేళ serious trouble (e.g., cannot access command-పంక్తి వాదం).\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}
