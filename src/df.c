/* df - summarize free file system space
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu>.
   --human-readable option added by lm@sgi.com.
   --si and large file support added by eggert@twinsun.com.  */

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>
#समावेश <c-ctype.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "canonicalize.h"
#समावेश "fsusage.h"
#समावेश "human.h"
#समावेश "mbswidth.h"
#समावेश "mountlist.h"
#समावेश "quote.h"
#समावेश "find-mount-point.h"
#समावेश "hash.h"
#समावेश "xstrtol-error.h"
#समावेश "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "df"

#घोषणा AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

काष्ठा devlist
{
  dev_t dev_अंक;
  काष्ठा mount_entry *me;
  काष्ठा devlist *अग्र;
  काष्ठा devlist *अग्र_same_dev;
  काष्ठा devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
अटल Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
अटल bool show_all_fs;

/* If true, show only local file systems.  */
अटल bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
अटल bool show_listed_fs;

/* Human-readable options for output.  */
अटल पूर्णांक human_output_opts = -1;

/* The units to use when printing sizes.  */
अटल uintmax_t output_block_आकार;

/* True if a file system has been processed for output.  */
अटल bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
अटल bool require_sync;

/* Desired exit status.  */
अटल पूर्णांक exit_status;

/* A file system type to display.  */

काष्ठा fs_type_list
{
  अक्षर *fs_name;
  काष्ठा fs_type_list *fs_अग्र;
};

/* Linked list of file system types to display.
   If 'fs_select_list' is null, list all types.
   This table is generated dynamically from command-line options,
   rather than hardcoding into the program what it thinks are the
   valid file system types; let the user specify any file system type
   they want to, and if there are any file systems of that type, they
   will be shown.

   Some file system types:
   4.2 4.3 ufs nfs swap ignore io vm efs dbg */

अटल काष्ठा fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

अटल काष्ठा fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
अटल काष्ठा mount_entry *mount_list;

/* If true, print file system type as well.  */
अटल bool print_type;

/* If true, print a grand total at the end.  */
अटल bool print_grand_योग;

/* Grand total data.  */
अटल काष्ठा fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
अटल क्रमागत
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
प्रकार क्रमागत
{
  SOURCE_FIELD, /* file system */
  FSTYPE_FIELD, /* FS type */
  आकार_FIELD,   /* FS size */
  USED_FIELD,   /* FS size used  */
  AVAIL_FIELD,  /* FS size available */
  PCENT_FIELD,  /* percent used */
  ITOTAL_FIELD, /* inode total */
  IUSED_FIELD,  /* inodes used */
  IAVAIL_FIELD, /* inodes available */
  IPCENT_FIELD, /* inodes used in percent */
  TARGET_FIELD, /* mount point */
  FILE_FIELD,   /* specified file name */
  INVALID_FIELD /* validation marker */
} display_field_t;

/* Flag if a field contains a block, an inode or another value.  */
प्रकार क्रमागत
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
काष्ठा field_तथ्य_t
{
  display_field_t field;
  अक्षर स्थिर *arg;
  field_type_t field_type;
  अक्षर स्थिर *caption;/* NULL means use default header of this field.  */
  पूर्णांक width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_दायाँ;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
अटल काष्ठा field_तथ्य_t field_तथ्य[] = {
  [SOURCE_FIELD] = { SOURCE_FIELD,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_FIELD] = { FSTYPE_FIELD,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [आकार_FIELD] = { आकार_FIELD,
    "size",   BLOCK_FLD, N_("blocks"),      5, true, false },

  [USED_FIELD] = { USED_FIELD,
    "used",   BLOCK_FLD, N_("Used"),        5, true, false },

  [AVAIL_FIELD] = { AVAIL_FIELD,
    "avail",  BLOCK_FLD, N_("Available"),   5, true, false },

  [PCENT_FIELD] = { PCENT_FIELD,
    "pcent",  BLOCK_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_FIELD] = { ITOTAL_FIELD,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_FIELD] = { IUSED_FIELD,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_FIELD] = { IAVAIL_FIELD,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_FIELD] = { IPCENT_FIELD,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [TARGET_FIELD] = { TARGET_FIELD,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_FIELD] = { FILE_FIELD,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

अटल अक्षर स्थिर *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

अटल काष्ठा
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  अक्षर ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  काष्ठा field_तथ्य_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
काष्ठा field_मूल्यावली_t
{
  uintmax_t निवेश_units;
  uintmax_t output_units;
  uintmax_t योग_2;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_मूल;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  NO_SYNC_OPTION = अक्षर_उच्च + 1,
  SYNC_OPTION,
  योग_OPTION,
  OUTPUT_OPTION
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"block-size", required_argument, शून्य, 'B'},
  {"inodes", no_argument, शून्य, 'i'},
  {"human-readable", no_argument, शून्य, 'h'},
  {"si", no_argument, शून्य, 'H'},
  {"local", no_argument, शून्य, 'l'},
  {"output", optional_argument, शून्य, OUTPUT_OPTION},
  {"portability", no_argument, शून्य, 'P'},
  {"print-type", no_argument, शून्य, 'T'},
  {"sync", no_argument, शून्य, SYNC_OPTION},
  {"no-sync", no_argument, शून्य, NO_SYNC_OPTION},
  {"total", no_argument, शून्य, योग_OPTION},
  {"type", required_argument, शून्य, 't'},
  {"exclude-type", required_argument, शून्य, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

अटल पूर्णांक
automount_stat_err (अक्षर स्थिर *file, काष्ठा stat *st)
{
  पूर्णांक fd = खोलो (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  अगर (fd < 0)
    {
      अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENOTDIR)
        वापस त्रुटि_सं;
      वापस stat (file, st) == 0 ? 0 : त्रुटि_सं;
    }
  अन्यथा
    {
      पूर्णांक err = ख_स्थिति (fd, st) == 0 ? 0 : त्रुटि_सं;
      बंद (fd);
      वापस err;
    }
}

क्रमागत { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

अटल व्योम
replace_control_chars (अक्षर *cell)
{
  अक्षर *p = cell;
  जबतक (*p)
    {
      अगर (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

अटल व्योम
replace_invalid_chars (अक्षर *cell)
{
  अक्षर *srcend = cell + म_माप (cell);
  अक्षर *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  माप_प्रकार n;

  क्रम (अक्षर *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      माप_प्रकार srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      अगर (ok)
        ok = !c32iscntrl (wc);
      अन्यथा
        n = 1;

      अगर (ok)
        {
          स_हटाओ (dst, src, n);
          dst += n;
        }
      अन्यथा
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

अटल व्योम
replace_problematic_chars (अक्षर *cell)
{
  अटल पूर्णांक tty_out = -1;
  अगर (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

अटल व्योम
alloc_table_row (व्योम)
{
  अगर (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          माप *table.rows);
  idx_t स्थिर row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, माप *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

अटल व्योम
print_table (व्योम)
{
  क्रम (idx_t row = 0; row < table.nrows; row++)
    {
      क्रम (idx_t col = 0; col < table.ncolumns; col++)
        {
          अक्षर *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          अगर (col != 0)
            अक्षर_दो (' ');

          पूर्णांक width = mbswidth (cell, MBSWIDTH_FLAGS);
          पूर्णांक fill = width < 0 ? 0 : table.columns[col]->width - width;
          अगर (table.columns[col]->align_दायाँ)
            क्रम (; 0 < fill; fill--)
              अक्षर_दो (' ');
          ख_माला_दो (cell, मानक_निकास);
          अगर (col + 1 < table.ncolumns)
            क्रम (; 0 < fill; fill--)
              अक्षर_दो (' ');
        }
      अक्षर_दो ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

अटल व्योम
alloc_field (पूर्णांक f, अक्षर स्थिर *c)
{
  अगर (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             माप *table.columns);
  idx_t स्थिर column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_तथ्य[f];
  अगर (c != शून्य)
    field_तथ्य[f].caption = c;

  affirm (!field_तथ्य[f].used);

  /* Mark field as used.  */
  field_तथ्य[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
अटल व्योम
decode_output_arg (अक्षर स्थिर *arg)
{
  अक्षर *arg_writable = xstrdup (arg);
  अक्षर *s = arg_writable;
  करो
    {
      /* find next comma */
      अक्षर *comma = म_अक्षर (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      अगर (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      क्रम (idx_t i = 0; i < countof (field_तथ्य); i++)
        {
          अगर (streq (field_तथ्य[i].arg, s))
            {
              field = i;
              अवरोध;
            }
        }
      अगर (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (निकास_त्रुटि);
        }

      अगर (field_तथ्य[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_तथ्य[field].arg));
          usage (निकास_त्रुटि);
        }

      चयन (field)
        {
        हाल SOURCE_FIELD:
        हाल FSTYPE_FIELD:
        हाल USED_FIELD:
        हाल PCENT_FIELD:
        हाल ITOTAL_FIELD:
        हाल IUSED_FIELD:
        हाल IAVAIL_FIELD:
        हाल IPCENT_FIELD:
        हाल TARGET_FIELD:
        हाल FILE_FIELD:
          alloc_field (field, शून्य);
          अवरोध;

        हाल आकार_FIELD:
          alloc_field (field, N_("Size"));
          अवरोध;

        हाल AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          अवरोध;

        हाल INVALID_FIELD:
        शेष:
          affirm (!"invalid field");
        }
      s = comma;
    }
  जबतक (s);

  मुक्त (arg_writable);
}

/* Get the appropriate columns for the mode.  */
अटल व्योम
get_field_list (व्योम)
{
  चयन (header_mode)
    {
    हाल DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, शून्य);
      अगर (print_type)
        alloc_field (FSTYPE_FIELD, शून्य);
      alloc_field (आकार_FIELD,   शून्य);
      alloc_field (USED_FIELD,   शून्य);
      alloc_field (AVAIL_FIELD,  शून्य);
      alloc_field (PCENT_FIELD,  शून्य);
      alloc_field (TARGET_FIELD, शून्य);
      अवरोध;

    हाल HUMAN_MODE:
      alloc_field (SOURCE_FIELD, शून्य);
      अगर (print_type)
        alloc_field (FSTYPE_FIELD, शून्य);

      alloc_field (आकार_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   शून्य);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  शून्य);
      alloc_field (TARGET_FIELD, शून्य);
      अवरोध;

    हाल INODES_MODE:
      alloc_field (SOURCE_FIELD, शून्य);
      अगर (print_type)
        alloc_field (FSTYPE_FIELD, शून्य);
      alloc_field (ITOTAL_FIELD,  शून्य);
      alloc_field (IUSED_FIELD,   शून्य);
      alloc_field (IAVAIL_FIELD,  शून्य);
      alloc_field (IPCENT_FIELD,  शून्य);
      alloc_field (TARGET_FIELD,  शून्य);
      अवरोध;

    हाल POSIX_MODE:
      alloc_field (SOURCE_FIELD, शून्य);
      अगर (print_type)
        alloc_field (FSTYPE_FIELD, शून्य);
      alloc_field (आकार_FIELD,   शून्य);
      alloc_field (USED_FIELD,   शून्य);
      alloc_field (AVAIL_FIELD,  शून्य);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, शून्य);
      अवरोध;

    हाल OUTPUT_MODE:
      अगर (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      अवरोध;

    शेष:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

अटल व्योम
get_header (व्योम)
{
  alloc_table_row ();

  क्रम (idx_t col = 0; col < table.ncolumns; col++)
    {
      अक्षर *cell;
      अक्षर स्थिर *header = _(table.columns[col]->caption);

      अगर (table.columns[col]->field == आकार_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          अक्षर buf[LONGEST_HUMAN_READABLE + 1];

          पूर्णांक opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_आकार;
          uintmax_t q1024 = output_block_आकार;
          bool divisible_by_1000;
          bool divisible_by_1024;

          करो
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          जबतक (divisible_by_1000 & divisible_by_1024);

          अगर (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          अगर (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          अगर (! (opts & human_base_1024))
            opts |= human_B;

          अक्षर *अंक = human_readable (output_block_आकार, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), अंक, header);
        }
      अन्यथा अगर (header_mode == POSIX_MODE
               && table.columns[col]->field == आकार_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_आकार, header);
        }
      अन्यथा
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      पूर्णांक cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
अटल bool
selected_fstype (अक्षर स्थिर *fstype)
{
  अगर (fs_select_list == शून्य || fstype == शून्य)
    वापस true;
  क्रम (स्थिर काष्ठा fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_अग्र)
    अगर (streq (fstype, fsp->fs_name))
      वापस true;
  वापस false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
अटल bool
excluded_fstype (अक्षर स्थिर *fstype)
{
  अगर (fs_exclude_list == शून्य || fstype == शून्य)
    वापस false;
  क्रम (स्थिर काष्ठा fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_अग्र)
    अगर (streq (fstype, fsp->fs_name))
      वापस true;
  वापस false;
}

अटल माप_प्रकार
devlist_hash (व्योम स्थिर *x, माप_प्रकार table_आकार)
{
  काष्ठा devlist स्थिर *p = x;
  वापस (uintmax_t) {p->dev_अंक} % table_आकार;
}

अटल bool
devlist_compare (व्योम स्थिर *x, व्योम स्थिर *y)
{
  काष्ठा devlist स्थिर *a = x;
  काष्ठा devlist स्थिर *b = y;
  वापस a->dev_अंक == b->dev_अंक;
}

अटल काष्ठा devlist *
devlist_for_dev (dev_t dev)
{
  अगर (devlist_table == शून्य)
    वापस शून्य;
  काष्ठा devlist dev_entry;
  dev_entry.dev_अंक = dev;

  काष्ठा devlist *found = hash_lookup (devlist_table, &dev_entry);
  अगर (found == शून्य)
    वापस शून्य;

  /* Return the last devlist entry we have seen with this dev_num */
  वापस found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

अटल व्योम
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  काष्ठा devlist *device_list = शून्य;
  पूर्णांक mount_list_आकार = 0;

  क्रम (काष्ठा mount_entry *me = mount_list; me; me = me->me_अग्र)
    mount_list_आकार++;

  devlist_table = hash_initialize (mount_list_आकार, शून्य,
                                   devlist_hash, devlist_compare, शून्य);
  अगर (devlist_table == शून्य)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  क्रम (काष्ठा mount_entry *me = mount_list; me;)
    {
      काष्ठा stat buf;
      काष्ठा mount_entry *discard_me = शून्य;
      काष्ठा devlist *last_seen_dev = शून्य, *seen_dev = शून्य;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      अगर ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      अन्यथा
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          क्रम (; seen_dev && ! discard_me; seen_dev = seen_dev->अग्र_same_dev)
            {
              bool target_nearer_मूल = म_माप (seen_dev->me->me_mountdir)
                                        > म_माप (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_मूल = seen_dev->me->me_mntroot != शून्य
                                       && me->me_mntroot != शून्य
                                       && (म_माप (seen_dev->me->me_mntroot)
                                           < म_माप (me->me_mntroot));
              अगर (! print_grand_योग
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              अन्यथा अगर ((म_अक्षर (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! म_अक्षर (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_मूल && ! source_below_मूल)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (seen_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     seen_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = seen_dev->me;
                  seen_dev->me = me;
                }
              अन्यथा
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      अगर (discard_me)
        {
          me = me->me_अग्र;
          अगर (! devices_only)
            free_mount_entry (discard_me);
        }
      अन्यथा
        {
          /* Add the device number to the device_table.  */
          काष्ठा devlist *devlist = xmalloc (माप *devlist);
          devlist->me = me;
          devlist->dev_अंक = buf.st_dev;
          devlist->अग्र_same_dev = last_seen_dev;
          devlist->अग्र = device_list;
          device_list = devlist;

          काष्ठा devlist *hash_entry = hash_जोड़ो (devlist_table, devlist);
          अगर (hash_entry == शून्य)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_अग्र;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  अगर (! devices_only)
    {
      mount_list = शून्य;
      जबतक (device_list)
        {
          /* Add the mount entry.  */
          काष्ठा mount_entry *me = device_list->me;
          me->me_अग्र = mount_list;
          mount_list = me;
          काष्ठा devlist *अग्र = device_list->अग्र;
          मुक्त (device_list);
          device_list = अग्र;
        }
      hash_free (devlist_table);
      devlist_table = शून्य;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
अटल काष्ठा mount_entry स्थिर *
me_for_dev (dev_t dev)
{
  काष्ठा devlist *dl = devlist_for_dev (dev);
  अगर (dl)
        वापस dl->me;

  वापस शून्य;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
अटल bool
known_मूल्य (uintmax_t n)
{
  वापस n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

अटल अक्षर स्थिर *
df_readable (bool negative, uintmax_t n, अक्षर *buf,
             uintmax_t निवेश_units, uintmax_t output_units)
{
  अगर (! known_मूल्य (n) && !negative)
    वापस "-";
  अन्यथा
    {
      अक्षर *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, निवेश_units, output_units);
      अगर (negative)
        *--p = '-';
      वापस p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
अटल व्योम
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  अगर (*dest_neg == src_neg)
    {
      *dest += src;
      वापस;
    }

  अगर (*dest_neg)
    *dest = -*dest;

  अगर (src_neg)
    src = -src;

  अगर (src < *dest)
    *dest -= src;
  अन्यथा
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  अगर (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
अटल bool
has_uuid_suffix (अक्षर स्थिर *s)
{
  माप_प्रकार len = म_माप (s);
  वापस (36 < len
          && म_अखोज (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
अटल व्योम
get_field_मूल्यावली (काष्ठा field_मूल्यावली_t *bv,
                  काष्ठा field_मूल्यावली_t *iv,
                  स्थिर काष्ठा fs_usage *fsu)
{
  /* Inode values.  */
  iv->निवेश_units = iv->output_units = 1;
  iv->योग_2 = fsu->fsu_files;
  iv->available = iv->available_to_मूल = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  अगर (known_मूल्य (iv->योग_2) && known_मूल्य (iv->available_to_मूल))
    {
      iv->used = iv->योग_2 - iv->available_to_मूल;
      iv->negate_used = (iv->योग_2 < iv->available_to_मूल);
    }

  /* Block values.  */
  bv->निवेश_units = fsu->fsu_blocksize;
  bv->output_units = output_block_आकार;
  bv->योग_2 = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_मूल = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_शीर्ष_bit_set
                         && known_मूल्य (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  अगर (known_मूल्य (bv->योग_2) && known_मूल्य (bv->available_to_मूल))
    {
      bv->used = bv->योग_2 - bv->available_to_मूल;
      bv->negate_used = (bv->योग_2 < bv->available_to_मूल);
    }
}

/* Add block and inode values to grand total.  */
अटल व्योम
add_to_grand_योग (काष्ठा field_मूल्यावली_t *bv, काष्ठा field_मूल्यावली_t *iv)
{
  अगर (known_मूल्य (iv->योग_2))
    grand_fsu.fsu_files += iv->योग_2;
  अगर (known_मूल्य (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  अगर (known_मूल्य (bv->योग_2))
    grand_fsu.fsu_blocks += bv->निवेश_units * bv->योग_2;
  अगर (known_मूल्य (bv->available_to_मूल))
    grand_fsu.fsu_bfree += bv->निवेश_units * bv->available_to_मूल;
  अगर (known_मूल्य (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_शीर्ष_bit_set,
                            bv->निवेश_units * bv->available,
                            bv->negate_available);
}

/* Obtain a space listing for the device with absolute file name DEVICE.
   MOUNT_POINT names the root of the file system on DEVICE.
   FILE is the name to list; if null, it defaults to "-".
   If STAT_FILE is non-null, it is the name of a file within the file
   system that the user originally asked for; this provides better
   diagnostics, and sometimes it provides better results on networked
   file systems that give different free-space results depending on
   where in the file system you probe.
   If FSTYPE is non-null, it is the type of the file system on DEVICE.
   DEVICE may be null -- certain systems may
   not be able to produce statistics in this case.
   ME_DUMMY and ME_REMOTE are the mount entry flags.
   Caller must set PROCESS_ALL to true when iterating over all entries, as
   when df is invoked with no non-option argument.  See below for details.  */

अटल व्योम
get_dev (अक्षर स्थिर *device, अक्षर स्थिर *mount_point, अक्षर स्थिर *file,
         अक्षर स्थिर *stat_file, अक्षर स्थिर *fstype,
         bool me_dummy, bool me_remote,
         स्थिर काष्ठा fs_usage *force_fsu,
         bool process_all)
{
  अगर (me_remote && show_local_fs)
    वापस;

  अगर (me_dummy && !show_all_fs && !show_listed_fs)
    वापस;

  अगर (!selected_fstype (fstype) || excluded_fstype (fstype))
    वापस;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  अगर (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    वापस;

  अगर (!stat_file)
    stat_file = mount_point;

  काष्ठा fs_usage fsu;
  अगर (force_fsu)
    fsu = *force_fsu;
  अन्यथा अगर (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      अगर (process_all && (त्रुटि_सं == EACCES || त्रुटि_सं == ENOENT))
        {
          अगर (! show_all_fs)
            वापस;

          fstype = "-";
          fsu.fsu_bavail_शीर्ष_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      अन्यथा
        {
          error (0, त्रुटि_सं, "%s", quotef (stat_file));
          exit_status = निकास_त्रुटि;
          वापस;
        }
    }
  अन्यथा अगर (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      काष्ठा stat sb;
      अगर (stat (stat_file, &sb) == 0)
        {
          काष्ठा mount_entry स्थिर * dev_me = me_for_dev (sb.st_dev);
          अगर (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_शीर्ष_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  अगर (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    वापस;

  अगर (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  अगर (! device)
    device = "-";		/* unknown */

  अगर (! file)
    file = "-";			/* unspecified */

  अक्षर *dev_name = xstrdup (device);
  अक्षर *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  अगर (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      मुक्त (dev_name);
      dev_name = resolved_dev;
    }

  अगर (! fstype)
    fstype = "-";		/* unknown */

  काष्ठा field_मूल्यावली_t block_मूल्यावली;
  काष्ठा field_मूल्यावली_t inode_मूल्यावली;
  get_field_मूल्यावली (&block_मूल्यावली, &inode_मूल्यावली, &fsu);

  /* Add to grand total unless processing grand total line.  */
  अगर (print_grand_योग && ! force_fsu)
    add_to_grand_योग (&block_मूल्यावली, &inode_मूल्यावली);

  क्रम (idx_t col = 0; col < table.ncolumns; col++)
    {
      अक्षर buf[LONGEST_HUMAN_READABLE + 2];
      अक्षर *cell;

      काष्ठा field_मूल्यावली_t *v;
      चयन (table.columns[col]->field_type)
        {
        हाल BLOCK_FLD:
          v = &block_मूल्यावली;
          अवरोध;
        हाल INODE_FLD:
          v = &inode_मूल्यावली;
          अवरोध;
        हाल OTHER_FLD:
          v = शून्य;
          अवरोध;
        शेष:
          affirm (!"bad field_type");
        }

      चयन (table.columns[col]->field)
        {
        हाल SOURCE_FIELD:
          cell = xstrdup (dev_name);
          अवरोध;

        हाल FSTYPE_FIELD:
          cell = xstrdup (fstype);
          अवरोध;

        हाल आकार_FIELD:
        हाल ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->योग_2, buf,
                                       v->निवेश_units, v->output_units));
          अवरोध;

        हाल USED_FIELD:
        हाल IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->निवेश_units, v->output_units));
          अवरोध;

        हाल AVAIL_FIELD:
        हाल IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->निवेश_units, v->output_units));
          अवरोध;

        हाल PCENT_FIELD:
        हाल IPCENT_FIELD:
          {
            द्विगुन pct = -1;
            अगर (! known_मूल्य (v->used) || ! known_मूल्य (v->available))
              ;
            अन्यथा अगर (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_योग = v->used + v->available;
                pct = u100 / nonroot_योग + (u100 % nonroot_योग != 0);
              }
            अन्यथा
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                द्विगुन u = v->negate_used ? - (द्विगुन) - v->used : v->used;
                द्विगुन a = v->negate_available
                           ? - (द्विगुन) - v->available : v->available;
                द्विगुन nonroot_योग = u + a;
                अगर (nonroot_योग)
                  {
                    दीर्घ पूर्णांक lipct = pct = u * 100 / nonroot_योग;
                    द्विगुन ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    अगर (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            अवरोध;
          }

        हाल FILE_FIELD:
          cell = xstrdup (file);
          अवरोध;

        हाल TARGET_FIELD:
#अगर_घोषित HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          अगर (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          अन्यथा अगर (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#पूर्ण_अगर
          cell = xstrdup (mount_point);
          अवरोध;

        हाल INVALID_FIELD:
        शेष:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      पूर्णांक cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  मुक्त (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
अटल अक्षर *
last_device_for_mount (अक्षर स्थिर *mount)
{
  काष्ठा mount_entry स्थिर *le = शून्य;

  क्रम (काष्ठा mount_entry स्थिर *me = mount_list; me; me = me->me_अग्र)
    {
      अगर (streq (me->me_mountdir, mount))
        le = me;
    }

  अगर (le)
    {
      अक्षर *devname = le->me_devname;
      अक्षर *canon_dev = canonicalize_file_name (devname);
      अगर (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        वापस canon_dev;
      मुक्त (canon_dev);
      वापस xstrdup (le->me_devname);
    }
  अन्यथा
    वापस शून्य;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
अटल bool
get_device (अक्षर स्थिर *device)
{
  काष्ठा mount_entry स्थिर *best_match = शून्य;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  अक्षर स्थिर *file = device;

  अक्षर *resolved = canonicalize_file_name (device);
  अगर (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  माप_प्रकार best_match_len = SIZE_MAX;
  क्रम (काष्ठा mount_entry स्थिर *me = mount_list; me; me = me->me_अग्र)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      अक्षर *devname = me->me_devname;
      अक्षर *canon_dev = canonicalize_file_name (me->me_devname);
      अगर (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      अगर (streq (device, devname))
        {
          अक्षर *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          माप_प्रकार len = म_माप (me->me_mountdir);

          अगर (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              काष्ठा stat device_stats;
              bool this_match_accessible = false;

              अगर (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              अगर (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  अगर (len == 1) /* Traditional root.  */
                    {
                      मुक्त (last_device);
                      मुक्त (canon_dev);
                      अवरोध;
                    }
                  अन्यथा
                    best_match_len = len;
                }
            }

          मुक्त (last_device);
        }

      मुक्त (canon_dev);
    }

  मुक्त (resolved);

  अगर (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, शून्य,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, शून्य, false);
      वापस true;
    }
  अन्यथा अगर (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = निकास_त्रुटि;
      वापस true;
    }

  वापस false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
अटल व्योम
get_point (अक्षर स्थिर *point, स्थिर काष्ठा stat *statp)
{
  काष्ठा stat device_stats;
  काष्ठा mount_entry स्थिर *best_match = शून्य;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  अक्षर *resolved = canonicalize_file_name (point);
  अगर (resolved && resolved[0] == '/')
    {
      माप_प्रकार resolved_len = म_माप (resolved);
      माप_प्रकार best_match_len = 0;

      क्रम (काष्ठा mount_entry *me = mount_list; me; me = me->me_अग्र)
        {
          अगर (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              माप_प्रकार len = म_माप (me->me_mountdir);
              अगर (best_match_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  best_match = me;
                  best_match_len = len;
                }
            }
        }
    }
  मुक्त (resolved);
  अगर (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = शून्य;

  अगर (! best_match)
    क्रम (काष्ठा mount_entry *me = mount_list; me; me = me->me_अग्र)
      {
        अगर (me->me_dev == (dev_t) -1)
          {
            अगर (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            अन्यथा
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                अगर (त्रुटि_सं == EIO)
                  {
                    error (0, त्रुटि_सं, "%s", quotef (me->me_mountdir));
                    exit_status = निकास_त्रुटि;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        अगर (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            अगर (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            अन्यथा
              best_match = me;
          }
      }

  अगर (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             शून्य, false);
  अन्यथा
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      अक्षर *mp = find_mount_point (point, statp);
      अगर (mp)
        {
          get_dev (शून्य, mp, point, शून्य, शून्य,
                   false, false, शून्य, false);
          मुक्त (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

अटल व्योम
get_entry (अक्षर स्थिर *name, काष्ठा stat स्थिर *statp)
{
  अगर ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    वापस;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

अटल व्योम
get_all_entries (व्योम)
{
  filter_mount_list (show_all_fs);

  क्रम (काष्ठा mount_entry *me = mount_list; me; me = me->me_अग्र)
    get_dev (me->me_devname, me->me_mountdir, शून्य, शून्य, me->me_type,
             me->me_dummy, me->me_remote, शून्य, true);
}

/* Add FSTYPE to the list of file system types to display.  */

अटल व्योम
add_fs_type (अक्षर स्थिर *fstype)
{
  काष्ठा fs_type_list *fsp;

  fsp = xmalloc (माप *fsp);
  fsp->fs_name = (अक्षर *) fstype;
  fsp->fs_अग्र = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

अटल व्योम
add_excluded_fs_type (अक्षर स्थिर *fstype)
{
  काष्ठा fs_type_list *fsp;

  fsp = xmalloc (माप *fsp);
  fsp->fs_name = (अक्षर *) fstype;
  fsp->fs_अग्र = fs_exclude_list;
  fs_exclude_list = fsp;
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
Show information about the file प्रणाली on which each खाता resides,\n\
or all file systems by शेष.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --block-आकार=आकार_2\n\
         scale sizes by आकार_2 before printing them; see आकार_2 format below;\n\
         E.g., '-BM' prints sizes in units of 1,048,576 bytes\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes in powers of 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes in powers of 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead of block usage\n\
"));
      oputs (_("\
  -k\n\
         like --block-आकार=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         limit listing to local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         करो not invoke sync before getting usage info (शेष)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields अगर FIELD_LIST is omitted\n\
"));
      oputs (_("\
  -P, --portability\n\
         use the POSIX output format\n\
"));
      oputs (_("\
      --sync\n\
         invoke sync before getting usage info\n\
"));
      oputs (_("\
      --योग_2\n\
         elide all entries insignificant to available space,\n\
         and produce a grand योग_2\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         limit listing to file systems of type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file प्रणाली type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         limit listing to file systems not of type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_आकार_note ();
      ख_माला_दो (_("\n\
FIELD_LIST is a comma-separated list of columns to be included.  Valid\n\
field names are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info page).\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  काष्ठा stat *stats = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  अक्षर स्थिर *संदेश_mut_excl = _("options %s and %s are mutually exclusive");

  जबतक (true)
    {
      पूर्णांक oi = -1;
      पूर्णांक c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'a':
          show_all_fs = true;
          अवरोध;
        हाल 'B':
          {
            क्रमागत strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_आकार);
            अगर (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          अवरोध;
        हाल 'i':
          अगर (header_mode == OUTPUT_MODE)
            {
              error (0, 0, संदेश_mut_excl, "-i", "--output");
              usage (निकास_त्रुटि);
            }
          header_mode = INODES_MODE;
          अवरोध;
        हाल 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_आकार = 1;
          अवरोध;
        हाल 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_आकार = 1;
          अवरोध;
        हाल 'k':
          human_output_opts = 0;
          output_block_आकार = 1024;
          अवरोध;
        हाल 'l':
          show_local_fs = true;
          अवरोध;
        हाल 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_आकार = 1024 * 1024;
          अवरोध;
        हाल 'T':
          अगर (header_mode == OUTPUT_MODE)
            {
              error (0, 0, संदेश_mut_excl, "-T", "--output");
              usage (निकास_त्रुटि);
            }
          print_type = true;
          अवरोध;
        हाल 'P':
          अगर (header_mode == OUTPUT_MODE)
            {
              error (0, 0, संदेश_mut_excl, "-P", "--output");
              usage (निकास_त्रुटि);
            }
          posix_format = true;
          अवरोध;
        हाल SYNC_OPTION:
          require_sync = true;
          अवरोध;
        हाल NO_SYNC_OPTION:
          require_sync = false;
          अवरोध;

        हाल 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        हाल 't':
          add_fs_type (optarg);
          अवरोध;

        हाल 'v':		/* For SysV compatibility.  */
          /* ignore */
          अवरोध;
        हाल 'x':
          add_excluded_fs_type (optarg);
          अवरोध;

        हाल OUTPUT_OPTION:
          अगर (header_mode == INODES_MODE)
            {
              error (0, 0, संदेश_mut_excl, "-i", "--output");
              usage (निकास_त्रुटि);
            }
          अगर (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, संदेश_mut_excl, "-P", "--output");
              usage (निकास_त्रुटि);
            }
          अगर (print_type)
            {
              error (0, 0, संदेश_mut_excl, "-T", "--output");
              usage (निकास_त्रुटि);
            }
          header_mode = OUTPUT_MODE;
          अगर (optarg)
            decode_output_arg (optarg);
          अवरोध;

        हाल योग_OPTION:
          print_grand_योग = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (human_output_opts == -1)
    {
      अगर (posix_format)
        {
          human_output_opts = 0;
          output_block_आकार = (दो_पर्या ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      अन्यथा
        human_options (दो_पर्या ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_आकार);
    }

  अगर (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  अन्यथा अगर (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  अन्यथा अगर (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    क्रम (काष्ठा fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_अग्र)
      {
        क्रम (काष्ठा fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_अग्र)
          {
            अगर (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                अवरोध;
              }
          }
      }
    अगर (match)
      वापस निकास_त्रुटि;
  }

  अगर (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, माप *stats);
      क्रम (पूर्णांक i = optind; i < argc; ++i)
        {
          पूर्णांक err = automount_stat_err (argv[i], &stats[i - optind]);
          अगर (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = निकास_त्रुटि;
              argv[i] = शून्य;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != शून्य
                            || fs_exclude_list != शून्य
                            || print_type
                            || field_तथ्य[FSTYPE_FIELD].used
                            || show_local_fs));

  अगर (mount_list == शून्य)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      पूर्णांक status = 0;
      अगर ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != शून्य
               || fs_exclude_list != शून्य))
        {
          status = निकास_त्रुटि;
        }
      अक्षर स्थिर *warning = (status == 0 ? _("Warning: ") : "");
      error (status, त्रुटि_सं, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  अगर (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  अगर (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      क्रम (पूर्णांक i = optind; i < argc; ++i)
        अगर (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  अन्यथा
    get_all_entries ();

  अगर (file_systems_processed)
    {
      अगर (print_grand_योग)
        get_dev ("total",
                 (field_तथ्य[SOURCE_FIELD].used ? "-" : "total"),
                 शून्य, शून्य, शून्य, false, false, &grand_fsu, false);

      print_table ();
    }
  अन्यथा
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      अगर (exit_status == निकास_सफल)
        error (निकास_त्रुटि, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
