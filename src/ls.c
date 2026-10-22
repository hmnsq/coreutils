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

#समावेश <config.h>
#समावेश <sys/types.h>

#समावेश <termios.h>
#अगर HAVE_STROPTS_H
# include <stropts.h>
#पूर्ण_अगर
#समावेश <sys/ioctl.h>

#अगर_घोषित WINSIZE_IN_PTEM
# include <sys/stream.h>
# include <sys/ptem.h>
#पूर्ण_अगर

#समावेश <stdio.h>
#समावेश <setjmp.h>
#समावेश <pwd.h>
#समावेश <getopt.h>
#समावेश <signal.h>

#अगर HAVE_LANGINFO_CODESET
# include <langinfo.h>
#पूर्ण_अगर

/* NonStop circa 2011 lacks both SA_RESTART and siginterrupt, so don't
   restart syscalls after a signal handler fires.  This may cause
   colors to get messed up on the screen if 'ls' is interrupted, but
   that's the best we can do on such a platform.  */
#अगर_अघोषित SA_RESTART
# define SA_RESTART 0
#पूर्ण_अगर

#समावेश <fnmatch.h>

/* Gnulib includes.  */
#समावेश "acl.h"
#समावेश "areadlink.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "c-strcase.h"
#समावेश "canonicalize.h"
#समावेश "dev-ino.h"
#समावेश "filemode.h"
#समावेश "filenamecat.h"
#समावेश "filevercmp.h"
#समावेश "hard-locale.h"
#समावेश "hash.h"
#समावेश "human.h"
#समावेश "idcache.h"
#समावेश "mbswidth.h"
#समावेश "mpsort.h"
#समावेश "obstack.h"
#समावेश "quote.h"
#समावेश "stat-size.h"
#समावेश "stat-time.h"
#समावेश "strftime.h"
#समावेश "xdectoint.h"
#समावेश "xgethostname.h"
#समावेश "xmemdup0.h"
#समावेश "xstrtol-error.h"
#समावेश "xstrtol.h"

#समावेश "system.h"

#समावेश "dircolors.h"
#समावेश "ls.h"
#समावेश "statx.h"
#समावेश "term-sig.h"

/* Include <sys/capability.h> last to avoid a clash of <sys/types.h>
   include guards with some premature versions of libcap.
   For more details, see <https://bugzilla.redhat.com/483548>.  */
#अगर_घोषित HAVE_CAP
# include <sys/capability.h>
#पूर्ण_अगर

#अगर HAVE_LINUX_XATTR_H
# include <linux/xattr.h>
# ifndef XATTR_NAME_CAPS
#  define XATTR_NAME_CAPS "security.capability"
# endif
#पूर्ण_अगर

#घोषणा PROGRAM_NAME (ls_mode == LS_LS ? "ls" \
                      : (ls_mode == LS_MULTI_COL \
                         ? "dir" : "vdir"))

#घोषणा AUTHORS \
  proper_name ("Richard M. Stallman"), \
  proper_name ("David MacKenzie")

#घोषणा obstack_chunk_alloc दो_स्मृति
#घोषणा obstack_chunk_free मुक्त

/* Unix-based readdir implementations have historically returned a dirent.d_ino
   value that is sometimes not equal to the stat-obtained st_ino value for
   that same entry.  This error occurs for a readdir entry that refers
   to a mount point.  readdir's error is to return the inode number of
   the underlying directory -- one that typically cannot be stat'ed, as
   long as a file system is mounted on that directory.  RELIABLE_D_INO
   encapsulates whether we can use the more efficient approach of relying
   on readdir-supplied d_ino values, or whether we must incur the cost of
   calling stat or lstat to obtain each guaranteed-valid inode number.  */

#अगर_अघोषित READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define READDIR_LIES_ABOUT_MOUNTPOINT_D_INO 1
#पूर्ण_अगर

#अगर READDIR_LIES_ABOUT_MOUNTPOINT_D_INO
# define RELIABLE_D_INO(dp) NOT_AN_INODE_संख्या
#अन्यथा
# define RELIABLE_D_INO(dp) D_INO (dp)
#पूर्ण_अगर

#अगर ! HAVE_STRUCT_STAT_ST_AUTHOR
# define st_author st_uid
#पूर्ण_अगर

क्रमागत filetype
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
क्रमागत { filetype_cardinality = arg_directory + 1 };

/* Display letters and indicators for each filetype.
   Keep these in sync with enum filetype.  */
अटल अक्षर स्थिर filetype_letter[] =
  {'?', 'p', 'c', 'd', 'b', '-', 'l', 's', 'w', 'd'};
static_assert (countof (filetype_letter) == filetype_cardinality);

/* Map enum filetype to <dirent.h> d_type values.  */
अटल अचिन्हित अक्षर स्थिर filetype_d_type[] =
  {
    DT_UNKNOWN, DT_FIFO, DT_CHR, DT_DIR, DT_BLK, DT_REG, DT_LNK, DT_SOCK,
    DT_WHT, DT_DIR
  };
static_assert (countof (filetype_d_type) == filetype_cardinality);

/* Map d_type values to enum filetype.  */
अटल अक्षर स्थिर d_type_filetype[अच_अक्षर_उच्च + 1] =
  {
    [DT_BLK] = blockdev, [DT_CHR] = chardev, [DT_DIR] = directory,
    [DT_FIFO] = fifo, [DT_LNK] = symbolic_link, [DT_REG] = normal,
    [DT_SOCK] = sock, [DT_WHT] = whiteout
  };

क्रमागत acl_type
  {
    ACL_T_NONE,
    ACL_T_UNKNOWN,
    ACL_T_LSM_CONTEXT_ONLY,
    ACL_T_YES
  };

काष्ठा fileinfo
  {
    /* The file name.  */
    अक्षर *name;

    /* For symbolic link, name of the file linked to, otherwise zero.  */
    अक्षर *linkname;

    /* For terminal hyperlinks. */
    अक्षर *absolute_name;

    काष्ठा stat stat;

    क्रमागत filetype filetype;

    /* For symbolic link and long listing, st_mode of file linked to, otherwise
       zero.  */
    mode_t linkmode;

    /* security context.  */
    अक्षर *scontext;

    bool stat_ok;

    /* For symbolic link and color printing, true if linked-to file
       exists, otherwise false.  */
    bool linkok;

    /* For long listings, true if the file has an access control list,
       or a security context.  */
    क्रमागत acl_type acl_type;

    /* For color listings, true if a regular file has capability info.  */
    bool has_capability;

    /* Whether file name needs quoting. tri-state with -1 == unknown.  */
    पूर्णांक quoted;

    /* Cached screen width (including quoting).  */
    माप_प्रकार width;
  };

/* Null is a valid character in a color indicator (think about Epson
   printers, for example) so we have to use a length/buffer string
   type.  */

काष्ठा bin_str
  {
    माप_प्रकार len;			/* Number of bytes */
    अक्षर स्थिर *string;		/* Pointer to the same */
  };

#अगर ! HAVE_TCGETPGRP
# define tcgetpgrp(Fd) 0
#पूर्ण_अगर

अटल माप_प्रकार quote_name (अक्षर स्थिर *name,
                          काष्ठा quoting_options स्थिर *options,
                          पूर्णांक needs_general_quoting,
                          स्थिर काष्ठा bin_str *color,
                          bool allow_pad, काष्ठा obstack *ढेर,
                          अक्षर स्थिर *absolute_name);
अटल माप_प्रकार quote_name_buf (अक्षर **inbuf, माप_प्रकार bufsize, अक्षर *name,
                              काष्ठा quoting_options स्थिर *options,
                              पूर्णांक needs_general_quoting, माप_प्रकार *width,
                              bool *pad);
अटल पूर्णांक decode_switches (पूर्णांक argc, अक्षर **argv);
अटल bool file_ignored (अक्षर स्थिर *name);
अटल uintmax_t gobble_file (अक्षर स्थिर *name, क्रमागत filetype type,
                              ino_t inode, bool command_पंक्ति_arg,
                              अक्षर स्थिर *स_नाम);
अटल स्थिर काष्ठा bin_str * get_color_indicator (स्थिर काष्ठा fileinfo *f,
                                                   bool symlink_target);
अटल bool print_color_indicator (स्थिर काष्ठा bin_str *ind);
अटल व्योम put_indicator (स्थिर काष्ठा bin_str *ind);
अटल व्योम add_ignore_pattern (अक्षर स्थिर *pattern);
अटल व्योम attach (अक्षर *dest, अक्षर स्थिर *स_नाम, अक्षर स्थिर *name);
अटल व्योम clear_files (व्योम);
अटल व्योम extract_dirs_from_files (अक्षर स्थिर *स_नाम,
                                     bool command_पंक्ति_arg);
अटल bool get_link_name (अक्षर स्थिर *filename, काष्ठा fileinfo *f,
                           bool command_पंक्ति_arg);
अटल व्योम indent (माप_प्रकार from, माप_प्रकार to);
अटल idx_t calculate_columns (bool by_columns);
अटल व्योम print_current_files (व्योम);
अटल व्योम print_dir (अक्षर स्थिर *name, अक्षर स्थिर *realname,
                       bool command_पंक्ति_arg);
अटल माप_प्रकार print_file_name_and_frills (स्थिर काष्ठा fileinfo *f,
                                          माप_प्रकार start_col);
अटल व्योम print_horizontal (व्योम);
अटल पूर्णांक format_user_width (uid_t u);
अटल पूर्णांक format_group_width (gid_t g);
अटल व्योम print_long_format (स्थिर काष्ठा fileinfo *f);
अटल व्योम print_many_per_पंक्ति (व्योम);
अटल माप_प्रकार print_name_with_quoting (स्थिर काष्ठा fileinfo *f,
                                       bool symlink_target,
                                       काष्ठा obstack *ढेर,
                                       माप_प्रकार start_col);
अटल व्योम prep_non_filename_text (व्योम);
अटल bool print_type_indicator (bool stat_ok, mode_t mode,
                                  क्रमागत filetype type);
अटल व्योम print_with_separator (अक्षर sep);
अटल व्योम queue_directory (अक्षर स्थिर *name, अक्षर स्थिर *realname,
                             bool command_पंक्ति_arg);
अटल व्योम sort_files (व्योम);
अटल व्योम parse_ls_color (व्योम);

अटल माप_प्रकार quote_name_width (अक्षर स्थिर *name,
                                काष्ठा quoting_options स्थिर *options,
                                पूर्णांक needs_general_quoting);

/* Initial size of hash table.
   Most hierarchies are likely to be shallower than this.  */
क्रमागत { INITIAL_TABLE_आकार = 30 };

/* The set of 'active' directories, from the current command-line argument
   to the level in the hierarchy at which files are being listed.
   A directory is represented by its device and inode numbers (struct dev_ino).
   A directory is added to this set when ls begins listing it or its
   entries, and it is removed from the set just after ls has finished
   processing it.  This set is used solely to detect loops, e.g., with
   mkdir loop; cd loop; ln -s ../loop sub; ls -RL  */
अटल Hash_table *active_dir_set;

#घोषणा LOOP_DETECT (!!active_dir_set)

/* The table of files in the current directory:

   'cwd_file' points to a vector of 'struct fileinfo', one per file.
   'cwd_n_alloc' is the number of elements space has been allocated for.
   'cwd_n_used' is the number actually in use.  */

/* Address of block containing the files that are described.  */
अटल काष्ठा fileinfo *cwd_file;

/* Length of block that 'cwd_file' points to, measured in files.  */
अटल idx_t cwd_n_alloc;

/* Index of first unused slot in 'cwd_file'.  */
अटल idx_t cwd_n_used;

/* Whether files needs may need padding due to quoting.  */
अटल bool cwd_some_quoted;

/* Whether quoting style _may_ add outer quotes,
   and whether aligning those is useful.  */
अटल bool align_variable_outer_quotes;

/* Vector of pointers to files, in proper sorted order, and the number
   of entries allocated for it.  */
अटल व्योम **sorted_file;
अटल माप_प्रकार sorted_file_alloc;

/* When true, in a color listing, color each symlink name according to the
   type of file it points to.  Otherwise, color them according to the 'ln'
   directive in LS_COLORS.  Dangling (orphan) symlinks are treated specially,
   regardless.  This is set when 'ln=target' appears in LS_COLORS.  */

अटल bool color_symlink_as_referent;

अटल अक्षर स्थिर *hostname;

/* Mode of appropriate file for coloring.  */
अटल mode_t
file_or_link_mode (काष्ठा fileinfo स्थिर *file)
{
  वापस (color_symlink_as_referent && file->linkok
          ? file->linkmode : file->stat.st_mode);
}


/* Record of one pending directory waiting to be listed.  */

काष्ठा pending
  {
    अक्षर *name;
    /* If the directory is actually the file pointed to by a symbolic link we
       were told to list, 'realname' will contain the name of the symbolic
       link, otherwise zero.  */
    अक्षर *realname;
    bool command_पंक्ति_arg;
    काष्ठा pending *अग्र;
  };

अटल काष्ठा pending *pending_dirs;

/* Current time in seconds and nanoseconds since 1970, updated as
   needed when deciding whether a file is recent.  */

अटल काष्ठा timespec current_time;

अटल bool print_scontext;
अटल अक्षर UNKNOWN_SECURITY_CONTEXT[] = "?";

/* Whether any of the files has an ACL.  This affects the width of the
   mode column.  */

अटल bool any_has_acl;

/* The number of columns to use for columns containing inode numbers,
   block sizes, link counts, owners, groups, authors, major device
   numbers, minor device numbers, and file sizes, respectively.  */

अटल पूर्णांक inode_संख्या_width;
अटल पूर्णांक block_आकार_width;
अटल पूर्णांक nlink_width;
अटल पूर्णांक scontext_width;
अटल पूर्णांक owner_width;
अटल पूर्णांक group_width;
अटल पूर्णांक author_width;
अटल पूर्णांक major_device_संख्या_width;
अटल पूर्णांक minor_device_संख्या_width;
अटल पूर्णांक file_आकार_width;

/* Option flags */

/* long_format for lots of info, one per line.
   one_per_line for just names, one per line.
   many_per_line for just names, many per line, sorted vertically.
   horizontal for just names, many per line, sorted horizontally.
   with_commas for just names, many per line, separated by commas.

   -l (and other options that imply -l), -1, -C, -x and -m control
   this parameter.  */

क्रमागत format
  {
    long_format,		/* -l and other options that imply -l */
    one_per_पंक्ति,		/* -1 */
    many_per_पंक्ति,		/* -C */
    horizontal,			/* -x */
    with_commas			/* -m */
  };

अटल क्रमागत format format;

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  'locale' uses locale-dependent timestamps.  */
क्रमागत time_style
  {
    full_iso_time_style,	/* --time-style=full-iso */
    long_iso_time_style,	/* --time-style=long-iso */
    iso_time_style,		/* --time-style=iso */
    locale_time_style		/* --time-style=locale */
  };

अटल अक्षर स्थिर *स्थिर time_style_args[] =
{
  "full-iso", "long-iso", "iso", "locale", शून्य
};
अटल क्रमागत time_style स्थिर time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style,
  locale_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

/* Type of time to print or sort by.  Controlled by -c and -u.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

क्रमागत time_type
  {
    time_mtime = 0,		/* default */
    time_ctime,			/* -c */
    time_atime,			/* -u */
    time_btime,                 /* birth time */
    time_numtypes		/* the number of elements of this enum */
  };

अटल क्रमागत time_type time_type;
अटल bool explicit_time;

/* The file characteristic to sort by.  Controlled by -t, -S, -U, -X, -v.
   The values of each item of this enum are important since they are
   used as indices in the sort functions array (see sort_files()).  */

क्रमागत sort_type
  {
    sort_name = 0,		/* default */
    sort_extension,		/* -X */
    sort_width,
    sort_आकार,			/* -S */
    sort_version,		/* -v */
    sort_time,			/* -t; must be second to last */
    sort_none,			/* -U; must be last */
    sort_numtypes		/* the number of elements of this enum */
  };

अटल क्रमागत sort_type sort_type;

/* Direction of sort.
   false means highest first if numeric,
   lowest first if alphabetic;
   these are the defaults.
   true means the opposite order in each case.  -r  */

अटल bool sort_reverse;

/* True means to display owner information.  -g turns this off.  */

अटल bool print_owner = true;

/* True means to display author information.  */

अटल bool print_author;

/* True means to display group information.  -G and -o turn this off.  */

अटल bool print_group = true;

/* True means print the user and group id's as numbers rather
   than as names.  -n  */

अटल bool numeric_ids;

/* True means mention the size in blocks of each file.  -s  */

अटल bool print_block_आकार;

/* Human-readable options for output, when printing block counts.  */
अटल पूर्णांक human_output_opts;

/* The units to use when printing block counts.  */
अटल uintmax_t output_block_आकार;

/* Likewise, but for file sizes.  */
अटल पूर्णांक file_human_output_opts;
अटल uintmax_t file_output_block_आकार = 1;

/* Follow the output with a special string.  Using this format,
   Emacs' dired mode starts up twice as fast, and can handle all
   strange characters in file names.  */
अटल bool dired;

/* 'none' means don't mention the type of files.
   'slash' means mention directories only, with a '/'.
   'file_type' means mention file types.
   'classify' means mention file types and mark executables.

   Controlled by -F, -p, and --indicator-style.  */

क्रमागत indicator_style
  {
    none = 0,	/*     --indicator-style=none (default) */
    slash,	/* -p, --indicator-style=slash */
    file_type,	/*     --indicator-style=file-type */
    classify	/* -F, --indicator-style=classify */
  };

अटल क्रमागत indicator_style indicator_style;

/* Names of indicator styles.  */
अटल अक्षर स्थिर *स्थिर indicator_style_args[] =
{
  "none", "slash", "file-type", "classify", शून्य
};
अटल क्रमागत indicator_style स्थिर indicator_style_types[] =
{
  none, slash, file_type, classify
};
ARGMATCH_VERIFY (indicator_style_args, indicator_style_types);

/* True means use colors to mark types.  Also define the different
   colors as well as the stuff for the LS_COLORS environment variable.
   The LS_COLORS variable is now in a termcap-like format.  */

अटल bool print_with_color;

अटल bool print_hyperlink;

/* Whether we used any colors in the output so far.  If so, we will
   need to restore the default color later.  If not, we will need to
   call prep_non_filename_text before using color for the first time. */

अटल bool used_color = false;

क्रमागत when_type
  {
    when_never,		/* 0: default or --color=never */
    when_always,	/* 1: --color=always */
    when_if_tty		/* 2: --color=tty */
  };

क्रमागत Dereference_symlink
  {
    DEREF_UNDEFINED = 0,		/* default */
    DEREF_NEVER,
    DEREF_COMMAND_पंक्ति_ARGUMENTS,	/* -H */
    DEREF_COMMAND_पंक्ति_SYMLINK_TO_DIR,	/* the default, in certain cases */
    DEREF_ALWAYS			/* -L */
  };

क्रमागत indicator_no
  {
    C_बायाँ, C_दायाँ, C_END, C_RESET, C_NORM, C_FILE, C_DIR, C_LINK,
    C_FIFO, C_SOCK,
    C_BLK, C_CHR, C_MISSING, C_ORPHAN, C_EXEC, C_DOOR, C_SETUID, C_SETGID,
    C_STICKY, C_OTHER_WRITABLE, C_STICKY_OTHER_WRITABLE, C_CAP, C_MULTIHARDLINK,
    C_CLR_TO_EOL
  };

अटल अक्षर स्थिर indicator_name[][2]=
  {
    {'l','c'}, {'r','c'}, {'e','c'}, {'r','s'}, {'n','o'},
    {'f','i'}, {'d','i'}, {'l','n'},
    {'p','i'}, {'s','o'},
    {'b','d'}, {'c','d'}, {'m','i'}, {'o','r'}, {'e','x'},
    {'d','o'}, {'s','u'}, {'s','g'},
    {'s','t'}, {'o','w'}, {'t','w'}, {'c','a'}, {'m','h'},
    {'c','l'}
  };

काष्ठा color_ext_type
  {
    काष्ठा bin_str ext;		/* The extension we're looking for */
    काष्ठा bin_str seq;		/* The sequence to output when we do */
    bool   exact_match;		/* Whether to compare case insensitively */
    काष्ठा color_ext_type *अग्र;	/* Next in list */
  };

अटल काष्ठा bin_str color_indicator[] =
  {
    { 2, (अक्षर स्थिर []) {'\033','['} },/* lc: Left of color sequence */
    { 1, (अक्षर स्थिर []) {'m'} },	/* rc: Right of color sequence */
    { 0, शून्य },			/* ec: End color (replaces lc+rs+rc) */
    { 1, (अक्षर स्थिर []) {'0'} },	/* rs: Reset to ordinary colors */
    { 0, शून्य },			/* no: Normal */
    { 0, शून्य },			/* fi: File: default */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','4'}) },	/* di: Directory: bright blue */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','6'}) },	/* ln: Symlink: bright cyan */
    { 2, (अक्षर स्थिर []) {'3','3'} },		/* pi: Pipe: yellow/brown */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','5'}) },	/* so: Socket: bright magenta */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','3'}) },	/* bd: Block device: bright yellow */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','3'}) },	/* cd: Char device: bright yellow */
    { 0, शून्य },			/* mi: Missing file: undefined */
    { 0, शून्य },			/* or: Orphaned symlink: undefined */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','2'}) },	/* ex: Executable: bright green */
    { 5, ((अक्षर स्थिर [])
          {'0','1',';','3','5'}) },	/* do: Door: bright magenta */
    { 5, ((अक्षर स्थिर [])
          {'3','7',';','4','1'}) },	/* su: setuid: white on red */
    { 5, ((अक्षर स्थिर [])
          {'3','0',';','4','3'}) },	/* sg: setgid: black on yellow */
    { 5, ((अक्षर स्थिर [])
          {'3','7',';','4','4'}) },	/* st: sticky: black on blue */
    { 5, ((अक्षर स्थिर [])
          {'3','4',';','4','2'}) },	/* ow: other-writable: blue on green */
    { 5, ((अक्षर स्थिर [])
          {'3','0',';','4','2'}) },	/* tw: ow w/ sticky: black on green */
    { 0, शून्य },			/* ca: disabled by default */
    { 0, शून्य },			/* mh: disabled by default */
    { 3, ((अक्षर स्थिर [])
          {'\033','[','K'}) },		/* cl: clear to end of line */
  };

/* A list mapping file extensions to corresponding display sequence.  */
अटल काष्ठा color_ext_type *color_ext_list = शून्य;

/* Buffer for color sequences */
अटल अक्षर *color_buf;

/* True means to check for orphaned symbolic link, for displaying
   colors, or to group symlink to directories with other dirs.  */

अटल bool check_symlink_mode;

/* True means mention the inode number of each file.  -i  */

अटल bool print_inode;

/* What to do with symbolic links.  Affected by -d, -F, -H, -l (and
   other options that imply -l), and -L.  */

अटल क्रमागत Dereference_symlink dereference;

/* True means when a directory is found, display info on its
   contents.  -R  */

अटल bool recursive;

/* True means when an argument is a directory name, display info
   on it itself.  -d  */

अटल bool immediate_dirs;

/* True means that directories are grouped before files. */

अटल bool directories_first;

/* Which files to ignore.  */

अटल क्रमागत
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

काष्ठा ignore_pattern
  {
    अक्षर स्थिर *pattern;
    काष्ठा ignore_pattern *अग्र;
  };

अटल काष्ठा ignore_pattern *ignore_patterns;

/* Similar to IGNORE_PATTERNS, except that -a or -A causes this
   variable itself to be ignored.  */
अटल काष्ठा ignore_pattern *hide_patterns;

/* True means output nongraphic chars in file names as '?'.
   (-q, --hide-control-chars)
   qmark_funny_chars and the quoting style (-Q, --quoting-style=WORD) are
   independent.  The algorithm is: first, obey the quoting style to get a
   string representing the file name;  then, if qmark_funny_chars is set,
   replace all nonprintable chars in that string with '?'.  It's necessary
   to replace nonprintable chars even in quoted strings, because we don't
   want to mess up the terminal if control chars get sent to it, and some
   quoting methods pass through control chars as-is.  */
अटल bool qmark_funny_chars;

/* True means extra replacement of newline with '?'
   in cases where a newline in a file name would be
   ambiguous with newline separators.  */
अटल bool qmark_newline;

/* Quoting options for file and dir name output.  */

अटल काष्ठा quoting_options *filename_quoting_options;
अटल काष्ठा quoting_options *dirname_quoting_options;

/* The number of chars per hardware tab stop.  Setting this to zero
   inhibits the use of TAB characters for separating columns.  -T */
अटल माप_प्रकार tabsize;

/* True means print each directory name before listing it.  */

अटल bool print_dir_name;

/* The line length to use for breaking lines in many-per-line format.
   Can be set with -w.  If zero, there is no limit.  */

अटल माप_प्रकार पंक्ति_लंबाई;

/* The local time zone rules, as per the TZ environment variable.  */

अटल timezone_t localtz;

/* If true, the file listing format requires that stat be called on
   each file.  */

अटल bool format_needs_stat;

/* Similar to 'format_needs_stat', but set if only the file type is
   needed.  */

अटल bool format_needs_type;

/* Like 'format_needs_stat', but set only if capability colors are needed.  */

अटल bool format_needs_capability;

/* An arbitrary limit on the number of bytes in a printed timestamp.
   This is set to a relatively small value to avoid the need to worry
   about denial-of-service attacks on servers that run "ls" on behalf
   of remote clients.  1000 bytes should be enough for any practical
   timestamp format.  */

क्रमागत { TIME_STAMP_LEN_MAXIMUM = MAX (1000, INT_STRLEN_BOUND (समय_प्रकार)) };

/* strftime formats for non-recent and recent files, respectively, in
   -l output.  */

अटल अक्षर स्थिर *long_time_format[2] =
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

अटल sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */

अटल संक_पूर्ण_प्रकार अस्थिर interrupt_signal;

/* A count of the number of pending stop signals that have been received.  */

अटल संक_पूर्ण_प्रकार अस्थिर stop_signal_गिनती;

/* Desired exit status.  */

अटल पूर्णांक exit_status;

/* Exit statuses.  */
क्रमागत
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
क्रमागत
{
  AUTHOR_OPTION = अक्षर_उच्च + 1,
  BLOCK_आकार_OPTION,
  COLOR_OPTION,
  DEREFERENCE_COMMAND_पंक्ति_SYMLINK_TO_DIR_OPTION,
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

अटल काष्ठा option स्थिर long_options[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"escape", no_argument, शून्य, 'b'},
  {"directory", no_argument, शून्य, 'd'},
  {"dired", no_argument, शून्य, 'D'},
  {"full-time", no_argument, शून्य, FULL_TIME_OPTION},
  {"group-directories-first", no_argument, शून्य,
   GROUP_DIRECTORIES_FIRST_OPTION},
  {"human-readable", no_argument, शून्य, 'h'},
  {"inode", no_argument, शून्य, 'i'},
  {"kibibytes", no_argument, शून्य, 'k'},
  {"numeric-uid-gid", no_argument, शून्य, 'n'},
  {"no-group", no_argument, शून्य, 'G'},
  {"hide-control-chars", no_argument, शून्य, 'q'},
  {"reverse", no_argument, शून्य, 'r'},
  {"size", no_argument, शून्य, 's'},
  {"width", required_argument, शून्य, 'w'},
  {"almost-all", no_argument, शून्य, 'A'},
  {"ignore-backups", no_argument, शून्य, 'B'},
  {"classify", optional_argument, शून्य, 'F'},
  {"file-type", no_argument, शून्य, FILE_TYPE_INDICATOR_OPTION},
  {"si", no_argument, शून्य, SI_OPTION},
  {"dereference-command-line", no_argument, शून्य, 'H'},
  {"dereference-command-line-symlink-to-dir", no_argument, शून्य,
   DEREFERENCE_COMMAND_पंक्ति_SYMLINK_TO_DIR_OPTION},
  {"hide", required_argument, शून्य, HIDE_OPTION},
  {"ignore", required_argument, शून्य, 'I'},
  {"indicator-style", required_argument, शून्य, INDICATOR_STYLE_OPTION},
  {"dereference", no_argument, शून्य, 'L'},
  {"literal", no_argument, शून्य, 'N'},
  {"quote-name", no_argument, शून्य, 'Q'},
  {"quoting-style", required_argument, शून्य, QUOTING_STYLE_OPTION},
  {"recursive", no_argument, शून्य, 'R'},
  {"format", required_argument, शून्य, FORMAT_OPTION},
  {"show-control-chars", no_argument, शून्य, SHOW_CONTROL_CHARS_OPTION},
  {"sort", required_argument, शून्य, SORT_OPTION},
  {"tabsize", required_argument, शून्य, 'T'},
  {"time", required_argument, शून्य, TIME_OPTION},
  {"time-style", required_argument, शून्य, TIME_STYLE_OPTION},
  {"zero", no_argument, शून्य, ZERO_OPTION},
  {"color", optional_argument, शून्य, COLOR_OPTION},
  {"hyperlink", optional_argument, शून्य, HYPERLINK_OPTION},
  {"block-size", required_argument, शून्य, BLOCK_आकार_OPTION},
  {"context", no_argument, शून्य, 'Z'},
  {"author", no_argument, शून्य, AUTHOR_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

अटल अक्षर स्थिर *स्थिर format_args[] =
{
  "verbose", "long", "commas", "horizontal", "across",
  "vertical", "single-column", शून्य
};
अटल क्रमागत format स्थिर format_types[] =
{
  long_format, long_format, with_commas, horizontal, horizontal,
  many_per_पंक्ति, one_per_पंक्ति
};
ARGMATCH_VERIFY (format_args, format_types);

अटल अक्षर स्थिर *स्थिर sort_args[] =
{
  "none", "size", "time", "version", "extension",
  "name", "width", शून्य
};
अटल क्रमागत sort_type स्थिर sort_types[] =
{
  sort_none, sort_आकार, sort_time, sort_version, sort_extension,
  sort_name, sort_width
};
ARGMATCH_VERIFY (sort_args, sort_types);

अटल अक्षर स्थिर *स्थिर time_args[] =
{
  "atime", "access", "use",
  "ctime", "status",
  "mtime", "modification",
  "birth", "creation",
  शून्य
};
अटल क्रमागत time_type स्थिर time_types[] =
{
  time_atime, time_atime, time_atime,
  time_ctime, time_ctime,
  time_mtime, time_mtime,
  time_btime, time_btime,
};
ARGMATCH_VERIFY (time_args, time_types);

अटल अक्षर स्थिर *स्थिर when_args[] =
{
  /* force and none are for compatibility with another color-ls version */
  "always", "yes", "force",
  "never", "no", "none",
  "auto", "tty", "if-tty", शून्य
};
अटल क्रमागत when_type स्थिर when_types[] =
{
  when_always, when_always, when_always,
  when_never, when_never, when_never,
  when_if_tty, when_if_tty, when_if_tty
};
ARGMATCH_VERIFY (when_args, when_types);

/* Information about filling a column.  */
काष्ठा column_info
{
  bool valid_len;
  माप_प्रकार पंक्ति_len;
  माप_प्रकार *col_arr;
};

/* Array with information about column fullness.  */
अटल काष्ठा column_info *column_info;

/* Maximum number of columns ever possible for this display.  */
अटल माप_प्रकार max_idx;

/* The minimum width of a non-final column is 3: 1 character for the name
   and 2 for the separating white space.  The final column is not padded.  */
क्रमागत { MIN_COLUMN_WIDTH = 3 };


/* This zero-based index is for the --dired option.  It is incremented
   for each byte of output generated by this program so that the beginning
   and ending indices (in that output) of every file name can be recorded
   and later output themselves.  */
अटल off_t dired_pos;

अटल व्योम
dired_outbyte (अक्षर c)
{
  dired_pos++;
  अक्षर_दो (c);
}

/* Output the buffer S, of length S_LEN, and increment DIRED_POS by S_LEN.  */
अटल व्योम
dired_outbuf (अक्षर स्थिर *s, माप_प्रकार s_len)
{
  dired_pos += s_len;
  ख_डालो (s, माप *s, s_len, मानक_निकास);
}

/* Output the string S, and increment DIRED_POS by its length.  */
अटल व्योम
dired_outstring (अक्षर स्थिर *s)
{
  dired_outbuf (s, म_माप (s));
}

अटल व्योम
dired_indent (व्योम)
{
  अगर (dired)
    dired_outstring ("  ");
}

/* With --dired, store pairs of beginning and ending indices of file names.  */
अटल काष्ठा obstack dired_obstack;

/* With --dired, store pairs of beginning and ending indices of any
   directory names that appear as headers (just before 'total' line)
   for lists of directory entries.  Such directory names are seen when
   listing hierarchies using -R and when a directory is listed with at
   least one other command line argument.  */
अटल काष्ठा obstack subdired_obstack;

/* Save the current index on the specified obstack, OBS.  */
अटल व्योम
धकेलो_current_dired_pos (काष्ठा obstack *obs)
{
  अगर (dired)
    obstack_grow (obs, &dired_pos, माप dired_pos);
}

/* With -R, this stack is used to help detect directory cycles.
   The device/inode pairs on this stack mirror the pairs in the
   active_dir_set hash table.  */
अटल काष्ठा obstack dev_ino_obstack;

/* Push a pair onto the device/inode stack.  */
अटल व्योम
dev_ino_धकेलो (dev_t dev, ino_t ino)
{
  व्योम *vdi;
  काष्ठा dev_ino *di;
  पूर्णांक dev_ino_आकार = माप *di;
  obstack_blank (&dev_ino_obstack, dev_ino_आकार);
  vdi = obstack_अग्र_free (&dev_ino_obstack);
  di = vdi;
  di--;
  di->st_dev = dev;
  di->st_ino = ino;
}

/* Pop a dev/ino struct off the global dev_ino_obstack
   and return that struct.  */
अटल काष्ठा dev_ino
dev_ino_निकालो (व्योम)
{
  व्योम *vdi;
  काष्ठा dev_ino *di;
  पूर्णांक dev_ino_आकार = माप *di;
  affirm (dev_ino_आकार <= obstack_object_आकार (&dev_ino_obstack));
  obstack_blank_fast (&dev_ino_obstack, -dev_ino_आकार);
  vdi = obstack_अग्र_free (&dev_ino_obstack);
  di = vdi;
  वापस *di;
}

अटल व्योम
assert_matching_dev_ino (अक्षर स्थिर *name, काष्ठा dev_ino di)
{
  MAYBE_UNUSED काष्ठा stat sb;
  assure (0 <= stat (name, &sb));
  assure (sb.st_dev == di.st_dev);
  assure (sb.st_ino == di.st_ino);
}

अटल अक्षर eolbyte = '\n';

/* Write to standard output PREFIX, followed by the quoting style and
   a space-separated list of the integers stored in OS all on one line.  */

अटल व्योम
dired_dump_obstack (अक्षर स्थिर *prefix, काष्ठा obstack *os)
{
  माप_प्रकार n_pos;

  n_pos = obstack_object_आकार (os) / माप (dired_pos);
  अगर (n_pos > 0)
    {
      off_t *pos = obstack_finish (os);
      ख_माला_दो (prefix, मानक_निकास);
      क्रम (माप_प्रकार i = 0; i < n_pos; i++)
        {
          intmax_t p = pos[i];
          म_लिखो (" %jd", p);
        }
      अक्षर_दो ('\n');
    }
}

/* Return the platform birthtime member of the stat structure,
   or fallback to the mtime member, which we have populated
   from the statx structure or reset to an invalid timestamp
   where birth time is not supported.  */
अटल काष्ठा timespec
get_stat_btime (काष्ठा stat स्थिर *st)
{
  काष्ठा timespec btimespec;

#अगर HAVE_STATX && defined STATX_INO
  btimespec = get_stat_mtime (st);
#अन्यथा
  btimespec = get_stat_birthtime (st);
#पूर्ण_अगर

  वापस btimespec;
}

#अगर HAVE_STATX && defined STATX_INO
ATTRIBUTE_PURE
अटल अचिन्हित पूर्णांक
time_type_to_statx (व्योम)
{
  चयन (time_type)
    {
    हाल time_ctime:
      वापस STATX_CTIME;
    हाल time_mtime:
      वापस STATX_MTIME;
    हाल time_atime:
      वापस STATX_ATIME;
    हाल time_btime:
      वापस STATX_BTIME;
    हाल time_numtypes: शेष:
      unreachable ();
    }
    वापस 0;
}

ATTRIBUTE_PURE
अटल अचिन्हित पूर्णांक
calc_req_mask (व्योम)
{
  अचिन्हित पूर्णांक mask = STATX_MODE;

  अगर (print_inode)
    mask |= STATX_INO;

  अगर (print_block_आकार)
    mask |= STATX_BLOCKS;

  अगर (format == long_format) {
    mask |= STATX_NLINK | STATX_आकार | time_type_to_statx ();
    अगर (print_owner || print_author)
      mask |= STATX_UID;
    अगर (print_group)
      mask |= STATX_GID;
  }

  चयन (sort_type)
    {
    हाल sort_none:
    हाल sort_name:
    हाल sort_version:
    हाल sort_extension:
    हाल sort_width:
      अवरोध;
    हाल sort_time:
      mask |= time_type_to_statx ();
      अवरोध;
    हाल sort_आकार:
      mask |= STATX_आकार;
      अवरोध;
    हाल sort_numtypes: शेष:
      unreachable ();
    }

  वापस mask;
}

अटल पूर्णांक
do_statx (पूर्णांक fd, अक्षर स्थिर *name, काष्ठा stat *st, पूर्णांक flags,
          अचिन्हित पूर्णांक mask)
{
  काष्ठा statx stx;
  bool want_btime = mask & STATX_BTIME;
  पूर्णांक ret = statx (fd, name, flags | AT_NO_AUTOMOUNT, mask, &stx);
  अगर (ret >= 0)
    {
      statx_to_stat (&stx, st);
      /* Since we only need one timestamp type,
         store birth time in st_mtim.  */
      अगर (want_btime)
        {
          अगर (stx.stx_mask & STATX_BTIME)
            st->st_mtim = statx_timestamp_to_timespec (stx.stx_btime);
          अन्यथा
            st->st_mtim.tv_sec = st->st_mtim.tv_nsec = -1;
        }
    }

  वापस ret;
}

अटल पूर्णांक
do_stat (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस do_statx (AT_FDCWD, name, st, 0, calc_req_mask ());
}

अटल पूर्णांक
do_lstat (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस do_statx (AT_FDCWD, name, st, AT_SYMLINK_NOFOLLOW, calc_req_mask ());
}

अटल पूर्णांक
stat_for_mode (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस do_statx (AT_FDCWD, name, st, 0, STATX_MODE);
}

/* dev+ino should be static, so no need to sync with backing store */
अटल पूर्णांक
stat_for_ino (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस do_statx (AT_FDCWD, name, st, 0, STATX_INO);
}

अटल पूर्णांक
fstat_for_ino (पूर्णांक fd, काष्ठा stat *st)
{
  वापस do_statx (fd, "", st, AT_EMPTY_PATH, STATX_INO);
}
#अन्यथा
अटल पूर्णांक
do_stat (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस stat (name, st);
}

अटल पूर्णांक
do_lstat (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस lstat (name, st);
}

अटल पूर्णांक
stat_for_mode (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस stat (name, st);
}

अटल पूर्णांक
stat_for_ino (अक्षर स्थिर *name, काष्ठा stat *st)
{
  वापस stat (name, st);
}

अटल पूर्णांक
fstat_for_ino (पूर्णांक fd, काष्ठा stat *st)
{
  वापस ख_स्थिति (fd, st);
}
#पूर्ण_अगर

/* Return the address of the first plain %b spec in FMT, or NULL if
   there is no such spec.  %5b etc. do not match, so that user
   widths/flags are honored.  */

ATTRIBUTE_PURE
अटल अक्षर स्थिर *
first_percent_b (अक्षर स्थिर *fmt)
{
  क्रम (; *fmt; fmt++)
    अगर (fmt[0] == '%')
      चयन (fmt[1])
        {
        हाल 'b': वापस fmt;
        हाल '%': fmt++; अवरोध;
        }
  वापस शून्य;
}

अटल अक्षर RFC3986[256];
अटल व्योम
file_escape_init (व्योम)
{
  क्रम (पूर्णांक i = 0; i < 256; i++)
    RFC3986[i] |= c_isalnum (i) || i == '~' || i == '-' || i == '.' || i == '_';
}

क्रमागत { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Read the abbreviated month names from the locale, to align them
   and to determine the max width of the field and to truncate names
   greater than our max allowed.
   Note even though this handles multibyte locales correctly
   it's not restricted to them as single byte locales can have
   variable width abbreviated months and also precomputing/caching
   the names was seen to increase the performance of ls significantly.  */

/* abformat[RECENT][MON] is the format to use for timestamps with
   recentness RECENT and month MON.  */
क्रमागत { ABFORMAT_आकार = 128 };
अटल अक्षर abformat[2][12][ABFORMAT_आकार];
/* True if precomputed formats should be used.  This can be false if
   nl_langinfo fails, if a format or month abbreviation is unusually
   long, or if a month abbreviation contains '%'.  */
अटल bool use_abformat;

/* Store into ABMON the abbreviated month names, suitably aligned.
   Return true if successful.  */

अटल bool
abmon_init (अक्षर abmon[12][ABFORMAT_आकार])
{
#अगर_अघोषित HAVE_NL_LANGINFO
  वापस false;
#अन्यथा
  पूर्णांक max_mon_width = 0;
  पूर्णांक mon_width[12];
  पूर्णांक mon_len[12];

  क्रम (पूर्णांक i = 0; i < 12; i++)
    {
      अक्षर स्थिर *abbr = nl_langinfo (ABMON_1 + i);
      mon_len[i] = strnlen (abbr, ABFORMAT_आकार);
      अगर (mon_len[i] == ABFORMAT_आकार)
        वापस false;
      अगर (म_अक्षर (abbr, '%'))
        वापस false;
      mon_width[i] = mbswidth (म_नकल (abmon[i], abbr), MBSWIDTH_FLAGS);
      अगर (mon_width[i] < 0)
        वापस false;
      max_mon_width = MAX (max_mon_width, mon_width[i]);
    }

  क्रम (पूर्णांक i = 0; i < 12; i++)
    {
      पूर्णांक fill = max_mon_width - mon_width[i];
      अगर (ABFORMAT_आकार - mon_len[i] <= fill)
        वापस false;
      bool align_बायाँ = !c_isdigit (abmon[i][0]);
      पूर्णांक fill_offset;
      अगर (align_बायाँ)
        fill_offset = mon_len[i];
      अन्यथा
        {
          स_हटाओ (abmon[i] + fill, abmon[i], mon_len[i]);
          fill_offset = 0;
        }
      स_रखो (abmon[i] + fill_offset, ' ', fill);
      abmon[i][mon_len[i] + fill] = '\0';
    }

  वापस true;
#पूर्ण_अगर
}

/* Initialize ABFORMAT and USE_ABFORMAT.  */

अटल व्योम
abformat_init (व्योम)
{
  अक्षर स्थिर *pb[2];
  क्रम (पूर्णांक recent = 0; recent < 2; recent++)
    pb[recent] = first_percent_b (long_time_format[recent]);
  अगर (! (pb[0] || pb[1]))
    वापस;

  अक्षर abmon[12][ABFORMAT_आकार];
  अगर (! abmon_init (abmon))
    वापस;

  क्रम (पूर्णांक recent = 0; recent < 2; recent++)
    {
      अक्षर स्थिर *fmt = long_time_format[recent];
      क्रम (पूर्णांक i = 0; i < 12; i++)
        {
          अक्षर *nfmt = abformat[recent][i];
          पूर्णांक nbytes;

          अगर (! pb[recent])
            nbytes = snprintf (nfmt, ABFORMAT_आकार, "%s", fmt);
          अन्यथा
            {
              अगर (! (pb[recent] - fmt <= MIN (ABFORMAT_आकार, पूर्णांक_उच्च)))
                वापस;
              पूर्णांक prefix_len = pb[recent] - fmt;
              nbytes = snprintf (nfmt, ABFORMAT_आकार, "%.*s%s%s",
                                 prefix_len, fmt, abmon[i], pb[recent] + 2);
            }

          अगर (! (0 <= nbytes && nbytes < ABFORMAT_आकार))
            वापस;
        }
    }

  use_abformat = true;
}

अटल माप_प्रकार
dev_ino_hash (व्योम स्थिर *x, माप_प्रकार table_आकार)
{
  काष्ठा dev_ino स्थिर *p = x;
  वापस (uintmax_t) {p->st_ino} % table_आकार;
}

अटल bool
dev_ino_compare (व्योम स्थिर *x, व्योम स्थिर *y)
{
  काष्ठा dev_ino स्थिर *a = x;
  काष्ठा dev_ino स्थिर *b = y;
  वापस PSAME_INODE (a, b);
}

अटल व्योम
dev_ino_free (व्योम *x)
{
  मुक्त (x);
}

/* Add the device/inode pair (P->st_dev/P->st_ino) to the set of
   active directories.  Return true if there is already a matching
   entry in the table.  */

अटल bool
visit_dir (dev_t dev, ino_t ino)
{
  काष्ठा dev_ino *ent;
  काष्ठा dev_ino *ent_from_table;
  bool found_match;

  ent = xmalloc (माप *ent);
  ent->st_ino = ino;
  ent->st_dev = dev;

  /* Attempt to insert this entry into the table.  */
  ent_from_table = hash_जोड़ो (active_dir_set, ent);

  अगर (ent_from_table == शून्य)
    {
      /* Insertion failed due to lack of memory.  */
      xalloc_die ();
    }

  found_match = (ent_from_table != ent);

  अगर (found_match)
    {
      /* ent was not inserted, so free it.  */
      मुक्त (ent);
    }

  वापस found_match;
}

अटल व्योम
free_pending_ent (काष्ठा pending *p)
{
  मुक्त (p->name);
  मुक्त (p->realname);
  मुक्त (p);
}

अटल bool
is_colored (क्रमागत indicator_no type)
{
  /* Return true unless the string is "", "0" or "00"; try to be efficient.  */
  माप_प्रकार len = color_indicator[type].len;
  अगर (len == 0)
    वापस false;
  अगर (2 < len)
    वापस true;
  अक्षर स्थिर *s = color_indicator[type].string;
  वापस (s[0] != '0') | (s[len - 1] != '0');
}

अटल व्योम
restore_default_color (व्योम)
{
  put_indicator (&color_indicator[C_बायाँ]);
  put_indicator (&color_indicator[C_दायाँ]);
}

अटल व्योम
set_normal_color (व्योम)
{
  अगर (print_with_color && is_colored (C_NORM))
    {
      put_indicator (&color_indicator[C_बायाँ]);
      put_indicator (&color_indicator[C_NORM]);
      put_indicator (&color_indicator[C_दायाँ]);
    }
}

/* An ordinary signal was received; arrange for the program to exit.  */

अटल व्योम
sighandler (पूर्णांक sig)
{
  अगर (! interrupt_signal)
    interrupt_signal = sig;
}

/* A SIGTSTP was received; arrange for the program to suspend itself.  */

अटल व्योम
stophandler (MAYBE_UNUSED पूर्णांक sig)
{
  अगर (! interrupt_signal)
    stop_signal_गिनती++;
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.
   Signal handling can restore the default colors, so callers must
   immediately change colors after invoking this function.  */

अटल व्योम
process_signals (व्योम)
{
  जबतक (interrupt_signal || stop_signal_गिनती)
    {
      पूर्णांक sig;
      पूर्णांक stops;
      sigset_t oldset;

      अगर (used_color)
        restore_default_color ();
      ख_साफ (मानक_निकास);

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and stop_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      sig = interrupt_signal;
      stops = stop_signal_गिनती;

      /* SIGTSTP is special, since the application can receive that signal
         more than once.  In this case, don't set the signal handler to the
         default.  Instead, just raise the uncatchable SIGSTOP.  */
      अगर (stops)
        {
          stop_signal_गिनती = stops - 1;
          sig = SIGSTOP;
        }
      अन्यथा
        संकेत (sig, संक_पूर्व);

      /* Exit or suspend the program.  */
      उठाओ (sig);
      sigprocmask (SIG_SETMASK, &oldset, शून्य);

      /* If execution reaches here, then the program has been
         continued (after being suspended).  */
    }
}

/* Setup signal handlers if INIT is true,
   otherwise restore to the default.  */

अटल व्योम
signal_setup (bool init)
{
  /* The signals that are trapped, and the number of such signals.  */
  अटल पूर्णांक स्थिर stop_sig[] =
    {
      /* This one is handled specially.  */
      SIGTSTP,
    };

  क्रमागत { nsigs = countof (term_sig), nstop = countof (stop_sig) };

  अगर (init)
    {
      काष्ठा sigaction act;

      sigemptyset (&caught_signals);
      क्रम (पूर्णांक j = 0; j < nsigs + nstop; j++)
        {
          पूर्णांक sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          sigaction (sig, शून्य, &act);
          अगर (act.sa_handler != संक_छोड़ो)
            sigaddset (&caught_signals, sig);
        }

      act.sa_mask = caught_signals;
      act.sa_flags = SA_RESTART;

      क्रम (पूर्णांक j = 0; j < nsigs + nstop; j++)
        {
          पूर्णांक sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          अगर (sigismember (&caught_signals, sig))
            {
              act.sa_handler = j < nsigs ? sighandler : stophandler;
              sigaction (sig, &act, शून्य);
            }
        }
    }
  अन्यथा /* restore.  */
    {
      क्रम (पूर्णांक j = 0; j < nsigs + nstop; j++)
        {
          पूर्णांक sig = j < nsigs ? term_sig[j]: stop_sig[j - nsigs];
          अगर (sigismember (&caught_signals, sig))
            संकेत (sig, संक_पूर्व);
        }
    }
}

अटल व्योम
signal_init (व्योम)
{
  signal_setup (true);
}

अटल व्योम
signal_restore (व्योम)
{
  signal_setup (false);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक i;
  काष्ठा pending *thispend;
  पूर्णांक n_files;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (LS_FAILURE);
  निकास_पर (close_stdout);

  static_assert (countof (color_indicator) == countof (indicator_name));

  exit_status = निकास_सफल;
  print_dir_name = true;
  pending_dirs = शून्य;

  current_time.tv_sec = TYPE_MINIMUM (समय_प्रकार);
  current_time.tv_nsec = -1;

  i = decode_switches (argc, argv);

  अगर (print_with_color)
    parse_ls_color ();

  /* Test print_with_color again, because the call to parse_ls_color
     may have just reset it -- e.g., if LS_COLORS is invalid.  */

  अगर (print_with_color)
    {
      /* Don't use TAB characters in output.  Some terminal
         emulators can't handle the combination of tabs and
         color codes on the same line.  */
      tabsize = 0;
    }

  अगर (directories_first)
    check_symlink_mode = true;
  अन्यथा अगर (print_with_color)
    {
      /* Avoid following symbolic links when possible.  */
      अगर (is_colored (C_ORPHAN)
          || (is_colored (C_EXEC) && color_symlink_as_referent)
          || (is_colored (C_MISSING) && format == long_format))
        check_symlink_mode = true;
    }

  अगर (dereference == DEREF_UNDEFINED)
    dereference = ((immediate_dirs
                    || indicator_style == classify
                    || format == long_format)
                   ? DEREF_NEVER
                   : DEREF_COMMAND_पंक्ति_SYMLINK_TO_DIR);

  /* When using -R, initialize a data structure we'll use to
     detect any directory cycles.  */
  अगर (recursive)
    {
      active_dir_set = hash_initialize (INITIAL_TABLE_आकार, शून्य,
                                        dev_ino_hash,
                                        dev_ino_compare,
                                        dev_ino_free);
      अगर (active_dir_set == शून्य)
        xalloc_die ();

      obstack_init (&dev_ino_obstack);
    }

  localtz = tzalloc (दो_पर्या ("TZ"));

  format_needs_stat = ((sort_type == sort_time) | (sort_type == sort_आकार)
                       | (format == long_format)
                       | print_block_आकार | print_hyperlink | print_scontext);
  format_needs_type = ((! format_needs_stat)
                       & (recursive | print_with_color | print_scontext
                          | directories_first
                          | (indicator_style != none)));
  format_needs_capability = print_with_color && is_colored (C_CAP);

  अगर (dired)
    {
      obstack_init (&dired_obstack);
      obstack_init (&subdired_obstack);
    }

  अगर (print_hyperlink)
    {
      file_escape_init ();

      hostname = xgethostname ();
      /* The hostname is generally ignored,
         so ignore failures obtaining it.  */
      अगर (! hostname)
        hostname = "";
    }

  cwd_n_alloc = 100;
  cwd_file = xmalloc (cwd_n_alloc * माप *cwd_file);
  cwd_n_used = 0;

  clear_files ();

  n_files = argc - i;

  अगर (n_files <= 0)
    {
      अगर (immediate_dirs)
        gobble_file (".", directory, NOT_AN_INODE_संख्या, true, शून्य);
      अन्यथा
        queue_directory (".", शून्य, true);
    }
  अन्यथा
    करो
      gobble_file (argv[i++], unknown, NOT_AN_INODE_संख्या, true, शून्य);
    जबतक (i < argc);

  अगर (cwd_n_used)
    {
      sort_files ();
      अगर (!immediate_dirs)
        extract_dirs_from_files (शून्य, true);
      /* 'cwd_n_used' might be zero now.  */
    }

  /* In the following if/else blocks, it is sufficient to test 'pending_dirs'
     (and not pending_dirs->name) because there may be no markers in the queue
     at this point.  A marker may be enqueued when extract_dirs_from_files is
     called with a non-empty string or via print_dir.  */
  अगर (cwd_n_used)
    {
      print_current_files ();
      अगर (pending_dirs)
        dired_outbyte ('\n');
    }
  अन्यथा अगर (n_files <= 1 && pending_dirs && !pending_dirs->अग्र)
    print_dir_name = false;

  जबतक (pending_dirs)
    {
      thispend = pending_dirs;
      pending_dirs = pending_dirs->अग्र;

      अगर (LOOP_DETECT)
        {
          अगर (thispend->name == शून्य)
            {
              /* thispend->name == NULL means this is a marker entry
                 indicating we've finished processing the directory.
                 Use its dev/ino numbers to remove the corresponding
                 entry from the active_dir_set hash table.  */
              काष्ठा dev_ino di = dev_ino_निकालो ();
              काष्ठा dev_ino *found = hash_remove (active_dir_set, &di);
              अगर (false)
                assert_matching_dev_ino (thispend->realname, di);
              affirm (found);
              dev_ino_free (found);
              free_pending_ent (thispend);
              जारी;
            }
        }

      print_dir (thispend->name, thispend->realname,
                 thispend->command_पंक्ति_arg);

      free_pending_ent (thispend);
      print_dir_name = true;
    }

  अगर (print_with_color && used_color)
    {

      /* Skip the restore when it would be a no-op, i.e.,
         when left is "\033[" and right is "m".  */
      अगर (!(color_indicator[C_बायाँ].len == 2
            && memeq (color_indicator[C_बायाँ].string, "\033[", 2)
            && color_indicator[C_दायाँ].len == 1
            && color_indicator[C_दायाँ].string[0] == 'm'))
        restore_default_color ();

      ख_साफ (मानक_निकास);

      signal_restore ();

      /* Act on any signals that arrived before the default was restored.
         This can process signals out of order, but there doesn't seem to
         be an easy way to do them in order, and the order isn't that
         important anyway.  */
      क्रम (पूर्णांक j = stop_signal_गिनती; j; j--)
        उठाओ (SIGSTOP);
      पूर्णांक j = interrupt_signal;
      अगर (j)
        उठाओ (j);
    }

  अगर (dired)
    {
      /* No need to free these since we're about to exit.  */
      dired_dump_obstack ("//DIRED//", &dired_obstack);
      dired_dump_obstack ("//SUBDIRED//", &subdired_obstack);
      म_लिखो ("//DIRED-OPTIONS// --quoting-style=%s\n",
              quoting_style_args[get_quoting_style (filename_quoting_options)]);
    }

  अगर (LOOP_DETECT)
    {
      assure (hash_get_n_entries (active_dir_set) == 0);
      hash_free (active_dir_set);
    }

  वापस exit_status;
}

/* Return the line length indicated by the value given by SPEC, or -1
   if unsuccessful.  0 means no limit on line length.  */

अटल सूचक_भेद_प्रकार
decode_पंक्ति_लंबाई (अक्षर स्थिर *spec)
{
  uintmax_t val;

  /* Treat too-large values as if they were 0, which is
     effectively infinity.  */
  चयन (xstrtoumax (spec, शून्य, 0, &val, ""))
    {
    हाल LONGINT_OK:
      वापस val <= MIN (PTRDIFF_MAX, SIZE_MAX) ? val : 0;

    हाल LONGINT_OVERFLOW:
      वापस 0;

    हाल LONGINT_INVALID:
    हाल LONGINT_INVALID_SUFFIX_CHAR:
    हाल LONGINT_INVALID_SUFFIX_CHAR_WITH_OVERFLOW:
      वापस -1;

    शेष:
      unreachable ();
    }
}

/* Return true if standard output is a tty, caching the result.  */

अटल bool
stdout_isatty (व्योम)
{
  अटल चिन्हित अक्षर out_tty = -1;
  अगर (out_tty < 0)
    out_tty = isatty (STDOUT_FILENO);
  assume (out_tty == 0 || out_tty == 1);
  वापस out_tty;
}

/* Set all the option flags according to the switches specified.
   Return the index of the first non-option argument.  */

अटल पूर्णांक
decode_switches (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *time_style_option = शून्य;

  /* These variables are false or -1 unless a switch says otherwise.  */
  bool kibibytes_specified = false;
  पूर्णांक format_opt = -1;
  पूर्णांक hide_control_chars_opt = -1;
  पूर्णांक quoting_style_opt = -1;
  पूर्णांक sort_opt = -1;
  सूचक_भेद_प्रकार tabsize_opt = -1;
  सूचक_भेद_प्रकार width_opt = -1;

  जबतक (true)
    {
      पूर्णांक oi = -1;
      पूर्णांक c = getopt_long (argc, argv,
                           "abcdfghiklmnopqrstuvw:xABCDFGHI:LNQRST:UXZ1",
                           long_options, &oi);
      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'a':
          ignore_mode = IGNORE_MINIMAL;
          अवरोध;

        हाल 'b':
          quoting_style_opt = escape_quoting_style;
          अवरोध;

        हाल 'c':
          time_type = time_ctime;
          explicit_time = true;
          अवरोध;

        हाल 'd':
          immediate_dirs = true;
          अवरोध;

        हाल 'f':
          ignore_mode = IGNORE_MINIMAL; /* enable -a */
          sort_opt = sort_none;         /* enable -U */
          अवरोध;

        हाल FILE_TYPE_INDICATOR_OPTION: /* --file-type */
          indicator_style = file_type;
          अवरोध;

        हाल 'g':
          format_opt = long_format;
          print_owner = false;
          अवरोध;

        हाल 'h':
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI | human_base_1024;
          file_output_block_आकार = output_block_आकार = 1;
          अवरोध;

        हाल 'i':
          print_inode = true;
          अवरोध;

        हाल 'k':
          kibibytes_specified = true;
          अवरोध;

        हाल 'l':
          format_opt = long_format;
          अवरोध;

        हाल 'm':
          format_opt = with_commas;
          अवरोध;

        हाल 'n':
          numeric_ids = true;
          format_opt = long_format;
          अवरोध;

        हाल 'o':  /* Just like -l, but don't display group info.  */
          format_opt = long_format;
          print_group = false;
          अवरोध;

        हाल 'p':
          indicator_style = slash;
          अवरोध;

        हाल 'q':
          hide_control_chars_opt = true;
          अवरोध;

        हाल 'r':
          sort_reverse = true;
          अवरोध;

        हाल 's':
          print_block_आकार = true;
          अवरोध;

        हाल 't':
          sort_opt = sort_time;
          अवरोध;

        हाल 'u':
          time_type = time_atime;
          explicit_time = true;
          अवरोध;

        हाल 'v':
          sort_opt = sort_version;
          अवरोध;

        हाल 'w':
          width_opt = decode_पंक्ति_लंबाई (optarg);
          अगर (width_opt < 0)
            error (LS_FAILURE, 0, "%s: %s", _("invalid line width"),
                   quote (optarg));
          अवरोध;

        हाल 'x':
          format_opt = horizontal;
          अवरोध;

        हाल 'A':
          ignore_mode = IGNORE_DOT_AND_DOTDOT;
          अवरोध;

        हाल 'B':
          add_ignore_pattern ("*~");
          add_ignore_pattern (".*~");
          अवरोध;

        हाल 'C':
          format_opt = many_per_पंक्ति;
          अवरोध;

        हाल 'D':
          format_opt = long_format;
          print_hyperlink = false;
          dired = true;
          अवरोध;

        हाल 'F':
          {
            पूर्णांक i;
            अगर (optarg)
              i = XARGMATCH ("--classify", optarg, when_args, when_types);
            अन्यथा
              /* Using --classify with no argument is equivalent to using
                 --classify=always.  */
              i = when_always;

            अगर (i == when_always || (i == when_if_tty && stdout_isatty ()))
              indicator_style = classify;
            अवरोध;
          }

        हाल 'G':		/* inhibit display of group info */
          print_group = false;
          अवरोध;

        हाल 'H':
          dereference = DEREF_COMMAND_पंक्ति_ARGUMENTS;
          अवरोध;

        हाल DEREFERENCE_COMMAND_पंक्ति_SYMLINK_TO_DIR_OPTION:
          dereference = DEREF_COMMAND_पंक्ति_SYMLINK_TO_DIR;
          अवरोध;

        हाल 'I':
          add_ignore_pattern (optarg);
          अवरोध;

        हाल 'L':
          dereference = DEREF_ALWAYS;
          अवरोध;

        हाल 'N':
          quoting_style_opt = literal_quoting_style;
          अवरोध;

        हाल 'Q':
          quoting_style_opt = c_quoting_style;
          अवरोध;

        हाल 'R':
          recursive = true;
          अवरोध;

        हाल 'S':
          sort_opt = sort_आकार;
          अवरोध;

        हाल 'T':
          tabsize_opt = xnumtoumax (optarg, 0, 0, MIN (PTRDIFF_MAX, SIZE_MAX),
                                    "", _("invalid tab size"), LS_FAILURE, 0);
          अवरोध;

        हाल 'U':
          sort_opt = sort_none;
          अवरोध;

        हाल 'X':
          sort_opt = sort_extension;
          अवरोध;

        हाल '1':
          /* -1 has no effect after -l.  */
          अगर (format_opt != long_format)
            format_opt = one_per_पंक्ति;
          अवरोध;

        हाल AUTHOR_OPTION:
          print_author = true;
          अवरोध;

        हाल HIDE_OPTION:
          {
            काष्ठा ignore_pattern *hide = xmalloc (माप *hide);
            hide->pattern = optarg;
            hide->अग्र = hide_patterns;
            hide_patterns = hide;
          }
          अवरोध;

        हाल SORT_OPTION:
          sort_opt = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          अवरोध;

        हाल GROUP_DIRECTORIES_FIRST_OPTION:
          directories_first = true;
          अवरोध;

        हाल TIME_OPTION:
          time_type = XARGMATCH ("--time", optarg, time_args, time_types);
          explicit_time = true;
          अवरोध;

        हाल FORMAT_OPTION:
          format_opt = XARGMATCH ("--format", optarg, format_args,
                                  format_types);
          अवरोध;

        हाल FULL_TIME_OPTION:
          format_opt = long_format;
          time_style_option = (अक्षर *) "full-iso";
          अवरोध;

        हाल COLOR_OPTION:
          {
            पूर्णांक i;
            अगर (optarg)
              i = XARGMATCH ("--color", optarg, when_args, when_types);
            अन्यथा
              /* Using --color with no argument is equivalent to using
                 --color=always.  */
              i = when_always;

            print_with_color = (i == when_always
                                || (i == when_if_tty && stdout_isatty ()));
            अवरोध;
          }

        हाल HYPERLINK_OPTION:
          {
            पूर्णांक i;
            अगर (optarg)
              i = XARGMATCH ("--hyperlink", optarg, when_args, when_types);
            अन्यथा
              /* Using --hyperlink with no argument is equivalent to using
                 --hyperlink=always.  */
              i = when_always;

            print_hyperlink = (i == when_always
                               || (i == when_if_tty && stdout_isatty ()));
            अवरोध;
          }

        हाल INDICATOR_STYLE_OPTION:
          indicator_style = XARGMATCH ("--indicator-style", optarg,
                                       indicator_style_args,
                                       indicator_style_types);
          अवरोध;

        हाल QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          अवरोध;

        हाल TIME_STYLE_OPTION:
          time_style_option = optarg;
          अवरोध;

        हाल SHOW_CONTROL_CHARS_OPTION:
          hide_control_chars_opt = false;
          अवरोध;

        हाल BLOCK_आकार_OPTION:
          {
            क्रमागत strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_आकार);
            अगर (e != LONGINT_OK)
              xstrtol_fatal (e, oi, 0, long_options, optarg);
            file_human_output_opts = human_output_opts;
            file_output_block_आकार = output_block_आकार;
          }
          अवरोध;

        हाल SI_OPTION:
          file_human_output_opts = human_output_opts =
            human_autoscale | human_SI;
          file_output_block_आकार = output_block_आकार = 1;
          अवरोध;

        हाल 'Z':
          print_scontext = true;
          अवरोध;

        हाल ZERO_OPTION:
          eolbyte = 0;
          hide_control_chars_opt = false;
          अगर (format_opt != long_format)
            format_opt = one_per_पंक्ति;
          print_with_color = false;
          quoting_style_opt = literal_quoting_style;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (LS_FAILURE);
        }
    }

  अगर (! output_block_आकार)
    {
      अक्षर स्थिर *ls_block_आकार = दो_पर्या ("LS_BLOCK_SIZE");
      human_options (ls_block_आकार,
                     &human_output_opts, &output_block_आकार);
      अगर (ls_block_आकार || दो_पर्या ("BLOCK_SIZE"))
        {
          file_human_output_opts = human_output_opts;
          file_output_block_आकार = output_block_आकार;
        }
      अगर (kibibytes_specified)
        {
          human_output_opts = 0;
          output_block_आकार = 1024;
        }
    }

  format = (0 <= format_opt ? format_opt
            : ls_mode == LS_LS ? (stdout_isatty ()
                                  ? many_per_पंक्ति : one_per_पंक्ति)
            : ls_mode == LS_MULTI_COL ? many_per_पंक्ति
            : /* ls_mode == LS_LONG_FORMAT */ long_format);

  /* If the line length was not set by a switch but is needed to determine
     output, go to the work of obtaining it from the environment.  */
  सूचक_भेद_प्रकार linelen = width_opt;
  अगर (format == many_per_पंक्ति || format == horizontal || format == with_commas
      || print_with_color)
    {
#अगर_घोषित TIOCGWINSZ
      अगर (linelen < 0)
        {
          काष्ठा winsize ws;
          अगर (stdout_isatty ()
              && 0 <= ioctl (STDOUT_FILENO, TIOCGWINSZ, &ws)
              && 0 < ws.ws_col)
            linelen = ws.ws_col <= MIN (PTRDIFF_MAX, SIZE_MAX) ? ws.ws_col : 0;
        }
#पूर्ण_अगर
      अगर (linelen < 0)
        {
          अक्षर स्थिर *p = दो_पर्या ("COLUMNS");
          अगर (p && *p)
            {
              linelen = decode_पंक्ति_लंबाई (p);
              अगर (linelen < 0)
                error (0, 0,
                       _("ignoring invalid width"
                         " in environment variable COLUMNS: %s"),
                       quote (p));
            }
        }
    }

  पंक्ति_लंबाई = linelen < 0 ? 80 : linelen;

  /* Determine the max possible number of display columns.  */
  max_idx = पंक्ति_लंबाई / MIN_COLUMN_WIDTH;
  /* Account for first display column not having a separator,
     or line_lengths shorter than MIN_COLUMN_WIDTH.  */
  max_idx += पंक्ति_लंबाई % MIN_COLUMN_WIDTH != 0;

  अगर (format == many_per_पंक्ति || format == horizontal || format == with_commas)
    {
      अगर (0 <= tabsize_opt)
        tabsize = tabsize_opt;
      अन्यथा
        {
          tabsize = 8;
          अक्षर स्थिर *p = दो_पर्या ("TABSIZE");
          अगर (p)
            {
              uintmax_t tmp;
              अगर (xstrtoumax (p, शून्य, 0, &tmp, "") == LONGINT_OK
                  && tmp <= SIZE_MAX)
                tabsize = tmp;
              अन्यथा
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
                  && ((पंक्ति_लंबाई
                       && (format == with_commas
                           || format == many_per_पंक्ति || format == horizontal))
                      || format == one_per_पंक्ति || format == long_format);

  पूर्णांक qs = quoting_style_opt;
  अगर (qs < 0)
    qs = getenv_quoting_style ();
  अगर (qs < 0)
    qs = (ls_mode == LS_LS
          ? (stdout_isatty () ? shell_escape_quoting_style : -1)
          : escape_quoting_style);
  अगर (0 <= qs)
    set_quoting_style (शून्य, qs);
  qs = get_quoting_style (शून्य);
  align_variable_outer_quotes
    = ((format == long_format
        || ((format == many_per_पंक्ति || format == horizontal) && पंक्ति_लंबाई))
       && (qs == shell_quoting_style
           || qs == shell_escape_quoting_style
           || qs == c_maybe_quoting_style));
  filename_quoting_options = clone_quoting_options (शून्य);
  अगर (qs == escape_quoting_style)
    set_char_quoting (filename_quoting_options, ' ', 1);
  अगर (file_type <= indicator_style)
    {
      क्रम (अक्षर स्थिर *p = &"*=>@|"[indicator_style - file_type]; *p; p++)
        set_char_quoting (filename_quoting_options, *p, 1);
    }
  अगर (format == with_commas
      && (qs == shell_quoting_style
          || qs == shell_escape_quoting_style
          || qs == c_maybe_quoting_style
          || qs == escape_quoting_style))
    set_char_quoting (filename_quoting_options, ',', 1);

  dirname_quoting_options = clone_quoting_options (शून्य);
  set_char_quoting (dirname_quoting_options, ':', 1);

  /* --dired implies --format=long (-l) and sans --hyperlink.
     So ignore it if those overridden.  */
  dired &= (format == long_format) & !print_hyperlink;

  अगर (eolbyte < dired)
    error (LS_FAILURE, 0, _("--dired and --zero are incompatible"));

  /* If a time type is explicitly specified (with -c, -u, or --time=)
     and we're not showing a time (-l not specified), then sort by that time,
     rather than by name.  Note this behavior is unspecified by POSIX.  */

  sort_type = (0 <= sort_opt ? sort_opt
               : (format != long_format && explicit_time)
               ? sort_time : sort_name);

  अगर (format == long_format)
    {
      अक्षर *envstyle = शून्य;
      अक्षर *style = time_style_option;
      अगर (! style)
        style = envstyle = दो_पर्या ("TIME_STYLE");

      अगर (! style)
        style = (अक्षर *) "locale";
      अन्यथा
        {
          अटल अक्षर स्थिर posix_prefix[] = "posix-";
          जबतक (STREQ_LEN (style, posix_prefix, माप posix_prefix - 1))
            {
              अगर (! hard_locale (LC_TIME))
                वापस optind;
              style += माप posix_prefix - 1;
            }
        }

      अगर (*style == '+')
        {
          अक्षर *p0 = style + 1;
          अक्षर *p0nl = म_अक्षर (p0, '\n');
          अक्षर स्थिर *p1 = p0;
          अगर (p0nl)
            {
              p1 = p0nl + 1;
              अगर (म_अक्षर (p1, '\n'))
                error (LS_FAILURE, 0, _("invalid time style format %s"),
                       quote (p0));
              अगर (envstyle)
                p0 = xmemdup0 (p0, p0nl - p0);
              अन्यथा
                *p0nl = '\0';
            }
          long_time_format[0] = p0;
          long_time_format[1] = p1;
        }
      अन्यथा
        {
          चयन (x_timestyle_match (style, /*allow_posix=*/ true,
                                     time_style_args,
                                     (अक्षर स्थिर *) time_style_types,
                                     माप (*time_style_types), LS_FAILURE))
            {
            हाल full_iso_time_style:
              long_time_format[0] = long_time_format[1] =
                "%Y-%m-%d %H:%M:%S.%N %z";
              अवरोध;

            हाल long_iso_time_style:
              long_time_format[0] = long_time_format[1] = "%Y-%m-%d %H:%M";
              अवरोध;

            हाल iso_time_style:
              long_time_format[0] = "%Y-%m-%d ";
              long_time_format[1] = "%m-%d %H:%M";
              अवरोध;

            हाल locale_time_style:
              अगर (hard_locale (LC_TIME))
                {
                  क्रम (पूर्णांक i = 0; i < 2; i++)
                    long_time_format[i] =
                      dcgettext (शून्य, long_time_format[i], LC_TIME);
                }
            }
        }

      abformat_init ();
    }

  वापस optind;
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

अटल bool
get_funky_string (अक्षर **dest, अक्षर स्थिर **src, bool equals_end,
                  माप_प्रकार *output_गिनती)
{
  अक्षर अंक;			/* For numerical codes */
  माप_प्रकार गिनती;			/* Something to count with */
  क्रमागत {
    ST_GND, ST_BACKSLASH, ST_OCTAL, ST_HEX, ST_CARET, ST_END, ST_ERROR
  } state;
  अक्षर स्थिर *p;
  अक्षर *q;

  p = *src;			/* We don't want to double-indirect */
  q = *dest;			/* the whole darn time.  */

  गिनती = 0;			/* No characters counted in yet.  */
  अंक = 0;

  state = ST_GND;		/* Start in ground state.  */
  जबतक (state < ST_END)
    {
      चयन (state)
        {
        हाल ST_GND:		/* Ground state (no escapes) */
          चयन (*p)
            {
            हाल ':':
            हाल '\0':
              state = ST_END;	/* End of string */
              अवरोध;
            हाल '\\':
              state = ST_BACKSLASH; /* Backslash escape sequence */
              ++p;
              अवरोध;
            हाल '^':
              state = ST_CARET; /* Caret escape */
              ++p;
              अवरोध;
            हाल '=':
              अगर (equals_end)
                {
                  state = ST_END; /* End */
                  अवरोध;
                }
              FALLTHROUGH;
            शेष:
              *(q++) = *(p++);
              ++गिनती;
              अवरोध;
            }
          अवरोध;

        हाल ST_BACKSLASH:	/* Backslash escaped character */
          चयन (*p)
            {
            हाल '0':
            हाल '1':
            हाल '2':
            हाल '3':
            हाल '4':
            हाल '5':
            हाल '6':
            हाल '7':
              state = ST_OCTAL;	/* Octal sequence */
              अंक = *p - '0';
              अवरोध;
            हाल 'x':
            हाल 'X':
              state = ST_HEX;	/* Hex sequence */
              अंक = 0;
              अवरोध;
            हाल 'a':		/* Bell */
              अंक = '\a';
              अवरोध;
            हाल 'b':		/* Backspace */
              अंक = '\b';
              अवरोध;
            हाल 'e':		/* Escape */
              अंक = 27;
              अवरोध;
            हाल 'f':		/* Form feed */
              अंक = '\f';
              अवरोध;
            हाल 'n':		/* Newline */
              अंक = '\n';
              अवरोध;
            हाल 'r':		/* Carriage return */
              अंक = '\r';
              अवरोध;
            हाल 't':		/* Tab */
              अंक = '\t';
              अवरोध;
            हाल 'v':		/* Vtab */
              अंक = '\v';
              अवरोध;
            हाल '?':		/* Delete */
              अंक = 127;
              अवरोध;
            हाल '_':		/* Space */
              अंक = ' ';
              अवरोध;
            हाल '\0':		/* End of string */
              state = ST_ERROR;	/* Error! */
              अवरोध;
            शेष:		/* Escaped character like \ ^ : = */
              अंक = *p;
              अवरोध;
            }
          अगर (state == ST_BACKSLASH)
            {
              *(q++) = अंक;
              ++गिनती;
              state = ST_GND;
            }
          ++p;
          अवरोध;

        हाल ST_OCTAL:		/* Octal sequence */
          अगर (*p < '0' || *p > '7')
            {
              *(q++) = अंक;
              ++गिनती;
              state = ST_GND;
            }
          अन्यथा
            अंक = (अंक << 3) + (*(p++) - '0');
          अवरोध;

        हाल ST_HEX:		/* Hex sequence */
          चयन (*p)
            {
            हाल '0':
            हाल '1':
            हाल '2':
            हाल '3':
            हाल '4':
            हाल '5':
            हाल '6':
            हाल '7':
            हाल '8':
            हाल '9':
              अंक = (अंक << 4) + (*(p++) - '0');
              अवरोध;
            हाल 'a':
            हाल 'b':
            हाल 'c':
            हाल 'd':
            हाल 'e':
            हाल 'f':
              अंक = (अंक << 4) + (*(p++) - 'a') + 10;
              अवरोध;
            हाल 'A':
            हाल 'B':
            हाल 'C':
            हाल 'D':
            हाल 'E':
            हाल 'F':
              अंक = (अंक << 4) + (*(p++) - 'A') + 10;
              अवरोध;
            शेष:
              *(q++) = अंक;
              ++गिनती;
              state = ST_GND;
              अवरोध;
            }
          अवरोध;

        हाल ST_CARET:		/* Caret escape */
          state = ST_GND;	/* Should be the next state... */
          अगर (*p >= '@' && *p <= '~')
            {
              *(q++) = *(p++) & 037;
              ++गिनती;
            }
          अन्यथा अगर (*p == '?')
            {
              *(q++) = 127;
              ++गिनती;
            }
          अन्यथा
            state = ST_ERROR;
          अवरोध;

        हाल ST_END: हाल ST_ERROR: शेष:
          affirm (false);
        }
    }

  *dest = q;
  *src = p;
  *output_गिनती = गिनती;

  वापस state != ST_ERROR;
}

क्रमागत parse_state
  {
    PS_START = 1,
    PS_2,
    PS_3,
    PS_4,
    PS_DONE,
    PS_FAIL
  };


/* Check if the content of TERM is a valid name in dircolors.  */

अटल bool
known_term_type (व्योम)
{
  अक्षर स्थिर *term = दो_पर्या ("TERM");
  अगर (! term || ! *term)
    वापस false;

  अक्षर स्थिर *पंक्ति = G_पंक्ति;
  जबतक (पंक्ति - G_पंक्ति < माप (G_पंक्ति))
    {
      अगर (STRNCMP_LIT (पंक्ति, "TERM ") == 0)
        {
          अगर (fnmatch (पंक्ति + 5, term, 0) == 0)
            वापस true;
        }
      पंक्ति += म_माप (पंक्ति) + 1;
    }

  वापस false;
}

अटल व्योम
parse_ls_color (व्योम)
{
  अक्षर स्थिर *p;		/* Pointer to character being parsed */
  अक्षर *buf;			/* color_buf buffer pointer */
  अक्षर label0, label1;		/* Indicator label */
  काष्ठा color_ext_type *ext;	/* Extension we are working on */

  अगर ((p = दो_पर्या ("LS_COLORS")) == शून्य || *p == '\0')
    {
      /* LS_COLORS takes precedence, but if that's not set then
         honor the COLORTERM and TERM env variables so that
         we only go with the internal ANSI color codes if the
         former is non empty or the latter is set to a known value.  */
      अक्षर स्थिर *colorterm = दो_पर्या ("COLORTERM");
      अगर (! (colorterm && *colorterm) && ! known_term_type ())
        print_with_color = false;
      वापस;
    }

  ext = शून्य;

  /* This is an overly conservative estimate, but any possible
     LS_COLORS string will *not* generate a color_buf longer than
     itself, so it is a safe way of allocating a buffer in
     advance.  */
  buf = color_buf = xstrdup (p);

  क्रमागत parse_state state = PS_START;
  जबतक (true)
    {
      चयन (state)
        {
        हाल PS_START:		/* First label character */
          चयन (*p)
            {
            हाल ':':
              ++p;
              अवरोध;

            हाल '*':
              /* Allocate new extension block and add to head of
                 linked list (this way a later definition will
                 override an earlier one, which can be useful for
                 having terminal-specific defs override global).  */

              ext = xmalloc (माप *ext);
              ext->अग्र = color_ext_list;
              color_ext_list = ext;
              ext->exact_match = false;

              ++p;
              ext->ext.string = buf;

              state = (get_funky_string (&buf, &p, true, &ext->ext.len)
                       ? PS_4 : PS_FAIL);
              अवरोध;

            हाल '\0':
              state = PS_DONE;	/* Done! */
              जाओ done;

            शेष:	/* Assume it is file type label */
              label0 = *p++;
              state = PS_2;
              अवरोध;
            }
          अवरोध;

        हाल PS_2:		/* Second label character */
          अगर (*p)
            {
              label1 = *p++;
              state = PS_3;
            }
          अन्यथा
            state = PS_FAIL;	/* Error */
          अवरोध;

        हाल PS_3:		/* Equal sign after indicator label */
          state = PS_FAIL;	/* Assume failure...  */
          अगर (*(p++) == '=')/* It *should* be...  */
            {
              क्रम (पूर्णांक i = 0; i < countof (indicator_name); i++)
                {
                  अगर ((label0 == indicator_name[i][0])
                      && (label1 == indicator_name[i][1]))
                    {
                      color_indicator[i].string = buf;
                      state = (get_funky_string (&buf, &p, false,
                                                 &color_indicator[i].len)
                               ? PS_START : PS_FAIL);
                      अवरोध;
                    }
                }
              अगर (state == PS_FAIL)
                error (0, 0, _("unrecognized prefix: %s"),
                       quote ((अक्षर []) {label0, label1, '\0'}));
            }
          अवरोध;

        हाल PS_4:		/* Equal sign after *.ext */
          अगर (*(p++) == '=')
            {
              ext->seq.string = buf;
              state = (get_funky_string (&buf, &p, false, &ext->seq.len)
                       ? PS_START : PS_FAIL);
            }
          अन्यथा
            state = PS_FAIL;
          अवरोध;

        हाल PS_FAIL:
          जाओ done;

        हाल PS_DONE: शेष:
          affirm (false);
        }
    }
 done:

  अगर (state == PS_FAIL)
    {
      error (0, 0,
             _("unparsable value for LS_COLORS environment variable"));
      मुक्त (color_buf);
      क्रम (काष्ठा color_ext_type *e = color_ext_list; e != शून्य;
           /* empty */)
        {
          काष्ठा color_ext_type *e2 = e;
          e = e->अग्र;
          मुक्त (e2);
        }
      print_with_color = false;
      वापस;
    }
  अन्यथा
    {
      /* Postprocess list to set EXACT_MATCH on entries where there are
         different cased extensions with separate sequences defined.
         Also set ext.len to SIZE_MAX on any entries that can't
         match due to precedence, to avoid redundant string compares.  */
      क्रम (काष्ठा color_ext_type *e1 = color_ext_list; e1 != शून्य;
           e1 = e1->अग्र)
        {
          bool case_ignored = false;

          क्रम (काष्ठा color_ext_type *e2 = e1->अग्र; e2 != शून्य;
               e2 = e2->अग्र)
            {
              अगर (e2->ext.len < SIZE_MAX && e1->ext.len == e2->ext.len)
                {
                  अगर (memeq (e1->ext.string, e2->ext.string, e1->ext.len))
                    e2->ext.len = SIZE_MAX; /* Ignore */
                  अन्यथा अगर (c_strncasecmp (e1->ext.string, e2->ext.string,
                                          e1->ext.len) == 0)
                    {
                      अगर (case_ignored)
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                        }
                      अन्यथा अगर (e1->seq.len == e2->seq.len
                               && memeq (e1->seq.string, e2->seq.string,
                                         e1->seq.len))
                        {
                          e2->ext.len = SIZE_MAX; /* Ignore */
                          case_ignored = true;    /* Ignore all subsequent */
                        }
                      अन्यथा
                        {
                          e1->exact_match = true;
                          e2->exact_match = true;
                        }
                    }
                }
            }
        }
    }

  अगर (color_indicator[C_LINK].len == 6
      && !STRNCMP_LIT (color_indicator[C_LINK].string, "target"))
    color_symlink_as_referent = true;
}

/* Set the exit status to report a failure.  If SERIOUS, it is a
   serious failure; otherwise, it is merely a minor problem.  */

अटल व्योम
set_exit_status (bool serious)
{
  अगर (serious)
    exit_status = LS_FAILURE;
  अन्यथा अगर (exit_status == निकास_सफल)
    exit_status = LS_MINOR_PROBLEM;
}

/* Assuming a failure is serious if SERIOUS, use the printf-style
   MESSAGE to report the failure to access a file named FILE.  Assume
   errno is set appropriately for the failure.  */

अटल व्योम
file_failure (bool serious, अक्षर स्थिर *message, अक्षर स्थिर *file)
{
  error (0, त्रुटि_सं, message, quoteaf (file));
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

अटल व्योम
queue_directory (अक्षर स्थिर *name, अक्षर स्थिर *realname, bool command_पंक्ति_arg)
{
  काष्ठा pending *new = xmalloc (माप *new);
  new->realname = realname ? xstrdup (realname) : शून्य;
  new->name = name ? xstrdup (name) : शून्य;
  new->command_पंक्ति_arg = command_पंक्ति_arg;
  new->अग्र = pending_dirs;
  pending_dirs = new;
}

/* Read directory NAME, and list the files in it.
   If REALNAME is nonzero, print its name instead of NAME;
   this is used for symbolic links to directories.
   COMMAND_LINE_ARG means this directory was mentioned on the command line.  */

अटल व्योम
print_dir (अक्षर स्थिर *name, अक्षर स्थिर *realname, bool command_पंक्ति_arg)
{
  सूची *dirp;
  काष्ठा dirent *अग्र;
  uintmax_t योग_blocks = 0;
  अटल bool first = true;

  त्रुटि_सं = 0;
  dirp = सूची_खोलो (name);
  अगर (!dirp)
    {
      अगर (command_पंक्ति_arg || ! ignorable_traversal_errno (त्रुटि_सं))
        file_failure (command_पंक्ति_arg, _("cannot open directory %s"), name);
      वापस;
    }

  अगर (LOOP_DETECT)
    {
      काष्ठा stat dir_stat;
      पूर्णांक fd = dirfd (dirp);

      /* If dirfd failed, endure the overhead of stat'ing by path  */
      अगर ((0 <= fd
           ? fstat_for_ino (fd, &dir_stat)
           : stat_for_ino (name, &dir_stat)) < 0)
        {
          अगर (! ignorable_traversal_errno (त्रुटि_सं))
            file_failure (command_पंक्ति_arg,
                          _("cannot determine device and inode of %s"), name);
          बंद_सूची (dirp);
          वापस;
        }

      /* If we've already visited this dev/inode pair, warn that
         we've found a loop, and do not process this directory.  */
      अगर (visit_dir (dir_stat.st_dev, dir_stat.st_ino))
        {
          error (0, 0, _("%s: not listing already-listed directory"),
                 quotef (name));
          बंद_सूची (dirp);
          set_exit_status (true);
          वापस;
        }

      dev_ino_धकेलो (dir_stat.st_dev, dir_stat.st_ino);
    }

  clear_files ();

  अगर (recursive || print_dir_name)
    {
      अगर (!first)
        dired_outbyte ('\n');
      first = false;
      dired_indent ();

      अक्षर *absolute_name = शून्य;
      अगर (print_hyperlink)
        {
          absolute_name = canonicalize_filename_mode (name, CAN_MISSING);
          अगर (! absolute_name
              && ! ignorable_traversal_errno (त्रुटि_सं))
            file_failure (command_पंक्ति_arg,
                          _("error canonicalizing %s"), name);
        }
      quote_name (realname ? realname : name, dirname_quoting_options, -1,
                  शून्य, true, &subdired_obstack, absolute_name);

      मुक्त (absolute_name);

      dired_outstring (":\n");
    }

  /* Read the directory entries, and insert the subfiles into the 'cwd_file'
     table.  */

  जबतक (true)
    {
      /* Set errno to zero so we can distinguish between a readdir failure
         and when readdir simply finds that there are no more entries.  */
      त्रुटि_सं = 0;
      अग्र = सूची_पढ़ो (dirp);
      अगर (अग्र)
        {
          अगर (! file_ignored (अग्र->d_name))
            {
              क्रमागत filetype type;
#अगर HAVE_STRUCT_DIRENT_D_TYPE
              type = d_type_filetype[अग्र->d_type];
#अन्यथा
              type = unknown;
#पूर्ण_अगर
              योग_blocks += gobble_file (अग्र->d_name, type,
                                           RELIABLE_D_INO (अग्र),
                                           false, name);

              /* In this narrow case, print out each name right away, so
                 ls uses constant memory while processing the entries of
                 this directory.  Useful when there are many (millions)
                 of entries in a directory.  */
              अगर (format == one_per_पंक्ति && sort_type == sort_none
                      && !print_block_आकार && !recursive)
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
      अन्यथा
        {
          पूर्णांक err = त्रुटि_सं;
          अगर (err == 0)
            अवरोध;
          /* Ignore errors indicating that the directory was removed.  */
          अगर (ignorable_traversal_errno (err))
            अवरोध;
          file_failure (command_पंक्ति_arg, _("reading directory %s"), name);
          अगर (err != EOVERFLOW)
            अवरोध;
        }

      /* When processing a very large directory, and since we've inhibited
         interrupts, this loop would take so long that ls would be annoyingly
         uninterruptible.  This ensures that it handles signals promptly.  */
      process_signals ();
    }

  अगर (बंद_सूची (dirp) != 0)
    {
      file_failure (command_पंक्ति_arg, _("closing directory %s"), name);
      /* Don't return; print whatever we got.  */
    }

  /* Sort the directory contents.  */
  sort_files ();

  /* If any member files are subdirectories, perhaps they should have their
     contents listed rather than being mentioned here as files.  */

  अगर (recursive)
    extract_dirs_from_files (name, false);

  अगर (format == long_format || print_block_आकार)
    {
      अक्षर buf[LONGEST_HUMAN_READABLE + 3];
      अक्षर *p = human_readable (योग_blocks, buf + 1, human_output_opts,
                                ST_NBLOCKSIZE, output_block_आकार);
      अक्षर *pend = p + म_माप (p);
      *--p = ' ';
      *pend++ = eolbyte;
      dired_indent ();
      dired_outstring (_("total"));
      dired_outbuf (p, pend - p);
    }

  अगर (cwd_n_used)
    print_current_files ();
}

/* Add 'pattern' to the list of patterns for which files that match are
   not listed.  */

अटल व्योम
add_ignore_pattern (अक्षर स्थिर *pattern)
{
  काष्ठा ignore_pattern *ignore;

  ignore = xmalloc (माप *ignore);
  ignore->pattern = pattern;
  /* Add it to the head of the linked list.  */
  ignore->अग्र = ignore_patterns;
  ignore_patterns = ignore;
}

/* Return true if one of the PATTERNS matches FILE.  */

अटल bool
patterns_match (काष्ठा ignore_pattern स्थिर *patterns, अक्षर स्थिर *file)
{
  क्रम (काष्ठा ignore_pattern स्थिर *p = patterns; p; p = p->अग्र)
    अगर (fnmatch (p->pattern, file, FNM_PERIOD) == 0)
      वापस true;
  वापस false;
}

/* Return true if FILE should be ignored.  */

अटल bool
file_ignored (अक्षर स्थिर *name)
{
  वापस ((ignore_mode != IGNORE_MINIMAL
           && name[0] == '.'
           && (ignore_mode == IGNORE_DEFAULT || ! name[1 + (name[1] == '.')]))
          || (ignore_mode == IGNORE_DEFAULT
              && patterns_match (hide_patterns, name))
          || patterns_match (ignore_patterns, name));
}

/* POSIX requires that a file size be printed without a sign, even
   when negative.  Assume the typical case where negative sizes are
   actually positive values that have wrapped around.  */

अटल uintmax_t
unsigned_file_आकार (off_t आकार)
{
  वापस आकार + (आकार < 0) * ((uintmax_t) OFF_T_MAX - OFF_T_MIN + 1);
}

#अगर_घोषित HAVE_CAP
/* Return true if NAME has a capability (see linux/capability.h) */
अटल bool
has_capability (अक्षर स्थिर *name)
{
  अक्षर *परिणाम;
  bool has_cap;

  cap_t cap_d = cap_get_file (name);
  अगर (cap_d == शून्य)
    वापस false;

  परिणाम = cap_to_text (cap_d, शून्य);
  cap_free (cap_d);
  अगर (!परिणाम)
    वापस false;

  /* check if human-readable capability string is empty */
  has_cap = !!*परिणाम;

  cap_free (परिणाम);
  वापस has_cap;
}
#अन्यथा
अटल bool
has_capability (MAYBE_UNUSED अक्षर स्थिर *name)
{
  त्रुटि_सं = ENOTSUP;
  वापस false;
}
#पूर्ण_अगर

/* Enter and remove entries in the table 'cwd_file'.  */

अटल व्योम
free_ent (काष्ठा fileinfo *f)
{
  मुक्त (f->name);
  मुक्त (f->linkname);
  मुक्त (f->absolute_name);
  अगर (f->scontext != UNKNOWN_SECURITY_CONTEXT)
    aclinfo_scontext_free (f->scontext);
}

/* Empty the table of files.  */
अटल व्योम
clear_files (व्योम)
{
  क्रम (idx_t i = 0; i < cwd_n_used; i++)
    {
      काष्ठा fileinfo *f = sorted_file[i];
      free_ent (f);
    }

  cwd_n_used = 0;
  cwd_some_quoted = false;
  any_has_acl = false;
  inode_संख्या_width = 0;
  block_आकार_width = 0;
  nlink_width = 0;
  owner_width = 0;
  group_width = 0;
  author_width = 0;
  scontext_width = 0;
  major_device_संख्या_width = 0;
  minor_device_संख्या_width = 0;
  file_आकार_width = 0;
}

/* Cache file_has_aclinfo failure, when it's trivial to do.
   Like file_has_aclinfo, but when F's st_dev says it's on a file
   system lacking ACL support, return 0 with ENOTSUP immediately.  */
अटल पूर्णांक
file_has_aclinfo_cache (अक्षर स्थिर *file, काष्ठा fileinfo *f,
                        काष्ठा aclinfo *ai, पूर्णांक flags)
{
  /* If UNSUPPORTED_SCONTEXT, these variables hold the
     st_dev and associated info for the most recently processed device
     for which file_has_aclinfo failed indicating lack of support.  */
  अटल पूर्णांक unsupported_return;
  अटल अक्षर *unsupported_scontext;
  अटल पूर्णांक unsupported_scontext_err;
  अटल dev_t unsupported_device;

  अगर (f->stat_ok && unsupported_scontext
      && f->stat.st_dev == unsupported_device)
    {
      ai->buf = ai->u._gl_acl_ch;
      ai->आकार = 0;
      ai->scontext = unsupported_scontext;
      ai->scontext_err = unsupported_scontext_err;
      त्रुटि_सं = ENOTSUP;
      वापस unsupported_return;
    }

  त्रुटि_सं = 0;
  पूर्णांक n = file_has_aclinfo (file, ai, flags);
  पूर्णांक err = त्रुटि_सं;
  अगर (!USE_ACL)
    n = 0;
  /* A security-context error does not imply that ACLs are unsupported.
     Require an unsupported attribute listing too if ACLs or capability
     colors are needed.  */
  अगर (f->stat_ok && n <= 0 && !acl_errno_valid (err)
      && ((!USE_ACL && !format_needs_capability)
          || (ai->आकार < 0 && !acl_errno_valid (ai->u.err)))
      && (!(flags & ACL_GET_SCONTEXT) || !acl_errno_valid (ai->scontext_err)))
    {
      /* FIXME: acl_errno_valid() may be a bit too broad determine file system
         non support.  E.g. EBUSY was added for per file reasons (on cygwin). */
      unsupported_return = n;
      unsupported_scontext = ai->scontext;
      unsupported_scontext_err = ai->scontext_err;
      unsupported_device = f->stat.st_dev;
    }
  वापस n;
}

/* Cache has_capability failure, when it's trivial to do.
   Like has_capability, but when F's st_dev says it's on a file
   system lacking capability support, return 0 with ENOTSUP immediately.  */
अटल bool
has_capability_cache (अक्षर स्थिर *file, काष्ठा fileinfo *f)
{
  /* If UNSUPPORTED_CACHED, UNSUPPORTED_DEVICE is the cached
     st_dev of the most recently processed device for which we've
     found that has_capability fails indicating lack of support.  */
  अटल bool unsupported_cached;
  अटल dev_t unsupported_device;

  अगर (f->stat_ok && unsupported_cached && f->stat.st_dev == unsupported_device)
    {
      त्रुटि_सं = ENOTSUP;
      वापस 0;
    }

  bool b = has_capability (file);
  अगर (f->stat_ok && !b && !acl_errno_valid (त्रुटि_सं))
    {
      unsupported_cached = true;
      unsupported_device = f->stat.st_dev;
    }
  वापस b;
}

/* Get the ACL, security context, and capability information needed for F.
   FULL_NAME names the file; DO_DEREF says whether to follow symlinks.  */
अटल व्योम
get_extended_info (अक्षर स्थिर *full_name, काष्ठा fileinfo *f, bool do_deref)
{
  /* FIXME: file_has_aclinfo() conflates capability and acl xattr lookup
     with security context attributes.  Ideally we'd have separate
     ACL_GET_{CAPABILITY,ACL} flags to lookup just the required items,
     simplifying the logic here.  */

  क्रमागत filetype type = f->filetype;
  bool check_acl = USE_ACL && format == long_format;
  bool get_scontext = false;
#अगर HAVE_SMACK || USE_SELINUX_SELINUX_H
  get_scontext = (format == long_format) | print_scontext;
#पूर्ण_अगर
  bool check_capability = format_needs_capability & (type == normal);

  अगर (type != arg_directory && (check_acl | get_scontext | check_capability))
    {
      काष्ठा aclinfo ai;
      /* Without ACL support, request contexts to obtain the attribute
         names needed for capability coloring too.  */
      पूर्णांक aclinfo_flags = ((do_deref ? ACL_SYMLINK_FOLLOW : 0)
                           | (get_scontext || (!USE_ACL && check_capability)
                              ? ACL_GET_SCONTEXT : 0)
                           | filetype_d_type[type]);
      पूर्णांक n = file_has_aclinfo_cache (full_name, f, &ai, aclinfo_flags);
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
           && (त्रुटि_सं == EACCES
               || ignorable_traversal_errno (त्रुटि_सं));

      f->acl_type = (!have_scontext && !have_acl
                     ? (cannot_access_acl ? ACL_T_UNKNOWN : ACL_T_NONE)
                     : (have_scontext && !have_acl
                        ? ACL_T_LSM_CONTEXT_ONLY
                        : ACL_T_YES));
      any_has_acl |= f->acl_type != ACL_T_NONE;

      अगर (format == long_format && n < 0 && !cannot_access_acl)
        error (0, त्रुटि_सं, "%s", quotef (full_name));
      अन्यथा
        {
          /* When requesting security context information, don't make
             ls fail just because the file (even a command line argument)
             isn't on the right type of file system.  I.e., a getfilecon
             failure isn't in the same class as a stat failure.  */
          अगर (print_scontext && ai.scontext_err
              && ! ignorable_traversal_errno (ai.scontext_err)
              && (! (is_ENOTSUP (ai.scontext_err)
                     || ai.scontext_err == ENODATA)))
            error (0, ai.scontext_err, "%s", quotef (full_name));
        }

      /* has_capability adds around 30% runtime to 'ls --color',
         so call it only if really needed.  Note capability coloring
         is disabled in the default color config.  */
      अगर (check_capability && aclinfo_has_xattr (&ai, XATTR_NAME_CAPS))
        f->has_capability = has_capability_cache (full_name, f);

      f->scontext = ai.scontext;
      ai.scontext = शून्य;
      aclinfo_free (&ai);
    }
}

अटल bool
needs_quoting (अक्षर स्थिर *name)
{
  अक्षर test[2];
  माप_प्रकार len = quotearg_बफर (test, माप test , name, -1,
                                filename_quoting_options);
  वापस *name != *test || म_माप (name) != len;
}

/* Add a file to the current table of files.
   Verify that the file exists, and print an error message if it does not.
   Return the number of blocks that the file occupies.  */
अटल uintmax_t
gobble_file (अक्षर स्थिर *name, क्रमागत filetype type, ino_t inode,
             bool command_पंक्ति_arg, अक्षर स्थिर *स_नाम)
{
  uintmax_t blocks = 0;
  काष्ठा fileinfo *f;

  /* An inode value prior to gobble_file necessarily came from readdir,
     which is not used for command line arguments.  */
  affirm (! command_पंक्ति_arg || inode == NOT_AN_INODE_संख्या);

  अगर (cwd_n_used == cwd_n_alloc)
    cwd_file = xpalloc (cwd_file, &cwd_n_alloc, 1, -1, माप *cwd_file);

  f = &cwd_file[cwd_n_used];
  स_रखो (f, '\0', माप *f);
  f->stat.st_ino = inode;
  f->filetype = type;
  f->scontext = UNKNOWN_SECURITY_CONTEXT;

  f->quoted = -1;
  अगर ((! cwd_some_quoted) && align_variable_outer_quotes)
    {
      /* Determine if any quoted for padding purposes.  */
      f->quoted = needs_quoting (name);
      अगर (f->quoted)
        cwd_some_quoted = 1;
    }

  bool check_stat =
     (command_पंक्ति_arg
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
      || (print_inode && inode == NOT_AN_INODE_संख्या)
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
  अक्षर स्थिर *full_name = name;
  अगर (check_stat | print_scontext | format_needs_capability
      && name[0] != '/' && स_नाम)
    {
      अक्षर *p = alloca (म_माप (name) + म_माप (स_नाम) + 2);
      attach (p, स_नाम, name);
      full_name = p;
    }

  bool do_deref;

  अगर (!check_stat)
    do_deref = dereference == DEREF_ALWAYS;
  अन्यथा
    {
      पूर्णांक err;

      अगर (print_hyperlink)
        {
          f->absolute_name = canonicalize_filename_mode (full_name,
                                                         CAN_MISSING);
          अगर (! f->absolute_name
              && (command_पंक्ति_arg
                  || ! ignorable_traversal_errno (त्रुटि_सं)))
            file_failure (command_पंक्ति_arg,
                          _("error canonicalizing %s"), full_name);
        }

      चयन (dereference)
        {
        हाल DEREF_ALWAYS:
          err = do_stat (full_name, &f->stat);
          do_deref = true;
          अवरोध;

        हाल DEREF_COMMAND_पंक्ति_ARGUMENTS:
        हाल DEREF_COMMAND_पंक्ति_SYMLINK_TO_DIR:
          अगर (command_पंक्ति_arg)
            {
              bool need_lstat;
              err = do_stat (full_name, &f->stat);
              do_deref = true;

              अगर (dereference == DEREF_COMMAND_पंक्ति_ARGUMENTS)
                अवरोध;

              need_lstat = (err < 0
                            ? (त्रुटि_सं == ENOENT || त्रुटि_सं == ELOOP)
                            : ! S_ISDIR (f->stat.st_mode));
              अगर (!need_lstat)
                अवरोध;

              /* stat failed because of ENOENT || ELOOP, maybe indicating a
                 non-traversable symlink.  Or stat succeeded,
                 FULL_NAME does not refer to a directory,
                 and --dereference-command-line-symlink-to-dir is in effect.
                 Fall through so that we call lstat instead.  */
            }
          FALLTHROUGH;

        हाल DEREF_NEVER:
          err = do_lstat (full_name, &f->stat);
          do_deref = false;
          अवरोध;

        हाल DEREF_UNDEFINED: शेष:
          affirm (false);
        }

      अगर (err != 0)
        {
          अगर (! command_पंक्ति_arg && ignorable_traversal_errno (त्रुटि_सं))
            {
              पूर्णांक stat_errno = त्रुटि_सं;
              काष्ठा stat linkstat;
              bool dangling_link = (do_deref
                                    && (type == symbolic_link
                                        || type == unknown)
                                    && do_lstat (full_name, &linkstat) == 0);
              त्रुटि_सं = stat_errno;
              अगर (! dangling_link)
                {
                  free_ent (f);
                  वापस 0;
                }
            }

          /* Failure to stat a command line argument leads to
             an exit status of 2.  For other files, stat failure
             provokes an exit status of 1.  */
          file_failure (command_पंक्ति_arg,
                        _("cannot access %s"), full_name);

          अगर (command_पंक्ति_arg)
            {
              free_ent (f);
              वापस 0;
            }

          f->name = xstrdup (name);
          cwd_n_used++;

          वापस 0;
        }

      f->stat_ok = true;
      f->filetype = type = d_type_filetype[IFTODT (f->stat.st_mode)];
    }

  अगर (type == directory && command_पंक्ति_arg && !immediate_dirs)
    f->filetype = type = arg_directory;

  get_extended_info (full_name, f, do_deref);

  अगर ((type == symbolic_link)
      & ((format == long_format) | check_symlink_mode))
    {
      काष्ठा stat linkstats;

      अगर (! get_link_name (full_name, f, command_पंक्ति_arg))
        {
          free_ent (f);
          वापस 0;
        }

      /* Use the slower quoting path for this entry, though
         don't update CWD_SOME_QUOTED since alignment not affected.  */
      अगर (f->linkname && f->quoted == 0 && needs_quoting (f->linkname))
        f->quoted = -1;

      /* Avoid following symbolic links when possible, i.e., when
         they won't be traced and when no indicator is needed.  */
      अगर (f->linkname
          && (file_type <= indicator_style || check_symlink_mode)
          && stat_for_mode (full_name, &linkstats) == 0)
        {
          f->linkok = true;
          f->linkmode = linkstats.st_mode;
        }
    }

  blocks = STP_NBLOCKS (&f->stat);
  अगर (format == long_format || print_block_आकार)
    {
      अक्षर buf[LONGEST_HUMAN_READABLE + 1];
      पूर्णांक len = mbswidth (human_readable (blocks, buf, human_output_opts,
                                          ST_NBLOCKSIZE, output_block_आकार),
                          MBSWIDTH_FLAGS);
      अगर (block_आकार_width < len)
        block_आकार_width = len;
    }

  अगर (format == long_format)
    {
      अगर (print_owner)
        {
          पूर्णांक len = format_user_width (f->stat.st_uid);
          अगर (owner_width < len)
            owner_width = len;
        }

      अगर (print_group)
        {
          पूर्णांक len = format_group_width (f->stat.st_gid);
          अगर (group_width < len)
            group_width = len;
        }

      अगर (print_author)
        {
          पूर्णांक len = format_user_width (f->stat.st_author);
          अगर (author_width < len)
            author_width = len;
        }
    }

  अगर (print_scontext)
    {
      पूर्णांक len = म_माप (f->scontext);
      अगर (scontext_width < len)
        scontext_width = len;
    }

  अगर (format == long_format)
    {
      अक्षर b[INT_BUFSIZE_BOUND (uintmax_t)];
      पूर्णांक b_len = म_माप (umaxtostr (f->stat.st_nlink, b));
      अगर (nlink_width < b_len)
        nlink_width = b_len;

      अगर ((type == chardev) | (type == blockdev))
        {
          अक्षर buf[INT_BUFSIZE_BOUND (uintmax_t)];
          पूर्णांक len = म_माप (umaxtostr (major (f->stat.st_rdev), buf));
          अगर (major_device_संख्या_width < len)
            major_device_संख्या_width = len;
          len = म_माप (umaxtostr (minor (f->stat.st_rdev), buf));
          अगर (minor_device_संख्या_width < len)
            minor_device_संख्या_width = len;
          len = major_device_संख्या_width + 2 + minor_device_संख्या_width;
          अगर (file_आकार_width < len)
            file_आकार_width = len;
        }
      अन्यथा
        {
          अक्षर buf[LONGEST_HUMAN_READABLE + 1];
          uintmax_t आकार = unsigned_file_आकार (f->stat.st_आकार);
          पूर्णांक len = mbswidth (human_readable (आकार, buf,
                                              file_human_output_opts,
                                              1, file_output_block_आकार),
                              MBSWIDTH_FLAGS);
          अगर (file_आकार_width < len)
            file_आकार_width = len;
        }
    }

  अगर (print_inode)
    {
      अक्षर buf[INT_BUFSIZE_BOUND (uintmax_t)];
      पूर्णांक len = म_माप (umaxtostr (f->stat.st_ino, buf));
      अगर (inode_संख्या_width < len)
        inode_संख्या_width = len;
    }

  f->name = xstrdup (name);
  cwd_n_used++;

  वापस blocks;
}

/* Return true if F refers to a directory.  */
अटल bool
is_directory (स्थिर काष्ठा fileinfo *f)
{
  वापस f->filetype == directory || f->filetype == arg_directory;
}

/* Return true if F refers to a (symlinked) directory.  */
अटल bool
is_linked_directory (स्थिर काष्ठा fileinfo *f)
{
  वापस f->filetype == directory || f->filetype == arg_directory
         || S_ISDIR (f->linkmode);
}

/* Put the name of the file that FILENAME is a symbolic link to
   into the LINKNAME field of 'f'.  COMMAND_LINE_ARG indicates whether
   FILENAME is a command-line argument.  */

अटल bool
get_link_name (अक्षर स्थिर *filename, काष्ठा fileinfo *f, bool command_पंक्ति_arg)
{
  f->linkname = areadlink_with_आकार (filename, f->stat.st_आकार);
  अगर (f->linkname == शून्य)
    {
      अगर (! command_पंक्ति_arg
          && ignorable_traversal_errno (त्रुटि_सं))
        वापस false;
      file_failure (command_पंक्ति_arg, _("cannot read symbolic link %s"),
                    filename);
    }
  वापस true;
}

/* Return true if the last component of NAME is '.' or '..'
   This is so we don't try to recurse on '././././. ...' */

अटल bool
basename_is_dot_or_dotdot (अक्षर स्थिर *name)
{
  अक्षर स्थिर *base = last_component (name);
  वापस dot_or_dotdot (base);
}

/* Remove any entries from CWD_FILE that are for directories,
   and queue them to be listed as directories instead.
   DIRNAME is the prefix to prepend to each dirname
   to make it correct relative to ls's working dir;
   if it is null, no prefix is needed and "." and ".." should not be ignored.
   If COMMAND_LINE_ARG is true, this directory was mentioned at the top level,
   This is desirable when processing directories recursively.  */

अटल व्योम
extract_dirs_from_files (अक्षर स्थिर *स_नाम, bool command_पंक्ति_arg)
{
  idx_t i, j;
  bool ignore_dot_and_dot_dot = (स_नाम != शून्य);

  अगर (स_नाम && LOOP_DETECT)
    {
      /* Insert a marker entry first.  When we dequeue this marker entry,
         we'll know that DIRNAME has been processed and may be removed
         from the set of active directories.  */
      queue_directory (शून्य, स_नाम, false);
    }

  /* Queue the directories last one first, because queueing reverses the
     order.  */
  क्रम (i = cwd_n_used; 0 < i; )
    {
      i--;
      काष्ठा fileinfo *f = sorted_file[i];

      अगर (is_directory (f)
          && (! ignore_dot_and_dot_dot
              || ! basename_is_dot_or_dotdot (f->name)))
        {
          अगर (!स_नाम || f->name[0] == '/')
            queue_directory (f->name, f->linkname, command_पंक्ति_arg);
          अन्यथा
            {
              अक्षर *name = file_name_concat (स_नाम, f->name, शून्य);
              queue_directory (name, f->linkname, command_पंक्ति_arg);
              मुक्त (name);
            }
          अगर (f->filetype == arg_directory)
            free_ent (f);
        }
    }

  /* Now delete the directories from the table, compacting all the remaining
     entries.  */

  क्रम (i = 0, j = 0; i < cwd_n_used; i++)
    {
      काष्ठा fileinfo *f = sorted_file[i];
      sorted_file[j] = f;
      j += (f->filetype != arg_directory);
    }
  cwd_n_used = j;
}

/* Use strcoll to compare strings in this locale.  If an error occurs,
   report an error and longjmp to failed_strcoll.  */

अटल लाँघ_बफ failed_strcoll;

अटल पूर्णांक
xstrcoll (अक्षर स्थिर *a, अक्षर स्थिर *b)
{
  पूर्णांक diff;
  त्रुटि_सं = 0;
  diff = strcoll (a, b);
  अगर (त्रुटि_सं)
    {
      error (0, त्रुटि_सं, _("cannot compare file names %s and %s"),
             quote_n (0, a), quote_n (1, b));
      set_exit_status (false);
      दीर्घ_लाँघ (failed_strcoll, 1);
    }
  वापस diff;
}

/* Comparison routines for sorting the files.  */

प्रकार व्योम स्थिर *V;
प्रकार पूर्णांक (*qsortFunc)(V a, V b);

/* Used below in DEFINE_SORT_FUNCTIONS for _df_ sort function variants.  */
अटल पूर्णांक
dirfirst_check (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
                पूर्णांक (*cmp) (V, V))
{
  पूर्णांक diff = is_linked_directory (b) - is_linked_directory (a);
  वापस diff ? diff : cmp (a, b);
}

/* Define the 8 different sort function variants required for each sortkey.
   KEY_NAME is a token describing the sort key, e.g., ctime, atime, size.
   KEY_CMP_FUNC is a function to compare records based on that key, e.g.,
   ctime_cmp, atime_cmp, size_cmp.  Append KEY_NAME to the string,
   '[rev_][x]str{cmp|coll}[_df]_', to create each function name.  */
#घोषणा DEFINE_SORT_FUNCTIONS(कुंजी_name, कुंजी_cmp_func)			\
  /* direct, non-dirfirst versions */					\
  अटल पूर्णांक xstrcoll_##key_name (V a, V b)				\
  { वापस कुंजी_cmp_func (a, b, xstrcoll); }				\
  ATTRIBUTE_PURE अटल पूर्णांक strcmp_##key_name (V a, V b)		\
  { वापस कुंजी_cmp_func (a, b, म_भेद); }				\
                                                                        \
  /* reverse, non-dirfirst versions */					\
  अटल पूर्णांक rev_xstrcoll_##key_name (V a, V b)				\
  { वापस कुंजी_cmp_func (b, a, xstrcoll); }				\
  ATTRIBUTE_PURE अटल पूर्णांक rev_strcmp_##key_name (V a, V b)	\
  { वापस कुंजी_cmp_func (b, a, म_भेद); }				\
                                                                        \
  /* direct, dirfirst versions */					\
  अटल पूर्णांक xstrcoll_df_##key_name (V a, V b)				\
  { वापस dirfirst_check (a, b, xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE अटल पूर्णांक strcmp_df_##key_name (V a, V b)		\
  { वापस dirfirst_check (a, b, strcmp_##key_name); }			\
                                                                        \
  /* reverse, dirfirst versions */					\
  अटल पूर्णांक rev_xstrcoll_df_##key_name (V a, V b)			\
  { वापस dirfirst_check (a, b, rev_xstrcoll_##key_name); }		\
  ATTRIBUTE_PURE अटल पूर्णांक rev_strcmp_df_##key_name (V a, V b)	\
  { वापस dirfirst_check (a, b, rev_strcmp_##key_name); }

अटल पूर्णांक
cmp_ctime (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
           पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = timespec_cmp (get_stat_ctime (&b->stat),
                           get_stat_ctime (&a->stat));
  वापस diff ? diff : cmp (a->name, b->name);
}

अटल पूर्णांक
cmp_mtime (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
           पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = timespec_cmp (get_stat_mtime (&b->stat),
                           get_stat_mtime (&a->stat));
  वापस diff ? diff : cmp (a->name, b->name);
}

अटल पूर्णांक
cmp_atime (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
           पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = timespec_cmp (get_stat_atime (&b->stat),
                           get_stat_atime (&a->stat));
  वापस diff ? diff : cmp (a->name, b->name);
}

अटल पूर्णांक
cmp_btime (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
           पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = timespec_cmp (get_stat_btime (&b->stat),
                           get_stat_btime (&a->stat));
  वापस diff ? diff : cmp (a->name, b->name);
}

अटल पूर्णांक
off_cmp (off_t a, off_t b)
{
  वापस _GL_CMP (a, b);
}

अटल पूर्णांक
cmp_आकार (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
          पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = off_cmp (b->stat.st_आकार, a->stat.st_आकार);
  वापस diff ? diff : cmp (a->name, b->name);
}

अटल पूर्णांक
cmp_name (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
          पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  वापस cmp (a->name, b->name);
}

/* Compare file extensions.  Files with no extension are 'smallest'.
   If extensions are the same, compare by file names instead.  */

अटल पूर्णांक
cmp_extension (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
               पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  अक्षर स्थिर *base1 = म_खोजप (a->name, '.');
  अक्षर स्थिर *base2 = म_खोजप (b->name, '.');
  पूर्णांक diff = cmp (base1 ? base1 : "", base2 ? base2 : "");
  वापस diff ? diff : cmp (a->name, b->name);
}

/* Return the (cached) screen width,
   for the NAME associated with the passed fileinfo F.  */

अटल माप_प्रकार
fileinfo_name_width (काष्ठा fileinfo स्थिर *f)
{
  वापस f->width
         ? f->width
         : quote_name_width (f->name, filename_quoting_options, f->quoted);
}

अटल पूर्णांक
cmp_width (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b,
          पूर्णांक (*cmp) (अक्षर स्थिर *, अक्षर स्थिर *))
{
  पूर्णांक diff = fileinfo_name_width (a) - fileinfo_name_width (b);
  वापस diff ? diff : cmp (a->name, b->name);
}

DEFINE_SORT_FUNCTIONS (स_समय, cmp_ctime)
DEFINE_SORT_FUNCTIONS (mtime, cmp_mtime)
DEFINE_SORT_FUNCTIONS (atime, cmp_atime)
DEFINE_SORT_FUNCTIONS (btime, cmp_btime)
DEFINE_SORT_FUNCTIONS (आकार, cmp_आकार)
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
अटल पूर्णांक
cmp_version (काष्ठा fileinfo स्थिर *a, काष्ठा fileinfo स्थिर *b)
{
  पूर्णांक diff = filevercmp (a->name, b->name);
  वापस diff ? diff : म_भेद (a->name, b->name);
}

अटल पूर्णांक
xstrcoll_version (V a, V b)
{
  वापस cmp_version (a, b);
}
अटल पूर्णांक
rev_xstrcoll_version (V a, V b)
{
  वापस cmp_version (b, a);
}
अटल पूर्णांक
xstrcoll_df_version (V a, V b)
{
  वापस dirfirst_check (a, b, xstrcoll_version);
}
अटल पूर्णांक
rev_xstrcoll_df_version (V a, V b)
{
  वापस dirfirst_check (a, b, rev_xstrcoll_version);
}


/* We have 2^3 different variants for each sort-key function
   (for 3 independent sort modes).
   The function pointers stored in this array must be dereferenced as:

    sort_variants[sort_key][use_strcmp][reverse][dirs_first]

   Note that the order in which sort keys are listed in the function pointer
   array below is defined by the order of the elements in the time_type and
   sort_type enums!  */

#घोषणा LIST_SORTFUNCTION_VARIANTS(कुंजी_name)                        \
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

अटल qsortFunc स्थिर sort_functions[][2][2][2] =
  {
    LIST_SORTFUNCTION_VARIANTS (name),
    LIST_SORTFUNCTION_VARIANTS (extension),
    LIST_SORTFUNCTION_VARIANTS (width),
    LIST_SORTFUNCTION_VARIANTS (आकार),

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
        { शून्य, शून्य },
        { शून्य, शून्य },
      }
    },

    /* last are time sort functions */
    LIST_SORTFUNCTION_VARIANTS (mtime),
    LIST_SORTFUNCTION_VARIANTS (स_समय),
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

अटल व्योम
initialize_ordering_vector (व्योम)
{
  क्रम (idx_t i = 0; i < cwd_n_used; i++)
    sorted_file[i] = &cwd_file[i];
}

/* Cache values based on attributes global to all files.  */

अटल व्योम
update_current_files_info (व्योम)
{
  /* Cache screen width of name, if needed multiple times.  */
  अगर (sort_type == sort_width
      || (पंक्ति_लंबाई && (format == many_per_पंक्ति || format == horizontal)))
    {
      क्रम (idx_t i = 0; i < cwd_n_used; i++)
        {
          काष्ठा fileinfo *f = sorted_file[i];
          f->width = fileinfo_name_width (f);
        }
    }
}

/* Sort the files now in the table.  */

अटल व्योम
sort_files (व्योम)
{
  bool use_strcmp;

  अगर (sorted_file_alloc < cwd_n_used + (cwd_n_used >> 1))
    {
      मुक्त (sorted_file);
      sorted_file = xinmalloc (cwd_n_used, 3 * माप *sorted_file);
      sorted_file_alloc = 3 * cwd_n_used;
    }

  initialize_ordering_vector ();

  update_current_files_info ();

  अगर (sort_type == sort_none)
    वापस;

  /* Try strcoll.  If it fails, fall back on strcmp.  We can't safely
     ignore strcoll failures, as a failing strcoll might be a
     comparison function that is not a total order, and if we ignored
     the failure this might cause qsort to dump core.  */

  अगर (! बनाओ_लाँघ (failed_strcoll))
    use_strcmp = false;      /* strcoll() succeeded */
  अन्यथा
    {
      use_strcmp = true;
      affirm (sort_type != sort_version);
      initialize_ordering_vector ();
    }

  /* When sort_type == sort_time, use time_type as subindex.  */
  mpsort ((व्योम स्थिर **) sorted_file, cwd_n_used,
          sort_functions[sort_type + (sort_type == sort_time ? time_type : 0)]
                        [use_strcmp][sort_reverse]
                        [directories_first]);
}

/* List all the files now in the table.  */

अटल व्योम
print_current_files (व्योम)
{
  चयन (format)
    {
    हाल one_per_पंक्ति:
      क्रम (idx_t i = 0; i < cwd_n_used; i++)
        {
          print_file_name_and_frills (sorted_file[i], 0);
          अक्षर_दो (eolbyte);
        }
      अवरोध;

    हाल many_per_पंक्ति:
      अगर (! पंक्ति_लंबाई)
        print_with_separator (' ');
      अन्यथा
        print_many_per_पंक्ति ();
      अवरोध;

    हाल horizontal:
      अगर (! पंक्ति_लंबाई)
        print_with_separator (' ');
      अन्यथा
        print_horizontal ();
      अवरोध;

    हाल with_commas:
      print_with_separator (',');
      अवरोध;

    हाल long_format:
      क्रम (idx_t i = 0; i < cwd_n_used; i++)
        {
          set_normal_color ();
          print_long_format (sorted_file[i]);
          dired_outbyte (eolbyte);
        }
      अवरोध;
    }
}

/* Replace the first %b with precomputed aligned month names.
   Note on glibc-2.7 at least, this speeds up the whole 'ls -lU'
   process by around 17%, compared to letting strftime() handle the %b.  */

अटल सूचक_भेद_प्रकार
align_nstrftime (अक्षर *buf, माप_प्रकार आकार, bool recent, काष्ठा पंचांग स्थिर *पंचांग,
                 timezone_t tz, पूर्णांक ns)
{
  अक्षर स्थिर *nfmt = (use_abformat
                      ? abformat[recent][पंचांग->tm_mon]
                      : long_time_format[recent]);
  वापस nstrftime (buf, आकार, nfmt, पंचांग, tz, ns);
}

/* Return the expected number of columns in a long-format timestamp,
   or zero if it cannot be calculated.  */

अटल पूर्णांक
long_time_expected_width (व्योम)
{
  अटल पूर्णांक width = -1;

  अगर (width < 0)
    {
      समय_प्रकार epoch = 0;
      काष्ठा पंचांग पंचांग;
      अक्षर buf[TIME_STAMP_LEN_MAXIMUM + 1];

      /* In case you're wondering if localtime_rz can fail with an input time_t
         value of 0, let's just say it's very unlikely, but not inconceivable.
         The TZ environment variable would have to specify a time zone that
         is 2**31-1900 years or more ahead of UTC.  This could happen only on
         a 64-bit system that blindly accepts e.g., TZ=UTC+20000000000000.
         However, this is not possible with Solaris 10 or glibc-2.3.5, since
         their implementations limit the offset to 167:59 and 24:00, resp.  */
      अगर (localtime_rz (localtz, &epoch, &पंचांग))
        {
          सूचक_भेद_प्रकार len = align_nstrftime (buf, माप buf, false,
                                           &पंचांग, localtz, 0);
          अगर (len > 0)
            width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
        }

      अगर (width < 0)
        width = 0;
    }

  वापस width;
}

/* Print the user or group name NAME, with numeric id ID, using a
   print width of WIDTH columns.  */

अटल व्योम
format_user_or_group (अक्षर स्थिर *name, uintmax_t id, पूर्णांक width)
{
  अगर (name)
    {
      पूर्णांक name_width = mbswidth (name, MBSWIDTH_FLAGS);
      पूर्णांक width_gap = name_width < 0 ? 0 : width - name_width;
      पूर्णांक pad = MAX (0, width_gap);
      dired_outstring (name);

      करो
        dired_outbyte (' ');
      जबतक (pad--);
    }
  अन्यथा
    dired_pos += म_लिखो ("%*ju ", width, id);
}

/* Print the name or id of the user with id U, using a print width of
   WIDTH.  */

अटल व्योम
format_user (uid_t u, पूर्णांक width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? शून्य : getuser (u)), u, width);
}

/* Likewise, for groups.  */

अटल व्योम
format_group (gid_t g, पूर्णांक width, bool stat_ok)
{
  format_user_or_group (! stat_ok ? "?" :
                        (numeric_ids ? शून्य : getgroup (g)), g, width);
}

/* Return the number of columns that format_user_or_group will print,
   or -1 if unknown.  */

अटल पूर्णांक
format_user_or_group_width (अक्षर स्थिर *name, uintmax_t id)
{
  वापस (name
          ? mbswidth (name, MBSWIDTH_FLAGS)
          : snprintf (शून्य, 0, "%ju", id));
}

/* Return the number of columns that format_user will print,
   or -1 if unknown.  */

अटल पूर्णांक
format_user_width (uid_t u)
{
  वापस format_user_or_group_width (numeric_ids ? शून्य : getuser (u), u);
}

/* Likewise, for groups.  */

अटल पूर्णांक
format_group_width (gid_t g)
{
  वापस format_user_or_group_width (numeric_ids ? शून्य : getgroup (g), g);
}

/* Return a pointer to a formatted version of F->stat.st_ino,
   possibly using buffer, which must be at least
   INT_BUFSIZE_BOUND (uintmax_t) bytes.  */
अटल अक्षर *
format_inode (अक्षर buf[INT_BUFSIZE_BOUND (uintmax_t)],
              स्थिर काष्ठा fileinfo *f)
{
  वापस (f->stat_ok && f->stat.st_ino != NOT_AN_INODE_संख्या
          ? umaxtostr (f->stat.st_ino, buf)
          : (अक्षर *) "?");
}

/* Print information about F in long format.  */
अटल व्योम
print_long_format (स्थिर काष्ठा fileinfo *f)
{
  अक्षर modebuf[12];
  अक्षर buf
    [LONGEST_HUMAN_READABLE + 1		/* inode */
     + LONGEST_HUMAN_READABLE + 1	/* size in blocks */
     + माप (modebuf) - 1 + 1		/* mode string */
     + INT_BUFSIZE_BOUND (uintmax_t)	/* st_nlink */
     + LONGEST_HUMAN_READABLE + 2	/* major device number */
     + LONGEST_HUMAN_READABLE + 1	/* minor device number */
     + TIME_STAMP_LEN_MAXIMUM + 1	/* max length of time/date */
     ];
  सूचक_भेद_प्रकार s;
  अक्षर *p;
  काष्ठा timespec when_timespec;
  काष्ठा पंचांग when_local;
  bool btime_ok = true;

  /* Compute the mode string, except remove the trailing space if no
     file in this directory has an ACL or security context.  */
  अगर (f->stat_ok)
    filemodestring (&f->stat, modebuf);
  अन्यथा
    {
      modebuf[0] = filetype_letter[f->filetype];
      स_रखो (modebuf + 1, '?', 10);
      modebuf[11] = '\0';
    }
  अगर (! any_has_acl)
    modebuf[10] = '\0';
  अन्यथा अगर (f->acl_type == ACL_T_LSM_CONTEXT_ONLY)
    modebuf[10] = '.';
  अन्यथा अगर (f->acl_type == ACL_T_YES)
    modebuf[10] = '+';
  अन्यथा अगर (f->acl_type == ACL_T_UNKNOWN)
    modebuf[10] = '?';

  चयन (time_type)
    {
    हाल time_ctime:
      when_timespec = get_stat_ctime (&f->stat);
      अवरोध;
    हाल time_mtime:
      when_timespec = get_stat_mtime (&f->stat);
      अवरोध;
    हाल time_atime:
      when_timespec = get_stat_atime (&f->stat);
      अवरोध;
    हाल time_btime:
      when_timespec = get_stat_btime (&f->stat);
      अगर (when_timespec.tv_sec == -1 && when_timespec.tv_nsec == -1)
        btime_ok = false;
      अवरोध;
    हाल time_numtypes: शेष:
      unreachable ();
    }

  p = buf;

  अगर (print_inode)
    {
      अक्षर hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
      p += प्र_लिखो (p, "%*s ", inode_संख्या_width, format_inode (hbuf, f));
    }

  अगर (print_block_आकार)
    {
      अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
      अक्षर स्थिर *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), hbuf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_आकार));
      पूर्णांक blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      क्रम (पूर्णांक pad = blocks_width < 0 ? 0 : block_आकार_width - blocks_width;
           0 < pad; pad--)
        *p++ = ' ';
      जबतक ((*p++ = *blocks++))
        जारी;
      p[-1] = ' ';
    }

  /* The last byte of the mode string is the POSIX
     "optional alternate access method flag".  */
  {
    अक्षर hbuf[INT_BUFSIZE_BOUND (uintmax_t)];
    p += प्र_लिखो (p, "%s %*s ", modebuf, nlink_width,
                  ! f->stat_ok ? "?" : umaxtostr (f->stat.st_nlink, hbuf));
  }

  dired_indent ();

  अगर (print_owner || print_group || print_author || print_scontext)
    {
      dired_outbuf (buf, p - buf);

      अगर (print_owner)
        format_user (f->stat.st_uid, owner_width, f->stat_ok);

      अगर (print_group)
        format_group (f->stat.st_gid, group_width, f->stat_ok);

      अगर (print_author)
        format_user (f->stat.st_author, author_width, f->stat_ok);

      अगर (print_scontext)
        format_user_or_group (f->scontext, 0, scontext_width);

      p = buf;
    }

  अगर (f->stat_ok
      && (S_ISCHR (f->stat.st_mode) || S_ISBLK (f->stat.st_mode)))
    {
      पूर्णांक blanks_width = (file_आकार_width
                          - (major_device_संख्या_width + 2
                             + minor_device_संख्या_width));
      p += प्र_लिखो (p, "%*ju, %*ju ",
                    major_device_संख्या_width + MAX (0, blanks_width),
                    (uintmax_t) major (f->stat.st_rdev),
                    minor_device_संख्या_width,
                    (uintmax_t) minor (f->stat.st_rdev));
    }
  अन्यथा
    {
      अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
      अक्षर स्थिर *आकार =
        (! f->stat_ok
         ? "?"
         : human_readable (unsigned_file_आकार (f->stat.st_आकार),
                           hbuf, file_human_output_opts, 1,
                           file_output_block_आकार));
      पूर्णांक आकार_width = mbswidth (आकार, MBSWIDTH_FLAGS);
      क्रम (पूर्णांक pad = आकार_width < 0 ? 0 : file_आकार_width - आकार_width;
           0 < pad; pad--)
        *p++ = ' ';
      जबतक ((*p++ = *आकार++))
        जारी;
      p[-1] = ' ';
    }

  s = -1;

  अगर (f->stat_ok && btime_ok
      && localtime_rz (localtz, &when_timespec.tv_sec, &when_local))
    {
      काष्ठा timespec six_months_ago;
      bool recent;

      /* If the file appears to be in the future, update the current
         time, in case the file happens to have been modified since
         the last time we checked the clock.  */
      अगर (timespec_cmp (current_time, when_timespec) < 0)
        समय_लो (&current_time);

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

  अगर (0 <= s)
    {
      p += s;
      *p++ = ' ';
    }
  अन्यथा
    {
      /* The time cannot be converted using the desired format, so
         print it as a huge integer number of seconds.  */
      अक्षर hbuf[INT_BUFSIZE_BOUND (intmax_t)];
      p += प्र_लिखो (p, "%*s ", long_time_expected_width (),
                    (! f->stat_ok || ! btime_ok
                     ? "?"
                     : timetostr (when_timespec.tv_sec, hbuf)));
      /* FIXME: (maybe) We discarded when_timespec.tv_nsec. */
    }

  dired_outbuf (buf, p - buf);
  माप_प्रकार w = print_name_with_quoting (f, false, &dired_obstack, p - buf);

  अगर (f->filetype == symbolic_link)
    {
      अगर (f->linkname)
        {
          dired_outstring (" -> ");
          print_name_with_quoting (f, true, शून्य, (p - buf) + w + 4);
          अगर (indicator_style != none)
            print_type_indicator (true, f->linkmode, unknown);
        }
    }
  अन्यथा अगर (indicator_style != none)
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

अटल माप_प्रकार
quote_name_buf (अक्षर **inbuf, माप_प्रकार bufsize, अक्षर *name,
                काष्ठा quoting_options स्थिर *options,
                पूर्णांक needs_general_quoting, माप_प्रकार *width, bool *pad)
{
  अक्षर *buf = *inbuf;
  माप_प्रकार displayed_width IF_LINT ( = 0);
  माप_प्रकार len = 0;
  bool quoted;

  क्रमागत quoting_style qs = get_quoting_style (options);
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
                                   && !dired && म_अक्षर (name, '\n'));

  अगर (needs_general_quoting != 0)
    {
      len = quotearg_बफर (buf, bufsize, name, -1, options);
      अगर (bufsize <= len)
        {
          buf = xmalloc (len + 1);
          quotearg_बफर (buf, len + 1, name, -1, options);
        }

      quoted = (*name != *buf) || म_माप (name) != len;
    }
  अन्यथा अगर (needs_further_quoting || extra_newline_protection)
    {
      len = म_माप (name);
      अगर (bufsize <= len)
        buf = xmalloc (len + 1);
      स_नकल (buf, name, len + 1);

      quoted = false;
    }
  अन्यथा
    {
      len = म_माप (name);
      buf = name;
      quoted = false;
    }

  /* It's safe to replace newlines without multi-byte iteration, as newlines
     do not appear as part of any multi-byte encoded character.  */
  अगर (extra_newline_protection)
    क्रम (अक्षर *p = buf; p < buf + len; p++)
      अगर (eolbyte && *p == '\n')
        *p = '?';

  अगर (needs_further_quoting)
    {
      अगर (MB_CUR_MAX > 1)
        {
          अक्षर स्थिर *p = buf;
          अक्षर स्थिर *plimit = buf + len;
          अक्षर *q = buf;
          displayed_width = 0;

          जबतक (p < plimit)
            चयन (*p)
              {
                हाल ' ': हाल '!': हाल '"': हाल '#': हाल '%':
                हाल '&': हाल '\'': हाल '(': हाल ')': हाल '*':
                हाल '+': हाल ',': हाल '-': हाल '.': हाल '/':
                हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
                हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
                हाल ':': हाल ';': हाल '<': हाल '=': हाल '>':
                हाल '?':
                हाल 'A': हाल 'B': हाल 'C': हाल 'D': हाल 'E':
                हाल 'F': हाल 'G': हाल 'H': हाल 'I': हाल 'J':
                हाल 'K': हाल 'L': हाल 'M': हाल 'N': हाल 'O':
                हाल 'P': हाल 'Q': हाल 'R': हाल 'S': हाल 'T':
                हाल 'U': हाल 'V': हाल 'W': हाल 'X': हाल 'Y':
                हाल 'Z':
                हाल '[': हाल '\\': हाल ']': हाल '^': हाल '_':
                हाल 'a': हाल 'b': हाल 'c': हाल 'd': हाल 'e':
                हाल 'f': हाल 'g': हाल 'h': हाल 'i': हाल 'j':
                हाल 'k': हाल 'l': हाल 'm': हाल 'n': हाल 'o':
                हाल 'p': हाल 'q': हाल 'r': हाल 's': हाल 't':
                हाल 'u': हाल 'v': हाल 'w': हाल 'x': हाल 'y':
                हाल 'z': हाल '{': हाल '|': हाल '}': हाल '~':
                  /* These characters are printable ASCII characters.  */
                  *q++ = *p++;
                  displayed_width += 1;
                  अवरोध;
                शेष:
                  /* If we have a multibyte sequence, copy it until we
                     reach its end, replacing each non-printable multibyte
                     character with a single question mark.  */
                  {
                    mbstate_t mbstate; mbszero (&mbstate);
                    करो
                      {
                        char32_t wc;
                        माप_प्रकार bytes;
                        पूर्णांक w;

                        bytes = mbrtoc32 (&wc, p, plimit - p, &mbstate);

                        अगर (bytes == (माप_प्रकार) -1)
                          {
                            /* An invalid multibyte sequence was
                               encountered.  Skip one input byte, and
                               put a question mark.  */
                            p++;
                            *q++ = '?';
                            displayed_width += 1;
                            अवरोध;
                          }

                        अगर (bytes == (माप_प्रकार) -2)
                          {
                            /* An incomplete multibyte character
                               at the end.  Replace it entirely with
                               a question mark.  */
                            p = plimit;
                            *q++ = '?';
                            displayed_width += 1;
                            अवरोध;
                          }

                        अगर (bytes == 0)
                          /* A null wide character was encountered.  */
                          bytes = 1;

                        w = c32width (wc);
                        अगर (w >= 0)
                          {
                            /* A printable multibyte character.
                               Keep it.  */
                            क्रम (; bytes > 0; --bytes)
                              *q++ = *p++;
                            displayed_width += w;
                          }
                        अन्यथा
                          {
                            /* An nonprintable multibyte character.
                               Replace it entirely with a question
                               mark.  */
                            p += bytes;
                            *q++ = '?';
                            displayed_width += 1;
                          }
                      }
                    जबतक (! mbsinit (&mbstate));
                  }
                  अवरोध;
              }

          /* The buffer may have shrunk.  */
          len = q - buf;
        }
      अन्यथा
        {
          अक्षर *p = buf;
          अक्षर स्थिर *plimit = buf + len;

          जबतक (p < plimit)
            {
              अगर (! है_छाप (to_uchar (*p)))
                *p = '?';
              p++;
            }
          displayed_width = len;
        }
    }
  अन्यथा अगर (width != शून्य)
    {
      अगर (MB_CUR_MAX > 1)
        {
          displayed_width = mbsnwidth (buf, len, MBSWIDTH_FLAGS);
          displayed_width = MAX (0, displayed_width);
        }
      अन्यथा
        {
          अक्षर स्थिर *p = buf;
          अक्षर स्थिर *plimit = buf + len;

          displayed_width = 0;
          जबतक (p < plimit)
            {
              अगर (है_छाप (to_uchar (*p)))
                displayed_width++;
              p++;
            }
        }
    }

  /* Set padding to better align quoted items,
     and also give a visual indication that quotes are
     not actually part of the name.  */
  *pad = (align_variable_outer_quotes && cwd_some_quoted && ! quoted);

  अगर (width != शून्य)
    *width = displayed_width;

  *inbuf = buf;

  वापस len;
}

अटल माप_प्रकार
quote_name_width (अक्षर स्थिर *name, काष्ठा quoting_options स्थिर *options,
                  पूर्णांक needs_general_quoting)
{
  अक्षर smallbuf[बफ_मान];
  अक्षर *buf = smallbuf;
  माप_प्रकार width;
  bool pad;

  quote_name_buf (&buf, माप smallbuf, (अक्षर *) name, options,
                  needs_general_quoting, &width, &pad);

  अगर (buf != smallbuf && buf != name)
    मुक्त (buf);

  width += pad;

  वापस width;
}

/* %XX escape any input out of range as defined in RFC3986,
   and also if PATH, convert all path separators to '/'.  */
अटल अक्षर *
file_escape (अक्षर स्थिर *str, bool path)
{
  अक्षर *esc = xnmalloc (3, म_माप (str) + 1);
  अक्षर *p = esc;
  जबतक (*str)
    {
      अगर (path && ISSLASH (*str))
        {
          *p++ = '/';
          str++;
        }
      अन्यथा अगर (RFC3986[to_uchar (*str)])
        *p++ = *str++;
      अन्यथा
        p += प्र_लिखो (p, "%%%02x", to_uchar (*str++));
    }
  *p = '\0';
  वापस esc;
}

अटल माप_प्रकार
quote_name (अक्षर स्थिर *name, काष्ठा quoting_options स्थिर *options,
            पूर्णांक needs_general_quoting, स्थिर काष्ठा bin_str *color,
            bool allow_pad, काष्ठा obstack *ढेर, अक्षर स्थिर *absolute_name)
{
  अक्षर smallbuf[बफ_मान];
  अक्षर *buf = smallbuf;
  माप_प्रकार len;
  bool pad;

  len = quote_name_buf (&buf, माप smallbuf, (अक्षर *) name, options,
                        needs_general_quoting, शून्य, &pad);

  अगर (pad && allow_pad)
    dired_outbyte (' ');

  अगर (color)
    print_color_indicator (color);

  /* If we're padding, then don't include the outer quotes in
     the --hyperlink, to improve the alignment of those links.  */
  bool skip_quotes = false;

  अगर (absolute_name)
    {
      अगर (align_variable_outer_quotes && cwd_some_quoted && ! pad)
        {
          skip_quotes = true;
          अक्षर_दो (*buf);
        }
      अक्षर *h = file_escape (hostname, /* path= */ false);
      अक्षर *n = file_escape (absolute_name, /* path= */ true);
      /* TODO: It would be good to be able to define parameters
         to give hints to the terminal as how best to render the URI.
         For example since ls is outputting a dense block of URIs
         it would be best to not underline by default, and only
         do so upon hover etc.  */
      म_लिखो ("\033]8;;file://%s%s%s\033\\", h, *n == '/' ? "" : "/", n);
      मुक्त (h);
      मुक्त (n);
    }

  अगर (ढेर)
    धकेलो_current_dired_pos (ढेर);

  ख_डालो (buf + skip_quotes, 1, len - (skip_quotes * 2), मानक_निकास);

  dired_pos += len;

  अगर (ढेर)
    धकेलो_current_dired_pos (ढेर);

  अगर (absolute_name)
    {
      ख_माला_दो ("\033]8;;\033\\", मानक_निकास);
      अगर (skip_quotes)
        अक्षर_दो (*(buf + len - 1));
    }

  अगर (buf != smallbuf && buf != name)
    मुक्त (buf);

  वापस len + pad;
}

अटल माप_प्रकार
print_name_with_quoting (स्थिर काष्ठा fileinfo *f,
                         bool symlink_target,
                         काष्ठा obstack *ढेर,
                         माप_प्रकार start_col)
{
  अक्षर स्थिर *name = symlink_target ? f->linkname : f->name;

  स्थिर काष्ठा bin_str *color
    = print_with_color ? get_color_indicator (f, symlink_target) : शून्य;

  bool used_color_this_time = (print_with_color
                               && (color || is_colored (C_NORM)));

  माप_प्रकार len = quote_name (name, filename_quoting_options, f->quoted,
                           color, !symlink_target, ढेर, f->absolute_name);

  process_signals ();
  अगर (used_color_this_time)
    {
      prep_non_filename_text ();

      /* We use the byte length rather than display width here as
         an optimization to avoid accurately calculating the width,
         because we only output the clear to EOL sequence if the name
         _might_ wrap to the next line.  This may output a sequence
         unnecessarily in multi-byte locales for example,
         but in that case it's inconsequential to the output.  */
      अगर (पंक्ति_लंबाई
          && (start_col / पंक्ति_लंबाई != (start_col + len - 1) / पंक्ति_लंबाई))
        put_indicator (&color_indicator[C_CLR_TO_EOL]);
    }

  वापस len;
}

अटल व्योम
prep_non_filename_text (व्योम)
{
  अगर (color_indicator[C_END].string != शून्य)
    put_indicator (&color_indicator[C_END]);
  अन्यथा
    {
      put_indicator (&color_indicator[C_बायाँ]);
      put_indicator (&color_indicator[C_RESET]);
      put_indicator (&color_indicator[C_दायाँ]);
    }
}

/* Print the file name of 'f' with appropriate quoting.
   Also print file size, inode number, and filetype indicator character,
   as requested by switches.  */

अटल माप_प्रकार
print_file_name_and_frills (स्थिर काष्ठा fileinfo *f, माप_प्रकार start_col)
{
  अक्षर buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  set_normal_color ();

  अगर (print_inode)
    म_लिखो ("%*s ", format == with_commas ? 0 : inode_संख्या_width,
            format_inode (buf, f));

  अगर (print_block_आकार)
    {
      अक्षर स्थिर *blocks =
        (! f->stat_ok
         ? "?"
         : human_readable (STP_NBLOCKS (&f->stat), buf, human_output_opts,
                           ST_NBLOCKSIZE, output_block_आकार));
      पूर्णांक blocks_width = mbswidth (blocks, MBSWIDTH_FLAGS);
      पूर्णांक pad = 0;
      अगर (0 <= blocks_width && block_आकार_width && format != with_commas)
        pad = block_आकार_width - blocks_width;
      म_लिखो ("%*s%s ", pad, "", blocks);
    }

  अगर (print_scontext)
    म_लिखो ("%*s ", format == with_commas ? 0 : scontext_width, f->scontext);

  माप_प्रकार width = print_name_with_quoting (f, false, शून्य, start_col);

  अगर (indicator_style != none)
    width += print_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);

  वापस width;
}

/* Given these arguments describing a file, return the single-byte
   type indicator, or 0.  */
अटल अक्षर
get_type_indicator (bool stat_ok, mode_t mode, क्रमागत filetype type)
{
  अक्षर c;

  अगर (stat_ok ? S_ISREG (mode) : type == normal)
    {
      अगर (stat_ok && indicator_style == classify && (mode & S_IXUGO))
        c = '*';
      अन्यथा
        c = 0;
    }
  अन्यथा
    {
      अगर (stat_ok ? S_ISDIR (mode) : type == directory || type == arg_directory)
        c = '/';
      अन्यथा अगर (indicator_style == slash)
        c = 0;
      अन्यथा अगर (stat_ok ? S_ISLNK (mode) : type == symbolic_link)
        c = '@';
      अन्यथा अगर (stat_ok ? S_ISFIFO (mode) : type == fifo)
        c = '|';
      अन्यथा अगर (stat_ok ? S_ISSOCK (mode) : type == sock)
        c = '=';
      अन्यथा अगर (stat_ok && S_ISDOOR (mode))
        c = '>';
      अन्यथा
        c = 0;
    }
  वापस c;
}

अटल bool
print_type_indicator (bool stat_ok, mode_t mode, क्रमागत filetype type)
{
  अक्षर c = get_type_indicator (stat_ok, mode, type);
  अगर (c)
    dired_outbyte (c);
  वापस !!c;
}

/* Returns if color sequence was printed.  */
अटल bool
print_color_indicator (स्थिर काष्ठा bin_str *ind)
{
  अगर (ind)
    {
      /* Need to reset so not dealing with attribute combinations */
      अगर (is_colored (C_NORM))
        restore_default_color ();
      put_indicator (&color_indicator[C_बायाँ]);
      put_indicator (ind);
      put_indicator (&color_indicator[C_दायाँ]);
    }

  वापस ind != शून्य;
}

/* Returns color indicator or NULL if none.  */
ATTRIBUTE_PURE
अटल स्थिर काष्ठा bin_str *
get_color_indicator (स्थिर काष्ठा fileinfo *f, bool symlink_target)
{
  क्रमागत indicator_no type;
  काष्ठा color_ext_type *ext;	/* Color extension */
  माप_प्रकार len;			/* Length of name */

  अक्षर स्थिर *name;
  mode_t mode;
  पूर्णांक linkok;
  अगर (symlink_target)
    {
      name = f->linkname;
      mode = f->linkmode;
      linkok = f->linkok ? 0 : -1;
    }
  अन्यथा
    {
      name = f->name;
      mode = file_or_link_mode (f);
      linkok = f->linkok;
    }

  /* Is this a nonexistent file?  If so, linkok == -1.  */

  अगर (linkok == -1 && is_colored (C_MISSING))
    type = C_MISSING;
  अन्यथा अगर (!f->stat_ok)
    {
      अटल क्रमागत indicator_no स्थिर filetype_indicator[] =
        {
          C_ORPHAN, C_FIFO, C_CHR, C_DIR, C_BLK, C_FILE,
          C_LINK, C_SOCK, C_FILE, C_DIR
        };
      static_assert (countof (filetype_indicator) == filetype_cardinality);
      type = filetype_indicator[f->filetype];
    }
  अन्यथा
    {
      अगर (S_ISREG (mode))
        {
          type = C_FILE;

          अगर ((mode & S_ISUID) != 0 && is_colored (C_SETUID))
            type = C_SETUID;
          अन्यथा अगर ((mode & S_ISGID) != 0 && is_colored (C_SETGID))
            type = C_SETGID;
          अन्यथा अगर (f->has_capability)
            type = C_CAP;
          अन्यथा अगर ((mode & S_IXUGO) != 0 && is_colored (C_EXEC))
            type = C_EXEC;
          अन्यथा अगर ((1 < f->stat.st_nlink) && is_colored (C_MULTIHARDLINK))
            type = C_MULTIHARDLINK;
        }
      अन्यथा अगर (S_ISDIR (mode))
        {
          type = C_DIR;

          अगर ((mode & S_ISVTX) && (mode & S_IWOTH)
              && is_colored (C_STICKY_OTHER_WRITABLE))
            type = C_STICKY_OTHER_WRITABLE;
          अन्यथा अगर ((mode & S_IWOTH) != 0 && is_colored (C_OTHER_WRITABLE))
            type = C_OTHER_WRITABLE;
          अन्यथा अगर ((mode & S_ISVTX) != 0 && is_colored (C_STICKY))
            type = C_STICKY;
        }
      अन्यथा अगर (S_ISLNK (mode))
        type = C_LINK;
      अन्यथा अगर (S_ISFIFO (mode))
        type = C_FIFO;
      अन्यथा अगर (S_ISSOCK (mode))
        type = C_SOCK;
      अन्यथा अगर (S_ISBLK (mode))
        type = C_BLK;
      अन्यथा अगर (S_ISCHR (mode))
        type = C_CHR;
      अन्यथा अगर (S_ISDOOR (mode))
        type = C_DOOR;
      अन्यथा
        {
          /* Classify a file of some other type as C_ORPHAN.  */
          type = C_ORPHAN;
        }
    }

  /* Check the file's suffix only if still classified as C_FILE.  */
  ext = शून्य;
  अगर (type == C_FILE)
    {
      /* Test if NAME has a recognized suffix.  */

      len = म_माप (name);
      name += len;		/* Pointer to final \0.  */
      क्रम (ext = color_ext_list; ext != शून्य; ext = ext->अग्र)
        {
          अगर (ext->ext.len <= len)
            {
              अगर (ext->exact_match)
                {
                  अगर (STREQ_LEN (name - ext->ext.len, ext->ext.string,
                                 ext->ext.len))
                    अवरोध;
                }
              अन्यथा
                {
                  अगर (c_strncasecmp (name - ext->ext.len, ext->ext.string,
                                     ext->ext.len) == 0)
                    अवरोध;
                }
            }
        }
    }

  /* Adjust the color for orphaned symlinks.  */
  अगर (type == C_LINK && !linkok)
    {
      अगर (color_symlink_as_referent || is_colored (C_ORPHAN))
        type = C_ORPHAN;
    }

  स्थिर काष्ठा bin_str *स्थिर s
    = ext ? &(ext->seq) : &color_indicator[type];

  वापस s->string ? s : शून्य;
}

/* Output a color indicator (which may contain nulls).
   Note we don't update dired_pos here as emacs can be configured
   to handle color codes by stripping/converting them.  */
अटल व्योम
put_indicator (स्थिर काष्ठा bin_str *ind)
{
  अगर (! used_color)
    {
      used_color = true;

      /* If the standard output is a controlling terminal, watch out
         for signals, so that the colors can be restored to the
         default state if "ls" is suspended or interrupted.  */

      अगर (0 <= tcgetpgrp (STDOUT_FILENO))
        signal_init ();

      prep_non_filename_text ();
    }

  ख_डालो (ind->string, ind->len, 1, मानक_निकास);
}

अटल माप_प्रकार
लंबाई_of_file_name_and_frills (स्थिर काष्ठा fileinfo *f)
{
  माप_प्रकार len = 0;
  अक्षर buf[MAX (LONGEST_HUMAN_READABLE + 1, INT_BUFSIZE_BOUND (uintmax_t))];

  अगर (print_inode)
    len += 1 + (format == with_commas
                ? म_माप (umaxtostr (f->stat.st_ino, buf))
                : inode_संख्या_width);

  अगर (print_block_आकार)
    len += 1 + (format == with_commas
                ? म_माप (! f->stat_ok ? "?"
                          : human_readable (STP_NBLOCKS (&f->stat), buf,
                                            human_output_opts, ST_NBLOCKSIZE,
                                            output_block_आकार))
                : block_आकार_width);

  अगर (print_scontext)
    len += 1 + (format == with_commas ? म_माप (f->scontext) : scontext_width);

  len += fileinfo_name_width (f);

  अगर (indicator_style != none)
    {
      अक्षर c = get_type_indicator (f->stat_ok, f->stat.st_mode, f->filetype);
      len += (c != 0);
    }

  वापस len;
}

अटल व्योम
print_many_per_पंक्ति (व्योम)
{
  idx_t cols = calculate_columns (true);
  काष्ठा column_info स्थिर *पंक्ति_fmt = &column_info[cols - 1];

  /* Calculate the number of rows that will be in each column except possibly
     for a short column on the right.  */
  idx_t rows = cwd_n_used / cols + (cwd_n_used % cols != 0);

  क्रम (idx_t row = 0; row < rows; row++)
    {
      माप_प्रकार col = 0;
      idx_t filesno = row;
      माप_प्रकार pos = 0;

      /* Print the next row.  */
      जबतक (true)
        {
          काष्ठा fileinfo स्थिर *f = sorted_file[filesno];
          माप_प्रकार name_लंबाई = लंबाई_of_file_name_and_frills (f);
          माप_प्रकार max_name_लंबाई = पंक्ति_fmt->col_arr[col++];
          print_file_name_and_frills (f, pos);

          अगर (cwd_n_used - rows <= filesno)
            अवरोध;
          filesno += rows;

          indent (pos + name_लंबाई, pos + max_name_लंबाई);
          pos += max_name_लंबाई;
        }
      अक्षर_दो (eolbyte);
    }
}

अटल व्योम
print_horizontal (व्योम)
{
  माप_प्रकार pos = 0;
  idx_t cols = calculate_columns (false);
  काष्ठा column_info स्थिर *पंक्ति_fmt = &column_info[cols - 1];
  काष्ठा fileinfo स्थिर *f = sorted_file[0];
  माप_प्रकार name_लंबाई = लंबाई_of_file_name_and_frills (f);
  माप_प्रकार max_name_लंबाई = पंक्ति_fmt->col_arr[0];

  /* Print first entry.  */
  print_file_name_and_frills (f, 0);

  /* Now the rest.  */
  क्रम (idx_t filesno = 1; filesno < cwd_n_used; filesno++)
    {
      idx_t col = filesno % cols;

      अगर (col == 0)
        {
          अक्षर_दो (eolbyte);
          pos = 0;
        }
      अन्यथा
        {
          indent (pos + name_लंबाई, pos + max_name_लंबाई);
          pos += max_name_लंबाई;
        }

      f = sorted_file[filesno];
      print_file_name_and_frills (f, pos);

      name_लंबाई = लंबाई_of_file_name_and_frills (f);
      max_name_लंबाई = पंक्ति_fmt->col_arr[col];
    }
  अक्षर_दो (eolbyte);
}

/* Output name + SEP + ' '.  */

अटल व्योम
print_with_separator (अक्षर sep)
{
  माप_प्रकार pos = 0;

  क्रम (idx_t filesno = 0; filesno < cwd_n_used; filesno++)
    {
      काष्ठा fileinfo स्थिर *f = sorted_file[filesno];
      माप_प्रकार len = पंक्ति_लंबाई ? लंबाई_of_file_name_and_frills (f) : 0;

      अगर (filesno != 0)
        {
          अक्षर separator;
          माप_प्रकार अग्र_pos = 2 + (sep == ',' && filesno < cwd_n_used - 1);

          अगर (! पंक्ति_लंबाई
              || (! ckd_add (&अग्र_pos, अग्र_pos, pos)
                  && ! ckd_add (&अग्र_pos, अग्र_pos, len)
                  && अग्र_pos <= पंक्ति_लंबाई))
            {
              pos += 2;
              separator = ' ';
            }
          अन्यथा
            {
              pos = 0;
              separator = eolbyte;
            }

          अक्षर_दो (sep);
          अक्षर_दो (separator);
        }

      print_file_name_and_frills (f, pos);
      pos += len;
    }
  अक्षर_दो (eolbyte);
}

/* Assuming cursor is at position FROM, indent up to position TO.
   Use a TAB character instead of two or more spaces whenever possible.  */

अटल व्योम
indent (माप_प्रकार from, माप_प्रकार to)
{
  जबतक (from < to)
    {
      अगर (tabsize != 0 && to / tabsize > (from + 1) / tabsize)
        {
          अक्षर_दो ('\t');
          from += tabsize - from % tabsize;
        }
      अन्यथा
        {
          अक्षर_दो (' ');
          from++;
        }
    }
}

/* Put DIRNAME/NAME into DEST, handling '.' and '/' properly.  */
/* FIXME: maybe remove this function someday.  See about using a
   non-malloc'ing version of file_name_concat.  */

अटल व्योम
attach (अक्षर *dest, अक्षर स्थिर *स_नाम, अक्षर स्थिर *name)
{
  अक्षर स्थिर *dirnamep = स_नाम;

  /* Copy dirname if it is not ".".  */
  अगर (स_नाम[0] != '.' || स_नाम[1] != 0)
    {
      जबतक (*dirnamep)
        *dest++ = *dirnamep++;
      /* Add '/' if 'dirname' doesn't already end with it.  */
      अगर (dirnamep > स_नाम && dirnamep[-1] != '/')
        *dest++ = '/';
    }
  जबतक (*name)
    *dest++ = *name++;
  *dest = 0;
}

/* Allocate enough column info suitable for the current number of
   files and display columns, and initialize the info to represent the
   narrowest possible columns.  */

अटल व्योम
init_column_info (idx_t max_cols)
{
  /* Currently allocated columns in column_info.  */
  अटल idx_t column_info_alloc;

  अगर (column_info_alloc < max_cols)
    {
      idx_t old_column_info_alloc = column_info_alloc;
      column_info = xpalloc (column_info, &column_info_alloc,
                             max_cols - column_info_alloc, -1,
                             माप *column_info);

      /* Allocate the new size_t objects by computing the triangle
         formula n * (n + 1) / 2, except that we don't need to
         allocate the part of the triangle that we've already
         allocated.  Check for address arithmetic overflow.  */
      idx_t column_info_growth = column_info_alloc - old_column_info_alloc, s;
      अगर (ckd_add (&s, old_column_info_alloc + 1, column_info_alloc)
          || ckd_mul (&s, s, column_info_growth))
        xalloc_die ();
      माप_प्रकार *p = xinmalloc (s >> 1, माप *p);

      /* Grow the triangle by parceling out the cells just allocated.  */
      क्रम (idx_t i = old_column_info_alloc; i < column_info_alloc; i++)
        {
          column_info[i].col_arr = p;
          p += i + 1;
        }
    }

  क्रम (idx_t i = 0; i < max_cols; ++i)
    {
      column_info[i].valid_len = true;
      column_info[i].पंक्ति_len = i * MIN_COLUMN_WIDTH + 1;
      क्रम (idx_t j = 0; j < i; ++j)
        column_info[i].col_arr[j] = MIN_COLUMN_WIDTH;
      column_info[i].col_arr[i] = 1;
    }
}

/* Calculate the number of columns needed to represent the current set
   of files in the current display width.  */

अटल idx_t
calculate_columns (bool by_columns)
{
  /* Normally the maximum number of columns is determined by the
     screen width.  But if few files are available this might limit it
     as well.  */
  idx_t max_cols = 0 < max_idx && max_idx < cwd_n_used ? max_idx : cwd_n_used;

  init_column_info (max_cols);

  /* Compute the maximum number of possible columns.  */
  क्रम (idx_t filesno = 0; filesno < cwd_n_used; ++filesno)
    {
      काष्ठा fileinfo स्थिर *f = sorted_file[filesno];
      माप_प्रकार name_लंबाई = लंबाई_of_file_name_and_frills (f);

      क्रम (idx_t i = 0; i < max_cols; ++i)
        {
          अगर (column_info[i].valid_len)
            {
              idx_t idx = (by_columns
                           ? filesno / ((cwd_n_used + i) / (i + 1))
                           : filesno % (i + 1));
              माप_प्रकार real_लंबाई = name_लंबाई + (idx == i ? 0 : 2);

              अगर (column_info[i].col_arr[idx] < real_लंबाई)
                {
                  column_info[i].पंक्ति_len += (real_लंबाई
                                              - column_info[i].col_arr[idx]);
                  column_info[i].col_arr[idx] = real_लंबाई;
                  column_info[i].valid_len = (column_info[i].पंक्ति_len
                                              <= पंक्ति_लंबाई);
                }
            }
        }
    }

  /* Find maximum allowed columns.  */
  idx_t cols;
  क्रम (cols = max_cols; 1 < cols; --cols)
    {
      अगर (column_info[cols - 1].valid_len)
        अवरोध;
    }

  वापस cols;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      ख_माला_दो (_("\
List information about the FILEs (the current directory by शेष).\n\
Sort entries alphabetically अगर none of -cftuvSUX nor --sort is specified.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         करो not ignore entries starting with .\n\
"));
      oputs (_("\
  -A, --almost-all\n\
         करो not list implied . and ..\n\
"));
      oputs (_("\
      --author\n\
         with -l, print the author of each file\n\
"));
      oputs (_("\
  -b, --escape\n\
         print C-style escapes क्रम nongraphic characters\n\
"));
      oputs (_("\
      --block-आकार=आकार_2\n\
         with -l, scale sizes by आकार_2 when printing them;\n\
         e.g., '--block-size=M'; see आकार_2 format below\n\
"));
      oputs (_("\
  -B, --ignore-backups\n\
         करो not list implied entries ending with ~\n\
"));
      oputs (_("\
  -c\n\
         with -lt: sort by, and show, स_समय\n\
           (समय of last change of file status information);\n\
         with -l: show स_समय and sort by name;\n\
         otherwise: sort by स_समय, newest first\n\
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
         generate output designed क्रम Emacs' dired mode\n\
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
         like -F, except करो not append '*'\n\
"));
      oputs (_("\
      --format=WORD\n\
         across,horizontal (-x), commas (-m), दीर्घ (-l),\n\
         single-column (-1), verbose (-l), vertical (-C)\n\
"));
      oputs (_("\
      --full-समय\n\
         like -l --समय-style=full-iso\n\
"));
      oputs (_("\
  -g\n\
         like -l, but करो not list owner\n\
"));
      oputs (_("\
      --group-directories-first\n\
         group directories before files\n\
"));
      oputs (_("\
  -G, --no-group\n\
         in a दीर्घ listing, don't print group names\n\
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
  -H, --dereference-command-पंक्ति\n\
         follow symbolic links listed on the command पंक्ति\n\
"));
      oputs (_("\
      --dereference-command-पंक्ति-symlink-to-dir\n\
         follow each command पंक्ति symbolic link that points to a directory\n\
"));
      oputs (_("\
      --hide=PATTERN\n\
         करो not list implied entries matching shell PATTERN\n\
         (overridden by -a or -A)\n\
"));
      oputs (_("\
      --hyperlink[=WHEN]\n\
         hyperlink file names WHEN\n\
"));
      oputs (_("\
      --indicator-style=WORD\n\
         append indicator with style WORD to entry names:\n\
           none (शेष), slash (-p), file-type (--file-type), classify (-F)\n\
"));
      oputs (_("\
  -i, --inode\n\
         print the index संख्या of each file\n\
"));
      oputs (_("\
  -I, --ignore=PATTERN\n\
         करो not list implied entries matching shell PATTERN\n\
"));
      oputs (_("\
  -k, --kibibytes\n\
         शेष to 1024-byte blocks क्रम file प्रणाली usage;\n\
         used only with -s and per directory totals\n\
"));
      oputs (_("\
  -l\n\
         use a दीर्घ listing format\n\
"));
      oputs (_("\
  -L, --dereference\n\
         when showing file information क्रम a symbolic link,\n\
         show information क्रम the file the link references\n\
         rather than क्रम the link itself\n\
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
         like -l, but करो not list group information\n\
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
         the शेष, unless program is 'ls' and output is a terminal\n\
"));
      oputs (_("\
  -Q, --quote-name\n\
         enclose entry names in द्विगुन quotes\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD क्रम entry names:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
         (overrides QUOTING_STYLE environment variable)\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse order जबतक sorting\n\
"));
      oputs (_("\
  -R, --recursive\n\
         list subdirectories recursively\n\
"));
      oputs (_("\
  -s, --आकार\n\
         print the allocated आकार of each file, in blocks\n\
"));
      oputs (_("\
  -S\n\
         sort by file आकार, largest first\n\
"));
      oputs (_("\
      --sort=WORD\n\
         change शेष 'name' sort to WORD:\n\
           none (-U), आकार (-S), समय (-t),\n\
           version (-v), extension (-X), name, width\n\
"));
      oputs (_("\
      --समय=WORD\n\
         select which timestamp used to display or sort;\n\
           access समय (-u): atime, access, use;\n\
           metadata change समय (-c): स_समय, status;\n\
           modified समय (शेष): mtime, modification;\n\
           birth समय: birth, creation;\n\
         with -l, WORD determines which समय to show;\n\
         with --sort=समय, sort by WORD (newest first)\n\
"));
      oputs (_("\
      --समय-style=TIME_STYLE\n\
         समय/date format with -l; see TIME_STYLE below\n\
"));
      oputs (_("\
  -t\n\
         sort by समय, newest first; see --समय\n\
"));
      oputs (_("\
  -T, --tabsize=COLS\n\
         assume tab stops at each COLS instead of 8\n\
"));
      oputs (_("\
  -u\n\
         with -lt: sort by, and show, access समय;\n\
         with -l: show access समय and sort by name;\n\
         otherwise: sort by access समय, newest first\n\
"));
      oputs (_("\
  -U\n\
         करो not sort directory entries\n\
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
         list entries by पंक्तियाँ instead of by columns\n\
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
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (_("\
  -1\n\
         list one file per पंक्ति\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_आकार_note ();
      ख_माला_दो (_("\
\n\
The TIME_STYLE argument can be full-iso, दीर्घ-iso, iso, locale, or +FORMAT.\n\
FORMAT is interpreted like in date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\n\
then FORMAT1 applies to non-recent files and FORMAT2 to recent files.\n\
TIME_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\n\
Also the TIME_STYLE environment variable sets the शेष style to use.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
The WHEN argument defaults to 'always' and can also be 'auto' or 'never'.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Using color to distinguish file types is disabled both by शेष and\n\
with --color=never.  With --color=स्वतः, ls emits color codes only when\n\
standard output is connected to a terminal.  The LS_COLORS environment\n\
variable can change the settings.  Use the dircolors(1) command to set it.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Exit status:\n\
 0  अगर OK,\n\
 1  अगर minor problems (e.g., cannot access subdirectory),\n\
 2  अगर serious trouble (e.g., cannot access command-पंक्ति argument).\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}
