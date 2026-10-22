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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <c-ctype.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "canonicalize.h"
#அந்தர்பாவ "fsusage.h"
#அந்தர்பாவ "human.h"
#அந்தர்பாவ "mbswidth.h"
#அந்தர்பாவ "mountlist.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "find-mount-point.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "xstrtol-error.h"
#அந்தர்பாவ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

கட்டமைப்பு devlist
{
  dev_t dev_இலக்கம்;
  கட்டமைப்பு mount_entry *me;
  கட்டமைப்பு devlist *next;
  கட்டமைப்பு devlist *next_same_dev;
  கட்டமைப்பு devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
நிலையான Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
நிலையான bool show_all_fs;

/* If true, show only local file systems.  */
நிலையான bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
நிலையான bool show_listed_fs;

/* Human-readable options for output.  */
நிலையான முழு human_output_opts = -1;

/* The units to use when printing sizes.  */
நிலையான uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
நிலையான bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
நிலையான bool require_sync;

/* Desired exit status.  */
நிலையான முழு exit_status;

/* A file system type to display.  */

கட்டமைப்பு fs_type_list
{
  எழுத்து *fs_name;
  கட்டமைப்பு fs_type_list *fs_next;
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

நிலையான கட்டமைப்பு fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

நிலையான கட்டமைப்பு fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
நிலையான கட்டமைப்பு mount_entry *mount_list;

/* If true, print file system type as well.  */
நிலையான bool print_type;

/* If true, print a grand total at the end.  */
நிலையான bool print_grand_total;

/* Grand total data.  */
நிலையான கட்டமைப்பு fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
நிலையான எண்ணிக்கை
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
வகைவரை எண்ணிக்கை
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
வகைவரை எண்ணிக்கை
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
கட்டமைப்பு field_data_t
{
  display_field_t field;
  எழுத்து மாறிலி *arg;
  field_type_t field_type;
  எழுத்து மாறிலி *caption;/* NULL means use default header of this field.  */
  முழு width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_வலது;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
நிலையான கட்டமைப்பு field_data_t field_data[] = {
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

நிலையான எழுத்து மாறிலி *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

நிலையான கட்டமைப்பு
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  எழுத்து ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  கட்டமைப்பு field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
கட்டமைப்பு field_மதிப்புகள்_t
{
  uintmax_t உள்ளீடு_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_வேர்;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"all", no_argument, வெற்று, 'a'},
  {"block-size", required_argument, வெற்று, 'B'},
  {"inodes", no_argument, வெற்று, 'i'},
  {"human-readable", no_argument, வெற்று, 'h'},
  {"si", no_argument, வெற்று, 'H'},
  {"local", no_argument, வெற்று, 'l'},
  {"output", optional_argument, வெற்று, OUTPUT_OPTION},
  {"portability", no_argument, வெற்று, 'P'},
  {"print-type", no_argument, வெற்று, 'T'},
  {"sync", no_argument, வெற்று, SYNC_OPTION},
  {"no-sync", no_argument, வெற்று, NO_SYNC_OPTION},
  {"total", no_argument, வெற்று, TOTAL_OPTION},
  {"type", required_argument, வெற்று, 't'},
  {"exclude-type", required_argument, வெற்று, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

நிலையான முழு
automount_stat_err (எழுத்து மாறிலி *file, கட்டமைப்பு stat *st)
{
  முழு fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  எனில் (fd < 0)
    {
      எனில் (errno == ENOENT || errno == ENOTDIR)
        திரும்பு errno;
      திரும்பு stat (file, st) == 0 ? 0 : errno;
    }
  இல்லையெனில்
    {
      முழு err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      திரும்பு err;
    }
}

எண்ணிக்கை { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

நிலையான வெறுமை
replace_control_chars (எழுத்து *cell)
{
  எழுத்து *p = cell;
  போது (*p)
    {
      எனில் (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

நிலையான வெறுமை
replace_invalid_chars (எழுத்து *cell)
{
  எழுத்து *srcend = cell + strlen (cell);
  எழுத்து *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  க்கு (எழுத்து *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      எனில் (ok)
        ok = !c32iscntrl (wc);
      இல்லையெனில்
        n = 1;

      எனில் (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      இல்லையெனில்
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

நிலையான வெறுமை
replace_problematic_chars (எழுத்து *cell)
{
  நிலையான முழு tty_out = -1;
  எனில் (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

நிலையான வெறுமை
alloc_table_row (வெறுமை)
{
  எனில் (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          அளவு *table.rows);
  idx_t மாறிலி row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, அளவு *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

நிலையான வெறுமை
print_table (வெறுமை)
{
  க்கு (idx_t row = 0; row < table.nrows; row++)
    {
      க்கு (idx_t col = 0; col < table.ncolumns; col++)
        {
          எழுத்து *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          எனில் (col != 0)
            putchar (' ');

          முழு width = mbswidth (cell, MBSWIDTH_FLAGS);
          முழு fill = width < 0 ? 0 : table.columns[col]->width - width;
          எனில் (table.columns[col]->align_வலது)
            க்கு (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          எனில் (col + 1 < table.ncolumns)
            க்கு (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

நிலையான வெறுமை
alloc_field (முழு f, எழுத்து மாறிலி *c)
{
  எனில் (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             அளவு *table.columns);
  idx_t மாறிலி column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  எனில் (c != வெற்று)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
நிலையான வெறுமை
decode_output_arg (எழுத்து மாறிலி *arg)
{
  எழுத்து *arg_writable = xstrdup (arg);
  எழுத்து *s = arg_writable;
  செய்
    {
      /* find next comma */
      எழுத்து *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      எனில் (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      க்கு (idx_t i = 0; i < countof (field_data); i++)
        {
          எனில் (streq (field_data[i].arg, s))
            {
              field = i;
              நிறுத்து;
            }
        }
      எனில் (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      எனில் (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      தேர்வு (field)
        {
        நிலை SOURCE_FIELD:
        நிலை FSTYPE_FIELD:
        நிலை USED_FIELD:
        நிலை PCENT_FIELD:
        நிலை ITOTAL_FIELD:
        நிலை IUSED_FIELD:
        நிலை IAVAIL_FIELD:
        நிலை IPCENT_FIELD:
        நிலை TARGET_FIELD:
        நிலை FILE_FIELD:
          alloc_field (field, வெற்று);
          நிறுத்து;

        நிலை SIZE_FIELD:
          alloc_field (field, N_("Size"));
          நிறுத்து;

        நிலை AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          நிறுத்து;

        நிலை INVALID_FIELD:
        இயல்பு:
          affirm (!"invalid field");
        }
      s = comma;
    }
  போது (s);

  விடுவி (arg_writable);
}

/* Get the appropriate columns for the mode.  */
நிலையான வெறுமை
get_field_list (வெறுமை)
{
  தேர்வு (header_mode)
    {
    நிலை DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, வெற்று);
      எனில் (print_type)
        alloc_field (FSTYPE_FIELD, வெற்று);
      alloc_field (SIZE_FIELD,   வெற்று);
      alloc_field (USED_FIELD,   வெற்று);
      alloc_field (AVAIL_FIELD,  வெற்று);
      alloc_field (PCENT_FIELD,  வெற்று);
      alloc_field (TARGET_FIELD, வெற்று);
      நிறுத்து;

    நிலை HUMAN_MODE:
      alloc_field (SOURCE_FIELD, வெற்று);
      எனில் (print_type)
        alloc_field (FSTYPE_FIELD, வெற்று);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   வெற்று);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  வெற்று);
      alloc_field (TARGET_FIELD, வெற்று);
      நிறுத்து;

    நிலை INODES_MODE:
      alloc_field (SOURCE_FIELD, வெற்று);
      எனில் (print_type)
        alloc_field (FSTYPE_FIELD, வெற்று);
      alloc_field (ITOTAL_FIELD,  வெற்று);
      alloc_field (IUSED_FIELD,   வெற்று);
      alloc_field (IAVAIL_FIELD,  வெற்று);
      alloc_field (IPCENT_FIELD,  வெற்று);
      alloc_field (TARGET_FIELD,  வெற்று);
      நிறுத்து;

    நிலை POSIX_MODE:
      alloc_field (SOURCE_FIELD, வெற்று);
      எனில் (print_type)
        alloc_field (FSTYPE_FIELD, வெற்று);
      alloc_field (SIZE_FIELD,   வெற்று);
      alloc_field (USED_FIELD,   வெற்று);
      alloc_field (AVAIL_FIELD,  வெற்று);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, வெற்று);
      நிறுத்து;

    நிலை OUTPUT_MODE:
      எனில் (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      நிறுத்து;

    இயல்பு:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

நிலையான வெறுமை
get_header (வெறுமை)
{
  alloc_table_row ();

  க்கு (idx_t col = 0; col < table.ncolumns; col++)
    {
      எழுத்து *cell;
      எழுத்து மாறிலி *header = _(table.columns[col]->caption);

      எனில் (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          எழுத்து buf[LONGEST_HUMAN_READABLE + 1];

          முழு opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          செய்
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          போது (divisible_by_1000 & divisible_by_1024);

          எனில் (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          எனில் (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          எனில் (! (opts & human_base_1024))
            opts |= human_B;

          எழுத்து *இலக்கம் = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), இலக்கம், header);
        }
      இல்லையெனில் எனில் (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      இல்லையெனில்
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      முழு cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
நிலையான bool
selected_fstype (எழுத்து மாறிலி *fstype)
{
  எனில் (fs_select_list == வெற்று || fstype == வெற்று)
    திரும்பு true;
  க்கு (மாறிலி கட்டமைப்பு fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    எனில் (streq (fstype, fsp->fs_name))
      திரும்பு true;
  திரும்பு false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
நிலையான bool
excluded_fstype (எழுத்து மாறிலி *fstype)
{
  எனில் (fs_exclude_list == வெற்று || fstype == வெற்று)
    திரும்பு false;
  க்கு (மாறிலி கட்டமைப்பு fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    எனில் (streq (fstype, fsp->fs_name))
      திரும்பு true;
  திரும்பு false;
}

நிலையான size_t
devlist_hash (வெறுமை மாறிலி *x, size_t table_size)
{
  கட்டமைப்பு devlist மாறிலி *p = x;
  திரும்பு (uintmax_t) {p->dev_இலக்கம்} % table_size;
}

நிலையான bool
devlist_compare (வெறுமை மாறிலி *x, வெறுமை மாறிலி *y)
{
  கட்டமைப்பு devlist மாறிலி *a = x;
  கட்டமைப்பு devlist மாறிலி *b = y;
  திரும்பு a->dev_இலக்கம் == b->dev_இலக்கம்;
}

நிலையான கட்டமைப்பு devlist *
devlist_for_dev (dev_t dev)
{
  எனில் (devlist_table == வெற்று)
    திரும்பு வெற்று;
  கட்டமைப்பு devlist dev_entry;
  dev_entry.dev_இலக்கம் = dev;

  கட்டமைப்பு devlist *found = hash_lookup (devlist_table, &dev_entry);
  எனில் (found == வெற்று)
    திரும்பு வெற்று;

  /* Return the last devlist entry we have seen with this dev_num */
  திரும்பு found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

நிலையான வெறுமை
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  கட்டமைப்பு devlist *device_list = வெற்று;
  முழு mount_list_size = 0;

  க்கு (கட்டமைப்பு mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, வெற்று,
                                   devlist_hash, devlist_compare, வெற்று);
  எனில் (devlist_table == வெற்று)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  க்கு (கட்டமைப்பு mount_entry *me = mount_list; me;)
    {
      கட்டமைப்பு stat buf;
      கட்டமைப்பு mount_entry *discard_me = வெற்று;
      கட்டமைப்பு devlist *last_seen_dev = வெற்று, *seen_dev = வெற்று;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      எனில் ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      இல்லையெனில்
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          க்கு (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_வேர் = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_வேர் = seen_dev->me->me_mntroot != வெற்று
                                       && me->me_mntroot != வெற்று
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              எனில் (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              இல்லையெனில் எனில் ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_வேர் && ! source_below_வேர்)
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
              இல்லையெனில்
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      எனில் (discard_me)
        {
          me = me->me_next;
          எனில் (! devices_only)
            free_mount_entry (discard_me);
        }
      இல்லையெனில்
        {
          /* Add the device number to the device_table.  */
          கட்டமைப்பு devlist *devlist = xmalloc (அளவு *devlist);
          devlist->me = me;
          devlist->dev_இலக்கம் = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          கட்டமைப்பு devlist *hash_entry = hash_செருகு (devlist_table, devlist);
          எனில் (hash_entry == வெற்று)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  எனில் (! devices_only)
    {
      mount_list = வெற்று;
      போது (device_list)
        {
          /* Add the mount entry.  */
          கட்டமைப்பு mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          கட்டமைப்பு devlist *next = device_list->next;
          விடுவி (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = வெற்று;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
நிலையான கட்டமைப்பு mount_entry மாறிலி *
me_for_dev (dev_t dev)
{
  கட்டமைப்பு devlist *dl = devlist_for_dev (dev);
  எனில் (dl)
        திரும்பு dl->me;

  திரும்பு வெற்று;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
நிலையான bool
known_மதிப்பு (uintmax_t n)
{
  திரும்பு n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

நிலையான எழுத்து மாறிலி *
df_readable (bool negative, uintmax_t n, எழுத்து *buf,
             uintmax_t உள்ளீடு_units, uintmax_t output_units)
{
  எனில் (! known_மதிப்பு (n) && !negative)
    திரும்பு "-";
  இல்லையெனில்
    {
      எழுத்து *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, உள்ளீடு_units, output_units);
      எனில் (negative)
        *--p = '-';
      திரும்பு p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
நிலையான வெறுமை
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  எனில் (*dest_neg == src_neg)
    {
      *dest += src;
      திரும்பு;
    }

  எனில் (*dest_neg)
    *dest = -*dest;

  எனில் (src_neg)
    src = -src;

  எனில் (src < *dest)
    *dest -= src;
  இல்லையெனில்
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  எனில் (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
நிலையான bool
has_uuid_suffix (எழுத்து மாறிலி *s)
{
  size_t len = strlen (s);
  திரும்பு (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
நிலையான வெறுமை
get_field_மதிப்புகள் (கட்டமைப்பு field_மதிப்புகள்_t *bv,
                  கட்டமைப்பு field_மதிப்புகள்_t *iv,
                  மாறிலி கட்டமைப்பு fs_usage *fsu)
{
  /* Inode values.  */
  iv->உள்ளீடு_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_வேர் = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  எனில் (known_மதிப்பு (iv->total) && known_மதிப்பு (iv->available_to_வேர்))
    {
      iv->used = iv->total - iv->available_to_வேர்;
      iv->negate_used = (iv->total < iv->available_to_வேர்);
    }

  /* Block values.  */
  bv->உள்ளீடு_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_வேர் = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_உச்சி_bit_set
                         && known_மதிப்பு (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  எனில் (known_மதிப்பு (bv->total) && known_மதிப்பு (bv->available_to_வேர்))
    {
      bv->used = bv->total - bv->available_to_வேர்;
      bv->negate_used = (bv->total < bv->available_to_வேர்);
    }
}

/* Add block and inode values to grand total.  */
நிலையான வெறுமை
add_to_grand_total (கட்டமைப்பு field_மதிப்புகள்_t *bv, கட்டமைப்பு field_மதிப்புகள்_t *iv)
{
  எனில் (known_மதிப்பு (iv->total))
    grand_fsu.fsu_files += iv->total;
  எனில் (known_மதிப்பு (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  எனில் (known_மதிப்பு (bv->total))
    grand_fsu.fsu_blocks += bv->உள்ளீடு_units * bv->total;
  எனில் (known_மதிப்பு (bv->available_to_வேர்))
    grand_fsu.fsu_bfree += bv->உள்ளீடு_units * bv->available_to_வேர்;
  எனில் (known_மதிப்பு (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_உச்சி_bit_set,
                            bv->உள்ளீடு_units * bv->available,
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

நிலையான வெறுமை
get_dev (எழுத்து மாறிலி *device, எழுத்து மாறிலி *mount_point, எழுத்து மாறிலி *file,
         எழுத்து மாறிலி *stat_file, எழுத்து மாறிலி *fstype,
         bool me_dummy, bool me_remote,
         மாறிலி கட்டமைப்பு fs_usage *force_fsu,
         bool process_all)
{
  எனில் (me_remote && show_local_fs)
    திரும்பு;

  எனில் (me_dummy && !show_all_fs && !show_listed_fs)
    திரும்பு;

  எனில் (!selected_fstype (fstype) || excluded_fstype (fstype))
    திரும்பு;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  எனில் (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    திரும்பு;

  எனில் (!stat_file)
    stat_file = mount_point;

  கட்டமைப்பு fs_usage fsu;
  எனில் (force_fsu)
    fsu = *force_fsu;
  இல்லையெனில் எனில் (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      எனில் (process_all && (errno == EACCES || errno == ENOENT))
        {
          எனில் (! show_all_fs)
            திரும்பு;

          fstype = "-";
          fsu.fsu_bavail_உச்சி_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      இல்லையெனில்
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          திரும்பு;
        }
    }
  இல்லையெனில் எனில் (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      கட்டமைப்பு stat sb;
      எனில் (stat (stat_file, &sb) == 0)
        {
          கட்டமைப்பு mount_entry மாறிலி * dev_me = me_for_dev (sb.st_dev);
          எனில் (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_உச்சி_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  எனில் (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    திரும்பு;

  எனில் (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  எனில் (! device)
    device = "-";		/* unknown */

  எனில் (! file)
    file = "-";			/* unspecified */

  எழுத்து *dev_name = xstrdup (device);
  எழுத்து *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  எனில் (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      விடுவி (dev_name);
      dev_name = resolved_dev;
    }

  எனில் (! fstype)
    fstype = "-";		/* unknown */

  கட்டமைப்பு field_மதிப்புகள்_t block_மதிப்புகள்;
  கட்டமைப்பு field_மதிப்புகள்_t inode_மதிப்புகள்;
  get_field_மதிப்புகள் (&block_மதிப்புகள், &inode_மதிப்புகள், &fsu);

  /* Add to grand total unless processing grand total line.  */
  எனில் (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_மதிப்புகள், &inode_மதிப்புகள்);

  க்கு (idx_t col = 0; col < table.ncolumns; col++)
    {
      எழுத்து buf[LONGEST_HUMAN_READABLE + 2];
      எழுத்து *cell;

      கட்டமைப்பு field_மதிப்புகள்_t *v;
      தேர்வு (table.columns[col]->field_type)
        {
        நிலை BLOCK_FLD:
          v = &block_மதிப்புகள்;
          நிறுத்து;
        நிலை INODE_FLD:
          v = &inode_மதிப்புகள்;
          நிறுத்து;
        நிலை OTHER_FLD:
          v = வெற்று;
          நிறுத்து;
        இயல்பு:
          affirm (!"bad field_type");
        }

      தேர்வு (table.columns[col]->field)
        {
        நிலை SOURCE_FIELD:
          cell = xstrdup (dev_name);
          நிறுத்து;

        நிலை FSTYPE_FIELD:
          cell = xstrdup (fstype);
          நிறுத்து;

        நிலை SIZE_FIELD:
        நிலை ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->உள்ளீடு_units, v->output_units));
          நிறுத்து;

        நிலை USED_FIELD:
        நிலை IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->உள்ளீடு_units, v->output_units));
          நிறுத்து;

        நிலை AVAIL_FIELD:
        நிலை IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->உள்ளீடு_units, v->output_units));
          நிறுத்து;

        நிலை PCENT_FIELD:
        நிலை IPCENT_FIELD:
          {
            இரட்டை pct = -1;
            எனில் (! known_மதிப்பு (v->used) || ! known_மதிப்பு (v->available))
              ;
            இல்லையெனில் எனில் (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            இல்லையெனில்
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                இரட்டை u = v->negate_used ? - (இரட்டை) - v->used : v->used;
                இரட்டை a = v->negate_available
                           ? - (இரட்டை) - v->available : v->available;
                இரட்டை nonroot_total = u + a;
                எனில் (nonroot_total)
                  {
                    long முழு lipct = pct = u * 100 / nonroot_total;
                    இரட்டை ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    எனில் (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            நிறுத்து;
          }

        நிலை FILE_FIELD:
          cell = xstrdup (file);
          நிறுத்து;

        நிலை TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          எனில் (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          இல்லையெனில் எனில் (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          நிறுத்து;

        நிலை INVALID_FIELD:
        இயல்பு:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      முழு cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  விடுவி (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
நிலையான எழுத்து *
last_device_for_mount (எழுத்து மாறிலி *mount)
{
  கட்டமைப்பு mount_entry மாறிலி *le = வெற்று;

  க்கு (கட்டமைப்பு mount_entry மாறிலி *me = mount_list; me; me = me->me_next)
    {
      எனில் (streq (me->me_mountdir, mount))
        le = me;
    }

  எனில் (le)
    {
      எழுத்து *devname = le->me_devname;
      எழுத்து *canon_dev = canonicalize_file_name (devname);
      எனில் (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        திரும்பு canon_dev;
      விடுவி (canon_dev);
      திரும்பு xstrdup (le->me_devname);
    }
  இல்லையெனில்
    திரும்பு வெற்று;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
நிலையான bool
get_device (எழுத்து மாறிலி *device)
{
  கட்டமைப்பு mount_entry மாறிலி *best_match = வெற்று;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  எழுத்து மாறிலி *file = device;

  எழுத்து *resolved = canonicalize_file_name (device);
  எனில் (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  க்கு (கட்டமைப்பு mount_entry மாறிலி *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      எழுத்து *devname = me->me_devname;
      எழுத்து *canon_dev = canonicalize_file_name (me->me_devname);
      எனில் (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      எனில் (streq (device, devname))
        {
          எழுத்து *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          எனில் (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              கட்டமைப்பு stat device_stats;
              bool this_match_accessible = false;

              எனில் (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              எனில் (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  எனில் (len == 1) /* Traditional root.  */
                    {
                      விடுவி (last_device);
                      விடுவி (canon_dev);
                      நிறுத்து;
                    }
                  இல்லையெனில்
                    best_match_len = len;
                }
            }

          விடுவி (last_device);
        }

      விடுவி (canon_dev);
    }

  விடுவி (resolved);

  எனில் (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, வெற்று,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, வெற்று, false);
      திரும்பு true;
    }
  இல்லையெனில் எனில் (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      திரும்பு true;
    }

  திரும்பு false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
நிலையான வெறுமை
get_point (எழுத்து மாறிலி *point, மாறிலி கட்டமைப்பு stat *statp)
{
  கட்டமைப்பு stat device_stats;
  கட்டமைப்பு mount_entry மாறிலி *best_match = வெற்று;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  எழுத்து *resolved = canonicalize_file_name (point);
  எனில் (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      க்கு (கட்டமைப்பு mount_entry *me = mount_list; me; me = me->me_next)
        {
          எனில் (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              எனில் (best_match_len <= len && len <= resolved_len
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
  விடுவி (resolved);
  எனில் (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = வெற்று;

  எனில் (! best_match)
    க்கு (கட்டமைப்பு mount_entry *me = mount_list; me; me = me->me_next)
      {
        எனில் (me->me_dev == (dev_t) -1)
          {
            எனில் (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            இல்லையெனில்
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                எனில் (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        எனில் (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            எனில் (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            இல்லையெனில்
              best_match = me;
          }
      }

  எனில் (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             வெற்று, false);
  இல்லையெனில்
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      எழுத்து *mp = find_mount_point (point, statp);
      எனில் (mp)
        {
          get_dev (வெற்று, mp, point, வெற்று, வெற்று,
                   false, false, வெற்று, false);
          விடுவி (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

நிலையான வெறுமை
get_entry (எழுத்து மாறிலி *name, கட்டமைப்பு stat மாறிலி *statp)
{
  எனில் ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    திரும்பு;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

நிலையான வெறுமை
get_all_entries (வெறுமை)
{
  filter_mount_list (show_all_fs);

  க்கு (கட்டமைப்பு mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, வெற்று, வெற்று, me->me_type,
             me->me_dummy, me->me_remote, வெற்று, true);
}

/* Add FSTYPE to the list of file system types to display.  */

நிலையான வெறுமை
add_fs_type (எழுத்து மாறிலி *fstype)
{
  கட்டமைப்பு fs_type_list *fsp;

  fsp = xmalloc (அளவு *fsp);
  fsp->fs_name = (எழுத்து *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

நிலையான வெறுமை
add_excluded_fs_type (எழுத்து மாறிலி *fstype)
{
  கட்டமைப்பு fs_type_list *fsp;

  fsp = xmalloc (அளவு *fsp);
  fsp->fs_name = (எழுத்து *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by இயல்பு.\n\
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
         செய் not invoke sync before getting usage info (இயல்பு)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields எனில் FIELD_LIST is omitted\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  கட்டமைப்பு stat *stats = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  எழுத்து மாறிலி *செய்தி_mut_excl = _("options %s and %s are mutually exclusive");

  போது (true)
    {
      முழு oi = -1;
      முழு c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 'a':
          show_all_fs = true;
          நிறுத்து;
        நிலை 'B':
          {
            எண்ணிக்கை strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            எனில் (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          நிறுத்து;
        நிலை 'i':
          எனில் (header_mode == OUTPUT_MODE)
            {
              error (0, 0, செய்தி_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          நிறுத்து;
        நிலை 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          நிறுத்து;
        நிலை 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          நிறுத்து;
        நிலை 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          நிறுத்து;
        நிலை 'l':
          show_local_fs = true;
          நிறுத்து;
        நிலை 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          நிறுத்து;
        நிலை 'T':
          எனில் (header_mode == OUTPUT_MODE)
            {
              error (0, 0, செய்தி_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          நிறுத்து;
        நிலை 'P':
          எனில் (header_mode == OUTPUT_MODE)
            {
              error (0, 0, செய்தி_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          நிறுத்து;
        நிலை SYNC_OPTION:
          require_sync = true;
          நிறுத்து;
        நிலை NO_SYNC_OPTION:
          require_sync = false;
          நிறுத்து;

        நிலை 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        நிலை 't':
          add_fs_type (optarg);
          நிறுத்து;

        நிலை 'v':		/* For SysV compatibility.  */
          /* ignore */
          நிறுத்து;
        நிலை 'x':
          add_excluded_fs_type (optarg);
          நிறுத்து;

        நிலை OUTPUT_OPTION:
          எனில் (header_mode == INODES_MODE)
            {
              error (0, 0, செய்தி_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          எனில் (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, செய்தி_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          எனில் (print_type)
            {
              error (0, 0, செய்தி_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          எனில் (optarg)
            decode_output_arg (optarg);
          நிறுத்து;

        நிலை TOTAL_OPTION:
          print_grand_total = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (human_output_opts == -1)
    {
      எனில் (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      இல்லையெனில்
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  எனில் (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  இல்லையெனில் எனில் (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  இல்லையெனில் எனில் (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    க்கு (கட்டமைப்பு fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        க்கு (கட்டமைப்பு fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            எனில் (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                நிறுத்து;
              }
          }
      }
    எனில் (match)
      திரும்பு EXIT_FAILURE;
  }

  எனில் (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, அளவு *stats);
      க்கு (முழு i = optind; i < argc; ++i)
        {
          முழு err = automount_stat_err (argv[i], &stats[i - optind]);
          எனில் (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = வெற்று;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != வெற்று
                            || fs_exclude_list != வெற்று
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  எனில் (mount_list == வெற்று)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      முழு status = 0;
      எனில் ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != வெற்று
               || fs_exclude_list != வெற்று))
        {
          status = EXIT_FAILURE;
        }
      எழுத்து மாறிலி *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  எனில் (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  எனில் (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      க்கு (முழு i = optind; i < argc; ++i)
        எனில் (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  இல்லையெனில்
    get_all_entries ();

  எனில் (file_systems_processed)
    {
      எனில் (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 வெற்று, வெற்று, வெற்று, false, false, &grand_fsu, false);

      print_table ();
    }
  இல்லையெனில்
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      எனில் (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
