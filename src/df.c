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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>
#شامل <c-ctype.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "canonicalize.h"
#شامل "fsusage.h"
#شامل "human.h"
#شامل "mbswidth.h"
#شامل "mountlist.h"
#شامل "quote.h"
#شامل "find-mount-point.h"
#شامل "hash.h"
#شامل "xstrtol-error.h"
#شامل "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ساختار devlist
{
  dev_t dev_رقم;
  ساختار mount_entry *me;
  ساختار devlist *next;
  ساختار devlist *next_same_dev;
  ساختار devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ایستا Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ایستا bool show_all_fs;

/* If true, show only local file systems.  */
ایستا bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ایستا bool show_listed_fs;

/* Human-readable options for output.  */
ایستا صحیح human_output_opts = -1;

/* The units to use when printing sizes.  */
ایستا uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
ایستا bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ایستا bool require_sync;

/* Desired exit status.  */
ایستا صحیح exit_status;

/* A file system type to display.  */

ساختار fs_type_list
{
  نویسه *fs_name;
  ساختار fs_type_list *fs_next;
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

ایستا ساختار fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ایستا ساختار fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ایستا ساختار mount_entry *mount_list;

/* If true, print file system type as well.  */
ایستا bool print_type;

/* If true, print a grand total at the end.  */
ایستا bool print_grand_total;

/* Grand total data.  */
ایستا ساختار fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ایستا شمارش
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
تعریفنوع شمارش
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
تعریفنوع شمارش
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
ساختار field_data_t
{
  display_field_t field;
  نویسه ثابت *arg;
  field_type_t field_type;
  نویسه ثابت *caption;/* NULL means use default header of this field.  */
  صحیح width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_راست;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
ایستا ساختار field_data_t field_data[] = {
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

ایستا نویسه ثابت *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ایستا ساختار
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  نویسه ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ساختار field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
ساختار field_مقادیر_t
{
  uintmax_t درون_داد_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_ریشه;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

ایستا ساختار option ثابت long_options[] =
{
  {"all", no_argument, پوچ, 'a'},
  {"block-size", required_argument, پوچ, 'B'},
  {"inodes", no_argument, پوچ, 'i'},
  {"human-readable", no_argument, پوچ, 'h'},
  {"si", no_argument, پوچ, 'H'},
  {"local", no_argument, پوچ, 'l'},
  {"output", optional_argument, پوچ, OUTPUT_OPTION},
  {"portability", no_argument, پوچ, 'P'},
  {"print-type", no_argument, پوچ, 'T'},
  {"sync", no_argument, پوچ, SYNC_OPTION},
  {"no-sync", no_argument, پوچ, NO_SYNC_OPTION},
  {"total", no_argument, پوچ, TOTAL_OPTION},
  {"type", required_argument, پوچ, 't'},
  {"exclude-type", required_argument, پوچ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ایستا صحیح
automount_stat_err (نویسه ثابت *file, ساختار stat *st)
{
  صحیح fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  اگر (fd < 0)
    {
      اگر (errno == ENOENT || errno == ENOTDIR)
        بازگشت errno;
      بازگشت stat (file, st) == 0 ? 0 : errno;
    }
  وگرنه
    {
      صحیح err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      بازگشت err;
    }
}

شمارش { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ایستا تهی
replace_control_chars (نویسه *cell)
{
  نویسه *p = cell;
  وقتی (*p)
    {
      اگر (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ایستا تهی
replace_invalid_chars (نویسه *cell)
{
  نویسه *srcend = cell + strlen (cell);
  نویسه *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  برای (نویسه *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      اگر (ok)
        ok = !c32iscntrl (wc);
      وگرنه
        n = 1;

      اگر (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      وگرنه
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ایستا تهی
replace_problematic_chars (نویسه *cell)
{
  ایستا صحیح tty_out = -1;
  اگر (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ایستا تهی
alloc_table_row (تهی)
{
  اگر (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          اندازه *table.rows);
  idx_t ثابت row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, اندازه *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ایستا تهی
print_table (تهی)
{
  برای (idx_t row = 0; row < table.nrows; row++)
    {
      برای (idx_t col = 0; col < table.ncolumns; col++)
        {
          نویسه *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          اگر (col != 0)
            putchar (' ');

          صحیح width = mbswidth (cell, MBSWIDTH_FLAGS);
          صحیح fill = width < 0 ? 0 : table.columns[col]->width - width;
          اگر (table.columns[col]->align_راست)
            برای (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          اگر (col + 1 < table.ncolumns)
            برای (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ایستا تهی
alloc_field (صحیح f, نویسه ثابت *c)
{
  اگر (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             اندازه *table.columns);
  idx_t ثابت column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  اگر (c != پوچ)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ایستا تهی
decode_output_arg (نویسه ثابت *arg)
{
  نویسه *arg_writable = xstrdup (arg);
  نویسه *s = arg_writable;
  انجامبده
    {
      /* find next comma */
      نویسه *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      اگر (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      برای (idx_t i = 0; i < countof (field_data); i++)
        {
          اگر (streq (field_data[i].arg, s))
            {
              field = i;
              بشکن;
            }
        }
      اگر (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      اگر (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      انتخاب (field)
        {
        حالت SOURCE_FIELD:
        حالت FSTYPE_FIELD:
        حالت USED_FIELD:
        حالت PCENT_FIELD:
        حالت ITOTAL_FIELD:
        حالت IUSED_FIELD:
        حالت IAVAIL_FIELD:
        حالت IPCENT_FIELD:
        حالت TARGET_FIELD:
        حالت FILE_FIELD:
          alloc_field (field, پوچ);
          بشکن;

        حالت SIZE_FIELD:
          alloc_field (field, N_("Size"));
          بشکن;

        حالت AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          بشکن;

        حالت INVALID_FIELD:
        پیشفرض:
          affirm (!"invalid field");
        }
      s = comma;
    }
  وقتی (s);

  آزاد (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ایستا تهی
get_field_list (تهی)
{
  انتخاب (header_mode)
    {
    حالت DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, پوچ);
      اگر (print_type)
        alloc_field (FSTYPE_FIELD, پوچ);
      alloc_field (SIZE_FIELD,   پوچ);
      alloc_field (USED_FIELD,   پوچ);
      alloc_field (AVAIL_FIELD,  پوچ);
      alloc_field (PCENT_FIELD,  پوچ);
      alloc_field (TARGET_FIELD, پوچ);
      بشکن;

    حالت HUMAN_MODE:
      alloc_field (SOURCE_FIELD, پوچ);
      اگر (print_type)
        alloc_field (FSTYPE_FIELD, پوچ);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   پوچ);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  پوچ);
      alloc_field (TARGET_FIELD, پوچ);
      بشکن;

    حالت INODES_MODE:
      alloc_field (SOURCE_FIELD, پوچ);
      اگر (print_type)
        alloc_field (FSTYPE_FIELD, پوچ);
      alloc_field (ITOTAL_FIELD,  پوچ);
      alloc_field (IUSED_FIELD,   پوچ);
      alloc_field (IAVAIL_FIELD,  پوچ);
      alloc_field (IPCENT_FIELD,  پوچ);
      alloc_field (TARGET_FIELD,  پوچ);
      بشکن;

    حالت POSIX_MODE:
      alloc_field (SOURCE_FIELD, پوچ);
      اگر (print_type)
        alloc_field (FSTYPE_FIELD, پوچ);
      alloc_field (SIZE_FIELD,   پوچ);
      alloc_field (USED_FIELD,   پوچ);
      alloc_field (AVAIL_FIELD,  پوچ);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, پوچ);
      بشکن;

    حالت OUTPUT_MODE:
      اگر (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      بشکن;

    پیشفرض:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ایستا تهی
get_header (تهی)
{
  alloc_table_row ();

  برای (idx_t col = 0; col < table.ncolumns; col++)
    {
      نویسه *cell;
      نویسه ثابت *header = _(table.columns[col]->caption);

      اگر (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          نویسه buf[LONGEST_HUMAN_READABLE + 1];

          صحیح opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          انجامبده
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          وقتی (divisible_by_1000 & divisible_by_1024);

          اگر (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          اگر (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          اگر (! (opts & human_base_1024))
            opts |= human_B;

          نویسه *رقم = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), رقم, header);
        }
      وگرنه اگر (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      وگرنه
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      صحیح cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
ایستا bool
selected_fstype (نویسه ثابت *fstype)
{
  اگر (fs_select_list == پوچ || fstype == پوچ)
    بازگشت true;
  برای (ثابت ساختار fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    اگر (streq (fstype, fsp->fs_name))
      بازگشت true;
  بازگشت false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
ایستا bool
excluded_fstype (نویسه ثابت *fstype)
{
  اگر (fs_exclude_list == پوچ || fstype == پوچ)
    بازگشت false;
  برای (ثابت ساختار fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    اگر (streq (fstype, fsp->fs_name))
      بازگشت true;
  بازگشت false;
}

ایستا size_t
devlist_hash (تهی ثابت *x, size_t table_size)
{
  ساختار devlist ثابت *p = x;
  بازگشت (uintmax_t) {p->dev_رقم} % table_size;
}

ایستا bool
devlist_compare (تهی ثابت *x, تهی ثابت *y)
{
  ساختار devlist ثابت *a = x;
  ساختار devlist ثابت *b = y;
  بازگشت a->dev_رقم == b->dev_رقم;
}

ایستا ساختار devlist *
devlist_for_dev (dev_t dev)
{
  اگر (devlist_table == پوچ)
    بازگشت پوچ;
  ساختار devlist dev_entry;
  dev_entry.dev_رقم = dev;

  ساختار devlist *found = hash_lookup (devlist_table, &dev_entry);
  اگر (found == پوچ)
    بازگشت پوچ;

  /* Return the last devlist entry we have seen with this dev_num */
  بازگشت found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ایستا تهی
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ساختار devlist *device_list = پوچ;
  صحیح mount_list_size = 0;

  برای (ساختار mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, پوچ,
                                   devlist_hash, devlist_compare, پوچ);
  اگر (devlist_table == پوچ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  برای (ساختار mount_entry *me = mount_list; me;)
    {
      ساختار stat buf;
      ساختار mount_entry *discard_me = پوچ;
      ساختار devlist *last_seen_dev = پوچ, *seen_dev = پوچ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      اگر ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      وگرنه
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          برای (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_ریشه = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_ریشه = seen_dev->me->me_mntroot != پوچ
                                       && me->me_mntroot != پوچ
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              اگر (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              وگرنه اگر ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_ریشه && ! source_below_ریشه)
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
              وگرنه
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      اگر (discard_me)
        {
          me = me->me_next;
          اگر (! devices_only)
            free_mount_entry (discard_me);
        }
      وگرنه
        {
          /* Add the device number to the device_table.  */
          ساختار devlist *devlist = xmalloc (اندازه *devlist);
          devlist->me = me;
          devlist->dev_رقم = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          ساختار devlist *hash_entry = hash_درج (devlist_table, devlist);
          اگر (hash_entry == پوچ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  اگر (! devices_only)
    {
      mount_list = پوچ;
      وقتی (device_list)
        {
          /* Add the mount entry.  */
          ساختار mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          ساختار devlist *next = device_list->next;
          آزاد (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = پوچ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
ایستا ساختار mount_entry ثابت *
me_for_dev (dev_t dev)
{
  ساختار devlist *dl = devlist_for_dev (dev);
  اگر (dl)
        بازگشت dl->me;

  بازگشت پوچ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ایستا bool
known_مقدار (uintmax_t n)
{
  بازگشت n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ایستا نویسه ثابت *
df_readable (bool negative, uintmax_t n, نویسه *buf,
             uintmax_t درون_داد_units, uintmax_t output_units)
{
  اگر (! known_مقدار (n) && !negative)
    بازگشت "-";
  وگرنه
    {
      نویسه *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, درون_داد_units, output_units);
      اگر (negative)
        *--p = '-';
      بازگشت p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ایستا تهی
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  اگر (*dest_neg == src_neg)
    {
      *dest += src;
      بازگشت;
    }

  اگر (*dest_neg)
    *dest = -*dest;

  اگر (src_neg)
    src = -src;

  اگر (src < *dest)
    *dest -= src;
  وگرنه
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  اگر (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
ایستا bool
has_uuid_suffix (نویسه ثابت *s)
{
  size_t len = strlen (s);
  بازگشت (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ایستا تهی
get_field_مقادیر (ساختار field_مقادیر_t *bv,
                  ساختار field_مقادیر_t *iv,
                  ثابت ساختار fs_usage *fsu)
{
  /* Inode values.  */
  iv->درون_داد_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_ریشه = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  اگر (known_مقدار (iv->total) && known_مقدار (iv->available_to_ریشه))
    {
      iv->used = iv->total - iv->available_to_ریشه;
      iv->negate_used = (iv->total < iv->available_to_ریشه);
    }

  /* Block values.  */
  bv->درون_داد_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_ریشه = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_بالاترین_bit_set
                         && known_مقدار (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  اگر (known_مقدار (bv->total) && known_مقدار (bv->available_to_ریشه))
    {
      bv->used = bv->total - bv->available_to_ریشه;
      bv->negate_used = (bv->total < bv->available_to_ریشه);
    }
}

/* Add block and inode values to grand total.  */
ایستا تهی
add_to_grand_total (ساختار field_مقادیر_t *bv, ساختار field_مقادیر_t *iv)
{
  اگر (known_مقدار (iv->total))
    grand_fsu.fsu_files += iv->total;
  اگر (known_مقدار (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  اگر (known_مقدار (bv->total))
    grand_fsu.fsu_blocks += bv->درون_داد_units * bv->total;
  اگر (known_مقدار (bv->available_to_ریشه))
    grand_fsu.fsu_bfree += bv->درون_داد_units * bv->available_to_ریشه;
  اگر (known_مقدار (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_بالاترین_bit_set,
                            bv->درون_داد_units * bv->available,
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

ایستا تهی
get_dev (نویسه ثابت *device, نویسه ثابت *mount_point, نویسه ثابت *file,
         نویسه ثابت *stat_file, نویسه ثابت *fstype,
         bool me_dummy, bool me_remote,
         ثابت ساختار fs_usage *force_fsu,
         bool process_all)
{
  اگر (me_remote && show_local_fs)
    بازگشت;

  اگر (me_dummy && !show_all_fs && !show_listed_fs)
    بازگشت;

  اگر (!selected_fstype (fstype) || excluded_fstype (fstype))
    بازگشت;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  اگر (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    بازگشت;

  اگر (!stat_file)
    stat_file = mount_point;

  ساختار fs_usage fsu;
  اگر (force_fsu)
    fsu = *force_fsu;
  وگرنه اگر (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      اگر (process_all && (errno == EACCES || errno == ENOENT))
        {
          اگر (! show_all_fs)
            بازگشت;

          fstype = "-";
          fsu.fsu_bavail_بالاترین_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      وگرنه
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          بازگشت;
        }
    }
  وگرنه اگر (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ساختار stat sb;
      اگر (stat (stat_file, &sb) == 0)
        {
          ساختار mount_entry ثابت * dev_me = me_for_dev (sb.st_dev);
          اگر (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_بالاترین_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  اگر (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    بازگشت;

  اگر (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  اگر (! device)
    device = "-";		/* unknown */

  اگر (! file)
    file = "-";			/* unspecified */

  نویسه *dev_name = xstrdup (device);
  نویسه *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  اگر (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      آزاد (dev_name);
      dev_name = resolved_dev;
    }

  اگر (! fstype)
    fstype = "-";		/* unknown */

  ساختار field_مقادیر_t block_مقادیر;
  ساختار field_مقادیر_t inode_مقادیر;
  get_field_مقادیر (&block_مقادیر, &inode_مقادیر, &fsu);

  /* Add to grand total unless processing grand total line.  */
  اگر (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_مقادیر, &inode_مقادیر);

  برای (idx_t col = 0; col < table.ncolumns; col++)
    {
      نویسه buf[LONGEST_HUMAN_READABLE + 2];
      نویسه *cell;

      ساختار field_مقادیر_t *v;
      انتخاب (table.columns[col]->field_type)
        {
        حالت BLOCK_FLD:
          v = &block_مقادیر;
          بشکن;
        حالت INODE_FLD:
          v = &inode_مقادیر;
          بشکن;
        حالت OTHER_FLD:
          v = پوچ;
          بشکن;
        پیشفرض:
          affirm (!"bad field_type");
        }

      انتخاب (table.columns[col]->field)
        {
        حالت SOURCE_FIELD:
          cell = xstrdup (dev_name);
          بشکن;

        حالت FSTYPE_FIELD:
          cell = xstrdup (fstype);
          بشکن;

        حالت SIZE_FIELD:
        حالت ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->درون_داد_units, v->output_units));
          بشکن;

        حالت USED_FIELD:
        حالت IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->درون_داد_units, v->output_units));
          بشکن;

        حالت AVAIL_FIELD:
        حالت IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->درون_داد_units, v->output_units));
          بشکن;

        حالت PCENT_FIELD:
        حالت IPCENT_FIELD:
          {
            دوگانه pct = -1;
            اگر (! known_مقدار (v->used) || ! known_مقدار (v->available))
              ;
            وگرنه اگر (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            وگرنه
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                دوگانه u = v->negate_used ? - (دوگانه) - v->used : v->used;
                دوگانه a = v->negate_available
                           ? - (دوگانه) - v->available : v->available;
                دوگانه nonroot_total = u + a;
                اگر (nonroot_total)
                  {
                    long صحیح lipct = pct = u * 100 / nonroot_total;
                    دوگانه ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    اگر (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            بشکن;
          }

        حالت FILE_FIELD:
          cell = xstrdup (file);
          بشکن;

        حالت TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          اگر (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          وگرنه اگر (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          بشکن;

        حالت INVALID_FIELD:
        پیشفرض:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحیح cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  آزاد (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ایستا نویسه *
last_device_for_mount (نویسه ثابت *mount)
{
  ساختار mount_entry ثابت *le = پوچ;

  برای (ساختار mount_entry ثابت *me = mount_list; me; me = me->me_next)
    {
      اگر (streq (me->me_mountdir, mount))
        le = me;
    }

  اگر (le)
    {
      نویسه *devname = le->me_devname;
      نویسه *canon_dev = canonicalize_file_name (devname);
      اگر (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        بازگشت canon_dev;
      آزاد (canon_dev);
      بازگشت xstrdup (le->me_devname);
    }
  وگرنه
    بازگشت پوچ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ایستا bool
get_device (نویسه ثابت *device)
{
  ساختار mount_entry ثابت *best_match = پوچ;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  نویسه ثابت *file = device;

  نویسه *resolved = canonicalize_file_name (device);
  اگر (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  برای (ساختار mount_entry ثابت *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      نویسه *devname = me->me_devname;
      نویسه *canon_dev = canonicalize_file_name (me->me_devname);
      اگر (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      اگر (streq (device, devname))
        {
          نویسه *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          اگر (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              ساختار stat device_stats;
              bool this_match_accessible = false;

              اگر (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              اگر (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  اگر (len == 1) /* Traditional root.  */
                    {
                      آزاد (last_device);
                      آزاد (canon_dev);
                      بشکن;
                    }
                  وگرنه
                    best_match_len = len;
                }
            }

          آزاد (last_device);
        }

      آزاد (canon_dev);
    }

  آزاد (resolved);

  اگر (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, پوچ,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, پوچ, false);
      بازگشت true;
    }
  وگرنه اگر (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      بازگشت true;
    }

  بازگشت false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ایستا تهی
get_point (نویسه ثابت *point, ثابت ساختار stat *statp)
{
  ساختار stat device_stats;
  ساختار mount_entry ثابت *best_match = پوچ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  نویسه *resolved = canonicalize_file_name (point);
  اگر (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      برای (ساختار mount_entry *me = mount_list; me; me = me->me_next)
        {
          اگر (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              اگر (best_match_len <= len && len <= resolved_len
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
  آزاد (resolved);
  اگر (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = پوچ;

  اگر (! best_match)
    برای (ساختار mount_entry *me = mount_list; me; me = me->me_next)
      {
        اگر (me->me_dev == (dev_t) -1)
          {
            اگر (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            وگرنه
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                اگر (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        اگر (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            اگر (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            وگرنه
              best_match = me;
          }
      }

  اگر (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             پوچ, false);
  وگرنه
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      نویسه *mp = find_mount_point (point, statp);
      اگر (mp)
        {
          get_dev (پوچ, mp, point, پوچ, پوچ,
                   false, false, پوچ, false);
          آزاد (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ایستا تهی
get_entry (نویسه ثابت *name, ساختار stat ثابت *statp)
{
  اگر ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    بازگشت;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ایستا تهی
get_all_entries (تهی)
{
  filter_mount_list (show_all_fs);

  برای (ساختار mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, پوچ, پوچ, me->me_type,
             me->me_dummy, me->me_remote, پوچ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ایستا تهی
add_fs_type (نویسه ثابت *fstype)
{
  ساختار fs_type_list *fsp;

  fsp = xmalloc (اندازه *fsp);
  fsp->fs_name = (نویسه *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ایستا تهی
add_excluded_fs_type (نویسه ثابت *fstype)
{
  ساختار fs_type_list *fsp;

  fsp = xmalloc (اندازه *fsp);
  fsp->fs_name = (نویسه *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by پیشفرض.\n\
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
         انجامبده not invoke sync before getting usage info (پیشفرض)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields اگر FIELD_LIST is omitted\n\
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  ساختار stat *stats = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  نویسه ثابت *پیام_mut_excl = _("options %s and %s are mutually exclusive");

  وقتی (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت 'a':
          show_all_fs = true;
          بشکن;
        حالت 'B':
          {
            شمارش strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          بشکن;
        حالت 'i':
          اگر (header_mode == OUTPUT_MODE)
            {
              error (0, 0, پیام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          بشکن;
        حالت 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          بشکن;
        حالت 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          بشکن;
        حالت 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          بشکن;
        حالت 'l':
          show_local_fs = true;
          بشکن;
        حالت 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          بشکن;
        حالت 'T':
          اگر (header_mode == OUTPUT_MODE)
            {
              error (0, 0, پیام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          بشکن;
        حالت 'P':
          اگر (header_mode == OUTPUT_MODE)
            {
              error (0, 0, پیام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          بشکن;
        حالت SYNC_OPTION:
          require_sync = true;
          بشکن;
        حالت NO_SYNC_OPTION:
          require_sync = false;
          بشکن;

        حالت 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        حالت 't':
          add_fs_type (optarg);
          بشکن;

        حالت 'v':		/* For SysV compatibility.  */
          /* ignore */
          بشکن;
        حالت 'x':
          add_excluded_fs_type (optarg);
          بشکن;

        حالت OUTPUT_OPTION:
          اگر (header_mode == INODES_MODE)
            {
              error (0, 0, پیام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          اگر (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, پیام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          اگر (print_type)
            {
              error (0, 0, پیام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          اگر (optarg)
            decode_output_arg (optarg);
          بشکن;

        حالت TOTAL_OPTION:
          print_grand_total = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (human_output_opts == -1)
    {
      اگر (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      وگرنه
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  اگر (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  وگرنه اگر (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  وگرنه اگر (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    برای (ساختار fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        برای (ساختار fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            اگر (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                بشکن;
              }
          }
      }
    اگر (match)
      بازگشت EXIT_FAILURE;
  }

  اگر (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, اندازه *stats);
      برای (صحیح i = optind; i < argc; ++i)
        {
          صحیح err = automount_stat_err (argv[i], &stats[i - optind]);
          اگر (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = پوچ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != پوچ
                            || fs_exclude_list != پوچ
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  اگر (mount_list == پوچ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      صحیح status = 0;
      اگر ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != پوچ
               || fs_exclude_list != پوچ))
        {
          status = EXIT_FAILURE;
        }
      نویسه ثابت *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  اگر (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  اگر (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      برای (صحیح i = optind; i < argc; ++i)
        اگر (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  وگرنه
    get_all_entries ();

  اگر (file_systems_processed)
    {
      اگر (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 پوچ, پوچ, پوچ, false, false, &grand_fsu, false);

      print_table ();
    }
  وگرنه
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      اگر (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
