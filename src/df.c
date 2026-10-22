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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>
#ضمن <c-ctype.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "canonicalize.h"
#ضمن "fsusage.h"
#ضمن "human.h"
#ضمن "mbswidth.h"
#ضمن "mountlist.h"
#ضمن "quote.h"
#ضمن "find-mount-point.h"
#ضمن "hash.h"
#ضمن "xstrtol-error.h"
#ضمن "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

بنية devlist
{
  dev_t dev_num;
  بنية mount_entry *me;
  بنية devlist *next;
  بنية devlist *next_same_dev;
  بنية devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ساكن Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ساكن bool show_all_fs;

/* If true, show only local file systems.  */
ساكن bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ساكن bool show_listed_fs;

/* Human-readable options for output.  */
ساكن صحيح human_output_opts = -1;

/* The units to use when printing sizes.  */
ساكن uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
ساكن bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ساكن bool require_sync;

/* Desired exit status.  */
ساكن صحيح exit_status;

/* A file system type to display.  */

بنية fs_type_list
{
  حرف *fs_name;
  بنية fs_type_list *fs_next;
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

ساكن بنية fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ساكن بنية fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ساكن بنية mount_entry *mount_list;

/* If true, print file system type as well.  */
ساكن bool print_type;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_total;

/* Grand total data.  */
ساكن بنية fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساكن تعداد
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
تعريفنوع تعداد
{
  SOURCE_FIELD, /* file system */
  FSTYPE_FIELD, /* FS type */
  SIZE_FIELD,   /* FS size */
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
تعريفنوع تعداد
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
بنية field_data_t
{
  display_field_t field;
  حرف ثابت *arg;
  field_type_t field_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحيح width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_right;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساكن بنية field_data_t field_data[] = {
  [SOURCE_FIELD] = { SOURCE_FIELD,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_FIELD] = { FSTYPE_FIELD,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [SIZE_FIELD] = { SIZE_FIELD,
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

ساكن حرف ثابت *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساكن بنية
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  بنية field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
بنية field_values_t
{
  uintmax_t input_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_root;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_argument, عدم, 'a'},
  {"block-size", required_argument, عدم, 'B'},
  {"inodes", no_argument, عدم, 'i'},
  {"human-readable", no_argument, عدم, 'h'},
  {"si", no_argument, عدم, 'H'},
  {"local", no_argument, عدم, 'l'},
  {"output", optional_argument, عدم, OUTPUT_OPTION},
  {"portability", no_argument, عدم, 'P'},
  {"print-type", no_argument, عدم, 'T'},
  {"sync", no_argument, عدم, SYNC_OPTION},
  {"no-sync", no_argument, عدم, NO_SYNC_OPTION},
  {"total", no_argument, عدم, TOTAL_OPTION},
  {"type", required_argument, عدم, 't'},
  {"exclude-type", required_argument, عدم, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ساكن صحيح
automount_stat_err (حرف ثابت *file, بنية stat *st)
{
  صحيح fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  אם (fd < 0)
    {
      אם (errno == ENOENT || errno == ENOTDIR)
        החזר errno;
      החזר stat (file, st) == 0 ? 0 : errno;
    }
  אחרת
    {
      صحيح err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      החזר err;
    }
}

تعداد { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ساكن فراغ
replace_control_chars (حرف *cell)
{
  حرف *p = cell;
  כלעוד (*p)
    {
      אם (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ساكن فراغ
replace_invalid_chars (حرف *cell)
{
  حرف *srcend = cell + strlen (cell);
  حرف *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  עבור (حرف *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      אם (ok)
        ok = !c32iscntrl (wc);
      אחרת
        n = 1;

      אם (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      אחרת
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ساكن فراغ
replace_problematic_chars (حرف *cell)
{
  ساكن صحيح tty_out = -1;
  אם (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساكن فراغ
alloc_table_row (فراغ)
{
  אם (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          حجم *table.rows);
  idx_t ثابت row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, حجم *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساكن فراغ
print_table (فراغ)
{
  עבור (idx_t row = 0; row < table.nrows; row++)
    {
      עבור (idx_t col = 0; col < table.ncolumns; col++)
        {
          حرف *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          אם (col != 0)
            putchar (' ');

          صحيح width = mbswidth (cell, MBSWIDTH_FLAGS);
          صحيح fill = width < 0 ? 0 : table.columns[col]->width - width;
          אם (table.columns[col]->align_right)
            עבור (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          אם (col + 1 < table.ncolumns)
            עבור (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساكن فراغ
alloc_field (صحيح f, حرف ثابت *c)
{
  אם (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             حجم *table.columns);
  idx_t ثابت column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  אם (c != عدم)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ساكن فراغ
decode_output_arg (حرف ثابت *arg)
{
  حرف *arg_writable = xstrdup (arg);
  حرف *s = arg_writable;
  עשה
    {
      /* find next comma */
      حرف *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      אם (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      עבור (idx_t i = 0; i < countof (field_data); i++)
        {
          אם (streq (field_data[i].arg, s))
            {
              field = i;
              اكسر;
            }
        }
      אם (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      אם (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      بدل (field)
        {
        حالة SOURCE_FIELD:
        حالة FSTYPE_FIELD:
        حالة USED_FIELD:
        حالة PCENT_FIELD:
        حالة ITOTAL_FIELD:
        حالة IUSED_FIELD:
        حالة IAVAIL_FIELD:
        حالة IPCENT_FIELD:
        حالة TARGET_FIELD:
        حالة FILE_FIELD:
          alloc_field (field, عدم);
          اكسر;

        حالة SIZE_FIELD:
          alloc_field (field, N_("Size"));
          اكسر;

        حالة AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          اكسر;

        حالة INVALID_FIELD:
        افتراضي:
          affirm (!"invalid field");
        }
      s = comma;
    }
  כלעוד (s);

  حرر (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ساكن فراغ
get_field_list (فراغ)
{
  بدل (header_mode)
    {
    حالة DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, عدم);
      אם (print_type)
        alloc_field (FSTYPE_FIELD, عدم);
      alloc_field (SIZE_FIELD,   عدم);
      alloc_field (USED_FIELD,   عدم);
      alloc_field (AVAIL_FIELD,  عدم);
      alloc_field (PCENT_FIELD,  عدم);
      alloc_field (TARGET_FIELD, عدم);
      اكسر;

    حالة HUMAN_MODE:
      alloc_field (SOURCE_FIELD, عدم);
      אם (print_type)
        alloc_field (FSTYPE_FIELD, عدم);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   عدم);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  عدم);
      alloc_field (TARGET_FIELD, عدم);
      اكسر;

    حالة INODES_MODE:
      alloc_field (SOURCE_FIELD, عدم);
      אם (print_type)
        alloc_field (FSTYPE_FIELD, عدم);
      alloc_field (ITOTAL_FIELD,  عدم);
      alloc_field (IUSED_FIELD,   عدم);
      alloc_field (IAVAIL_FIELD,  عدم);
      alloc_field (IPCENT_FIELD,  عدم);
      alloc_field (TARGET_FIELD,  عدم);
      اكسر;

    حالة POSIX_MODE:
      alloc_field (SOURCE_FIELD, عدم);
      אם (print_type)
        alloc_field (FSTYPE_FIELD, عدم);
      alloc_field (SIZE_FIELD,   عدم);
      alloc_field (USED_FIELD,   عدم);
      alloc_field (AVAIL_FIELD,  عدم);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, عدم);
      اكسر;

    حالة OUTPUT_MODE:
      אם (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      اكسر;

    افتراضي:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساكن فراغ
get_header (فراغ)
{
  alloc_table_row ();

  עבור (idx_t col = 0; col < table.ncolumns; col++)
    {
      حرف *cell;
      حرف ثابت *header = _(table.columns[col]->caption);

      אם (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحيح opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          עשה
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          כלעוד (divisible_by_1000 & divisible_by_1024);

          אם (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          אם (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          אם (! (opts & human_base_1024))
            opts |= human_B;

          حرف *num = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), num, header);
        }
      אחרת אם (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      אחרת
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      صحيح cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
ساكن bool
selected_fstype (حرف ثابت *fstype)
{
  אם (fs_select_list == عدم || fstype == عدم)
    החזר true;
  עבור (ثابت بنية fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    אם (streq (fstype, fsp->fs_name))
      החזר true;
  החזר false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
ساكن bool
excluded_fstype (حرف ثابت *fstype)
{
  אם (fs_exclude_list == عدم || fstype == عدم)
    החזר false;
  עבור (ثابت بنية fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    אם (streq (fstype, fsp->fs_name))
      החזר true;
  החזר false;
}

ساكن size_t
devlist_hash (فراغ ثابت *x, size_t table_size)
{
  بنية devlist ثابت *p = x;
  החזר (uintmax_t) {p->dev_num} % table_size;
}

ساكن bool
devlist_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية devlist ثابت *a = x;
  بنية devlist ثابت *b = y;
  החזר a->dev_num == b->dev_num;
}

ساكن بنية devlist *
devlist_for_dev (dev_t dev)
{
  אם (devlist_table == عدم)
    החזר عدم;
  بنية devlist dev_entry;
  dev_entry.dev_num = dev;

  بنية devlist *found = hash_lookup (devlist_table, &dev_entry);
  אם (found == عدم)
    החזר عدم;

  /* Return the last devlist entry we have seen with this dev_num */
  החזר found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ساكن فراغ
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  بنية devlist *device_list = عدم;
  صحيح mount_list_size = 0;

  עבור (بنية mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, عدم,
                                   devlist_hash, devlist_compare, عدم);
  אם (devlist_table == عدم)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  עבור (بنية mount_entry *me = mount_list; me;)
    {
      بنية stat buf;
      بنية mount_entry *discard_me = عدم;
      بنية devlist *last_seen_dev = عدم, *seen_dev = عدم;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      אם ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      אחרת
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          עבור (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_root = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_root = seen_dev->me->me_mntroot != عدم
                                       && me->me_mntroot != عدم
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              אם (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              אחרת אם ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_root && ! source_below_root)
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
              אחרת
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      אם (discard_me)
        {
          me = me->me_next;
          אם (! devices_only)
            free_mount_entry (discard_me);
        }
      אחרת
        {
          /* Add the device number to the device_table.  */
          بنية devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_num = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          بنية devlist *hash_entry = hash_insert (devlist_table, devlist);
          אם (hash_entry == عدم)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  אם (! devices_only)
    {
      mount_list = عدم;
      כלעוד (device_list)
        {
          /* Add the mount entry.  */
          بنية mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          بنية devlist *next = device_list->next;
          حرر (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = عدم;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
ساكن بنية mount_entry ثابت *
me_for_dev (dev_t dev)
{
  بنية devlist *dl = devlist_for_dev (dev);
  אם (dl)
        החזר dl->me;

  החזר عدم;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ساكن bool
known_value (uintmax_t n)
{
  החזר n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ساكن حرف ثابت *
df_readable (bool negative, uintmax_t n, حرف *buf,
             uintmax_t input_units, uintmax_t output_units)
{
  אם (! known_value (n) && !negative)
    החזר "-";
  אחרת
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, input_units, output_units);
      אם (negative)
        *--p = '-';
      החזר p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ساكن فراغ
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  אם (*dest_neg == src_neg)
    {
      *dest += src;
      החזר;
    }

  אם (*dest_neg)
    *dest = -*dest;

  אם (src_neg)
    src = -src;

  אם (src < *dest)
    *dest -= src;
  אחרת
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  אם (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
ساكن bool
has_uuid_suffix (حرف ثابت *s)
{
  size_t len = strlen (s);
  החזר (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساكن فراغ
get_field_values (بنية field_values_t *bv,
                  بنية field_values_t *iv,
                  ثابت بنية fs_usage *fsu)
{
  /* Inode values.  */
  iv->input_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_root = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  אם (known_value (iv->total) && known_value (iv->available_to_root))
    {
      iv->used = iv->total - iv->available_to_root;
      iv->negate_used = (iv->total < iv->available_to_root);
    }

  /* Block values.  */
  bv->input_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_root = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_top_bit_set
                         && known_value (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  אם (known_value (bv->total) && known_value (bv->available_to_root))
    {
      bv->used = bv->total - bv->available_to_root;
      bv->negate_used = (bv->total < bv->available_to_root);
    }
}

/* Add block and inode values to grand total.  */
ساكن فراغ
add_to_grand_total (بنية field_values_t *bv, بنية field_values_t *iv)
{
  אם (known_value (iv->total))
    grand_fsu.fsu_files += iv->total;
  אם (known_value (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  אם (known_value (bv->total))
    grand_fsu.fsu_blocks += bv->input_units * bv->total;
  אם (known_value (bv->available_to_root))
    grand_fsu.fsu_bfree += bv->input_units * bv->available_to_root;
  אם (known_value (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_top_bit_set,
                            bv->input_units * bv->available,
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

ساكن فراغ
get_dev (حرف ثابت *device, حرف ثابت *mount_point, حرف ثابت *file,
         حرف ثابت *stat_file, حرف ثابت *fstype,
         bool me_dummy, bool me_remote,
         ثابت بنية fs_usage *force_fsu,
         bool process_all)
{
  אם (me_remote && show_local_fs)
    החזר;

  אם (me_dummy && !show_all_fs && !show_listed_fs)
    החזר;

  אם (!selected_fstype (fstype) || excluded_fstype (fstype))
    החזר;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  אם (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    החזר;

  אם (!stat_file)
    stat_file = mount_point;

  بنية fs_usage fsu;
  אם (force_fsu)
    fsu = *force_fsu;
  אחרת אם (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      אם (process_all && (errno == EACCES || errno == ENOENT))
        {
          אם (! show_all_fs)
            החזר;

          fstype = "-";
          fsu.fsu_bavail_top_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      אחרת
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          החזר;
        }
    }
  אחרת אם (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      بنية stat sb;
      אם (stat (stat_file, &sb) == 0)
        {
          بنية mount_entry ثابت * dev_me = me_for_dev (sb.st_dev);
          אם (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_top_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  אם (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    החזר;

  אם (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  אם (! device)
    device = "-";		/* unknown */

  אם (! file)
    file = "-";			/* unspecified */

  حرف *dev_name = xstrdup (device);
  حرف *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  אם (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      حرر (dev_name);
      dev_name = resolved_dev;
    }

  אם (! fstype)
    fstype = "-";		/* unknown */

  بنية field_values_t block_values;
  بنية field_values_t inode_values;
  get_field_values (&block_values, &inode_values, &fsu);

  /* Add to grand total unless processing grand total line.  */
  אם (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_values, &inode_values);

  עבור (idx_t col = 0; col < table.ncolumns; col++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      بنية field_values_t *v;
      بدل (table.columns[col]->field_type)
        {
        حالة BLOCK_FLD:
          v = &block_values;
          اكسر;
        حالة INODE_FLD:
          v = &inode_values;
          اكسر;
        حالة OTHER_FLD:
          v = عدم;
          اكسر;
        افتراضي:
          affirm (!"bad field_type");
        }

      بدل (table.columns[col]->field)
        {
        حالة SOURCE_FIELD:
          cell = xstrdup (dev_name);
          اكسر;

        حالة FSTYPE_FIELD:
          cell = xstrdup (fstype);
          اكسر;

        حالة SIZE_FIELD:
        حالة ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->input_units, v->output_units));
          اكسر;

        حالة USED_FIELD:
        حالة IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->input_units, v->output_units));
          اكسر;

        حالة AVAIL_FIELD:
        حالة IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->input_units, v->output_units));
          اكسر;

        حالة PCENT_FIELD:
        حالة IPCENT_FIELD:
          {
            مزدوج pct = -1;
            אם (! known_value (v->used) || ! known_value (v->available))
              ;
            אחרת אם (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            אחרת
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                مزدوج u = v->negate_used ? - (مزدوج) - v->used : v->used;
                مزدوج a = v->negate_available
                           ? - (مزدوج) - v->available : v->available;
                مزدوج nonroot_total = u + a;
                אם (nonroot_total)
                  {
                    long صحيح lipct = pct = u * 100 / nonroot_total;
                    مزدوج ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    אם (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            اكسر;
          }

        حالة FILE_FIELD:
          cell = xstrdup (file);
          اكسر;

        حالة TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          אם (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          אחרת אם (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          اكسر;

        حالة INVALID_FIELD:
        افتراضي:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحيح cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  حرر (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساكن حرف *
last_device_for_mount (حرف ثابت *mount)
{
  بنية mount_entry ثابت *le = عدم;

  עבור (بنية mount_entry ثابت *me = mount_list; me; me = me->me_next)
    {
      אם (streq (me->me_mountdir, mount))
        le = me;
    }

  אם (le)
    {
      حرف *devname = le->me_devname;
      حرف *canon_dev = canonicalize_file_name (devname);
      אם (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        החזר canon_dev;
      حرر (canon_dev);
      החזר xstrdup (le->me_devname);
    }
  אחרת
    החזר عدم;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ساكن bool
get_device (حرف ثابت *device)
{
  بنية mount_entry ثابت *best_match = عدم;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  אם (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  עבור (بنية mount_entry ثابت *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      אם (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      אם (streq (device, devname))
        {
          حرف *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          אם (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              بنية stat device_stats;
              bool this_match_accessible = false;

              אם (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              אם (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  אם (len == 1) /* Traditional root.  */
                    {
                      حرر (last_device);
                      حرر (canon_dev);
                      اكسر;
                    }
                  אחרת
                    best_match_len = len;
                }
            }

          حرر (last_device);
        }

      حرر (canon_dev);
    }

  حرر (resolved);

  אם (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, عدم,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, عدم, false);
      החזר true;
    }
  אחרת אם (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      החזר true;
    }

  החזר false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ساكن فراغ
get_point (حرف ثابت *point, ثابت بنية stat *statp)
{
  بنية stat device_stats;
  بنية mount_entry ثابت *best_match = عدم;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (point);
  אם (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      עבור (بنية mount_entry *me = mount_list; me; me = me->me_next)
        {
          אם (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              אם (best_match_len <= len && len <= resolved_len
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
  حرر (resolved);
  אם (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = عدم;

  אם (! best_match)
    עבור (بنية mount_entry *me = mount_list; me; me = me->me_next)
      {
        אם (me->me_dev == (dev_t) -1)
          {
            אם (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            אחרת
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                אם (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        אם (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            אם (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            אחרת
              best_match = me;
          }
      }

  אם (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             عدم, false);
  אחרת
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = find_mount_point (point, statp);
      אם (mp)
        {
          get_dev (عدم, mp, point, عدم, عدم,
                   false, false, عدم, false);
          حرر (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساكن فراغ
get_entry (حرف ثابت *name, بنية stat ثابت *statp)
{
  אם ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    החזר;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساكن فراغ
get_all_entries (فراغ)
{
  filter_mount_list (show_all_fs);

  עבור (بنية mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, عدم, عدم, me->me_type,
             me->me_dummy, me->me_remote, عدم, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساكن فراغ
add_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساكن فراغ
add_excluded_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by افتراضي.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --block-size=SIZE\n\
         scale sizes by SIZE before printing them; see SIZE format below;\n\
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
         like --block-size=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         limit listing to local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         עשה not invoke sync before getting usage info (افتراضي)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields אם FIELD_LIST is omitted\n\
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
      --total\n\
         elide all entries insignificant to available space,\n\
         and produce a grand total\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         limit listing to file systems of type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
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
      emit_size_note ();
      fputs (_("\n\
FIELD_LIST is a comma-separated list of columns to be included.  Valid\n\
field names are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info page).\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  بنية stat *stats = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  حرف ثابت *msg_mut_excl = _("options %s and %s are mutually exclusive");

  כלעוד (true)
    {
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      אם (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'a':
          show_all_fs = true;
          اكسر;
        حالة 'B':
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            אם (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;
        حالة 'i':
          אם (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          اكسر;
        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          اكسر;
        حالة 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          اكسر;
        حالة 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          اكسر;
        حالة 'l':
          show_local_fs = true;
          اكسر;
        حالة 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          اكسر;
        حالة 'T':
          אם (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          اكسر;
        حالة 'P':
          אם (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          اكسر;
        حالة SYNC_OPTION:
          require_sync = true;
          اكسر;
        حالة NO_SYNC_OPTION:
          require_sync = false;
          اكسر;

        حالة 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        حالة 't':
          add_fs_type (optarg);
          اكسر;

        حالة 'v':		/* For SysV compatibility.  */
          /* ignore */
          اكسر;
        حالة 'x':
          add_excluded_fs_type (optarg);
          اكسر;

        حالة OUTPUT_OPTION:
          אם (header_mode == INODES_MODE)
            {
              error (0, 0, msg_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          אם (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, msg_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          אם (print_type)
            {
              error (0, 0, msg_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          אם (optarg)
            decode_output_arg (optarg);
          اكسر;

        حالة TOTAL_OPTION:
          print_grand_total = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  אם (human_output_opts == -1)
    {
      אם (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      אחרת
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  אם (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  אחרת אם (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  אחרת אם (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    עבור (بنية fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        עבור (بنية fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            אם (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                اكسر;
              }
          }
      }
    אם (match)
      החזר EXIT_FAILURE;
  }

  אם (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, حجم *stats);
      עבור (صحيح i = optind; i < argc; ++i)
        {
          صحيح err = automount_stat_err (argv[i], &stats[i - optind]);
          אם (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = عدم;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != عدم
                            || fs_exclude_list != عدم
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  אם (mount_list == عدم)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      صحيح status = 0;
      אם ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != عدم
               || fs_exclude_list != عدم))
        {
          status = EXIT_FAILURE;
        }
      حرف ثابت *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  אם (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  אם (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      עבור (صحيح i = optind; i < argc; ++i)
        אם (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  אחרת
    get_all_entries ();

  אם (file_systems_processed)
    {
      אם (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 عدم, عدم, عدم, false, false, &grand_fsu, false);

      print_table ();
    }
  אחרת
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      אם (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
