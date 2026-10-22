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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <c-ctype.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "fsusage.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "mbswidth.h"
#ಅನ್ತರ್ಭಾವ "mountlist.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "find-mount-point.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"
#ಅನ್ತರ್ಭಾವ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ಸಂರಚನಾ devlist
{
  dev_t dev_ಅಂಕೆ;
  ಸಂರಚನಾ mount_entry *me;
  ಸಂರಚನಾ devlist *next;
  ಸಂರಚನಾ devlist *next_same_dev;
  ಸಂರಚನಾ devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ಸ್ಥಿರ Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ಸ್ಥಿರ bool show_all_fs;

/* If true, show only local file systems.  */
ಸ್ಥಿರ bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ಸ್ಥಿರ bool show_listed_fs;

/* Human-readable options for output.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ human_output_opts = -1;

/* The units to use when printing sizes.  */
ಸ್ಥಿರ uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
ಸ್ಥಿರ bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ಸ್ಥಿರ bool require_sync;

/* Desired exit status.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ exit_status;

/* A file system type to display.  */

ಸಂರಚನಾ fs_type_list
{
  ವರ್ಣ *fs_name;
  ಸಂರಚನಾ fs_type_list *fs_next;
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

ಸ್ಥಿರ ಸಂರಚನಾ fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ಸ್ಥಿರ ಸಂರಚನಾ fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ಸ್ಥಿರ ಸಂರಚನಾ mount_entry *mount_list;

/* If true, print file system type as well.  */
ಸ್ಥಿರ bool print_type;

/* If true, print a grand total at the end.  */
ಸ್ಥಿರ bool print_grand_total;

/* Grand total data.  */
ಸ್ಥಿರ ಸಂರಚನಾ fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ಸ್ಥಿರ ಗಣನಾ
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
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
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
ಸಂರಚನಾ field_data_t
{
  display_field_t field;
  ವರ್ಣ ಧ್ರುವ *arg;
  field_type_t field_type;
  ವರ್ಣ ಧ್ರುವ *caption;/* NULL means use default header of this field.  */
  ಪೂರ್ಣಾಙ್ಕ width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ಬಲ;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
ಸ್ಥಿರ ಸಂರಚನಾ field_data_t field_data[] = {
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

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ಸ್ಥಿರ ಸಂರಚನಾ
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  ವರ್ಣ ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ಸಂರಚನಾ field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t
{
  uintmax_t ಒಳಹರಿವು_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_ಬೇರು;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"all", no_argument, ರಿಕ್ತ, 'a'},
  {"block-size", required_argument, ರಿಕ್ತ, 'B'},
  {"inodes", no_argument, ರಿಕ್ತ, 'i'},
  {"human-readable", no_argument, ರಿಕ್ತ, 'h'},
  {"si", no_argument, ರಿಕ್ತ, 'H'},
  {"local", no_argument, ರಿಕ್ತ, 'l'},
  {"output", optional_argument, ರಿಕ್ತ, OUTPUT_OPTION},
  {"portability", no_argument, ರಿಕ್ತ, 'P'},
  {"print-type", no_argument, ರಿಕ್ತ, 'T'},
  {"sync", no_argument, ರಿಕ್ತ, SYNC_OPTION},
  {"no-sync", no_argument, ರಿಕ್ತ, NO_SYNC_OPTION},
  {"total", no_argument, ರಿಕ್ತ, TOTAL_OPTION},
  {"type", required_argument, ರಿಕ್ತ, 't'},
  {"exclude-type", required_argument, ರಿಕ್ತ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
automount_stat_err (ವರ್ಣ ಧ್ರುವ *file, ಸಂರಚನಾ stat *st)
{
  ಪೂರ್ಣಾಙ್ಕ fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ಯದಿ (fd < 0)
    {
      ಯದಿ (errno == ENOENT || errno == ENOTDIR)
        ಪ್ರತ್ಯಾಗಮ errno;
      ಪ್ರತ್ಯಾಗಮ stat (file, st) == 0 ? 0 : errno;
    }
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      ಪ್ರತ್ಯಾಗಮ err;
    }
}

ಗಣನಾ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ಸ್ಥಿರ ಶೂನ್ಯ
replace_control_chars (ವರ್ಣ *cell)
{
  ವರ್ಣ *p = cell;
  ಯಾವತ್ (*p)
    {
      ಯದಿ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
replace_invalid_chars (ವರ್ಣ *cell)
{
  ವರ್ಣ *srcend = cell + strlen (cell);
  ವರ್ಣ *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  ಪ್ರತಿ (ವರ್ಣ *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ಯದಿ (ok)
        ok = !c32iscntrl (wc);
      ಅನ್ಯಥಾ
        n = 1;

      ಯದಿ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ಅನ್ಯಥಾ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ಸ್ಥಿರ ಶೂನ್ಯ
replace_problematic_chars (ವರ್ಣ *cell)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tty_out = -1;
  ಯದಿ (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_table_row (ಶೂನ್ಯ)
{
  ಯದಿ (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          ಪರಿಮಾಣ *table.rows);
  idx_t ಧ್ರುವ row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, ಪರಿಮಾಣ *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_table (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t row = 0; row < table.nrows; row++)
    {
      ಪ್ರತಿ (idx_t col = 0; col < table.ncolumns; col++)
        {
          ವರ್ಣ *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ಯದಿ (col != 0)
            putchar (' ');

          ಪೂರ್ಣಾಙ್ಕ width = mbswidth (cell, MBSWIDTH_FLAGS);
          ಪೂರ್ಣಾಙ್ಕ fill = width < 0 ? 0 : table.columns[col]->width - width;
          ಯದಿ (table.columns[col]->align_ಬಲ)
            ಪ್ರತಿ (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          ಯದಿ (col + 1 < table.ncolumns)
            ಪ್ರತಿ (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_field (ಪೂರ್ಣಾಙ್ಕ f, ವರ್ಣ ಧ್ರುವ *c)
{
  ಯದಿ (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             ಪರಿಮಾಣ *table.columns);
  idx_t ಧ್ರುವ column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  ಯದಿ (c != ರಿಕ್ತ)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ಸ್ಥಿರ ಶೂನ್ಯ
decode_output_arg (ವರ್ಣ ಧ್ರುವ *arg)
{
  ವರ್ಣ *arg_writable = xstrdup (arg);
  ವರ್ಣ *s = arg_writable;
  ಕುರು
    {
      /* find next comma */
      ವರ್ಣ *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ಯದಿ (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      ಪ್ರತಿ (idx_t i = 0; i < countof (field_data); i++)
        {
          ಯದಿ (streq (field_data[i].arg, s))
            {
              field = i;
              ಭಙ್ಗ;
            }
        }
      ಯದಿ (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ಯದಿ (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      ವಿಕಲ್ಪ (field)
        {
        ಪಕ್ಷ SOURCE_FIELD:
        ಪಕ್ಷ FSTYPE_FIELD:
        ಪಕ್ಷ USED_FIELD:
        ಪಕ್ಷ PCENT_FIELD:
        ಪಕ್ಷ ITOTAL_FIELD:
        ಪಕ್ಷ IUSED_FIELD:
        ಪಕ್ಷ IAVAIL_FIELD:
        ಪಕ್ಷ IPCENT_FIELD:
        ಪಕ್ಷ TARGET_FIELD:
        ಪಕ್ಷ FILE_FIELD:
          alloc_field (field, ರಿಕ್ತ);
          ಭಙ್ಗ;

        ಪಕ್ಷ SIZE_FIELD:
          alloc_field (field, N_("Size"));
          ಭಙ್ಗ;

        ಪಕ್ಷ AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          ಭಙ್ಗ;

        ಪಕ್ಷ INVALID_FIELD:
        ಸ್ವಭಾವ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ಯಾವತ್ (s);

  ಮುಕ್ತ (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_field_list (ಶೂನ್ಯ)
{
  ವಿಕಲ್ಪ (header_mode)
    {
    ಪಕ್ಷ DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, ರಿಕ್ತ);
      ಯದಿ (print_type)
        alloc_field (FSTYPE_FIELD, ರಿಕ್ತ);
      alloc_field (SIZE_FIELD,   ರಿಕ್ತ);
      alloc_field (USED_FIELD,   ರಿಕ್ತ);
      alloc_field (AVAIL_FIELD,  ರಿಕ್ತ);
      alloc_field (PCENT_FIELD,  ರಿಕ್ತ);
      alloc_field (TARGET_FIELD, ರಿಕ್ತ);
      ಭಙ್ಗ;

    ಪಕ್ಷ HUMAN_MODE:
      alloc_field (SOURCE_FIELD, ರಿಕ್ತ);
      ಯದಿ (print_type)
        alloc_field (FSTYPE_FIELD, ರಿಕ್ತ);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   ರಿಕ್ತ);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  ರಿಕ್ತ);
      alloc_field (TARGET_FIELD, ರಿಕ್ತ);
      ಭಙ್ಗ;

    ಪಕ್ಷ INODES_MODE:
      alloc_field (SOURCE_FIELD, ರಿಕ್ತ);
      ಯದಿ (print_type)
        alloc_field (FSTYPE_FIELD, ರಿಕ್ತ);
      alloc_field (ITOTAL_FIELD,  ರಿಕ್ತ);
      alloc_field (IUSED_FIELD,   ರಿಕ್ತ);
      alloc_field (IAVAIL_FIELD,  ರಿಕ್ತ);
      alloc_field (IPCENT_FIELD,  ರಿಕ್ತ);
      alloc_field (TARGET_FIELD,  ರಿಕ್ತ);
      ಭಙ್ಗ;

    ಪಕ್ಷ POSIX_MODE:
      alloc_field (SOURCE_FIELD, ರಿಕ್ತ);
      ಯದಿ (print_type)
        alloc_field (FSTYPE_FIELD, ರಿಕ್ತ);
      alloc_field (SIZE_FIELD,   ರಿಕ್ತ);
      alloc_field (USED_FIELD,   ರಿಕ್ತ);
      alloc_field (AVAIL_FIELD,  ರಿಕ್ತ);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, ರಿಕ್ತ);
      ಭಙ್ಗ;

    ಪಕ್ಷ OUTPUT_MODE:
      ಯದಿ (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_header (ಶೂನ್ಯ)
{
  alloc_table_row ();

  ಪ್ರತಿ (idx_t col = 0; col < table.ncolumns; col++)
    {
      ವರ್ಣ *cell;
      ವರ್ಣ ಧ್ರುವ *header = _(table.columns[col]->caption);

      ಯದಿ (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          ವರ್ಣ buf[LONGEST_HUMAN_READABLE + 1];

          ಪೂರ್ಣಾಙ್ಕ opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          ಕುರು
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ಯಾವತ್ (divisible_by_1000 & divisible_by_1024);

          ಯದಿ (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          ಯದಿ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          ಯದಿ (! (opts & human_base_1024))
            opts |= human_B;

          ವರ್ಣ *ಅಂಕೆ = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), ಅಂಕೆ, header);
        }
      ಅನ್ಯಥಾ ಯದಿ (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      ಅನ್ಯಥಾ
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      ಪೂರ್ಣಾಙ್ಕ cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
selected_fstype (ವರ್ಣ ಧ್ರುವ *fstype)
{
  ಯದಿ (fs_select_list == ರಿಕ್ತ || fstype == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತಿ (ಧ್ರುವ ಸಂರಚನಾ fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    ಯದಿ (streq (fstype, fsp->fs_name))
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
ಸ್ಥಿರ bool
excluded_fstype (ವರ್ಣ ಧ್ರುವ *fstype)
{
  ಯದಿ (fs_exclude_list == ರಿಕ್ತ || fstype == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತಿ (ಧ್ರುವ ಸಂರಚನಾ fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    ಯದಿ (streq (fstype, fsp->fs_name))
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ size_t
devlist_hash (ಶೂನ್ಯ ಧ್ರುವ *x, size_t table_size)
{
  ಸಂರಚನಾ devlist ಧ್ರುವ *p = x;
  ಪ್ರತ್ಯಾಗಮ (uintmax_t) {p->dev_ಅಂಕೆ} % table_size;
}

ಸ್ಥಿರ bool
devlist_compare (ಶೂನ್ಯ ಧ್ರುವ *x, ಶೂನ್ಯ ಧ್ರುವ *y)
{
  ಸಂರಚನಾ devlist ಧ್ರುವ *a = x;
  ಸಂರಚನಾ devlist ಧ್ರುವ *b = y;
  ಪ್ರತ್ಯಾಗಮ a->dev_ಅಂಕೆ == b->dev_ಅಂಕೆ;
}

ಸ್ಥಿರ ಸಂರಚನಾ devlist *
devlist_for_dev (dev_t dev)
{
  ಯದಿ (devlist_table == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಸಂರಚನಾ devlist dev_entry;
  dev_entry.dev_ಅಂಕೆ = dev;

  ಸಂರಚನಾ devlist *found = hash_lookup (devlist_table, &dev_entry);
  ಯದಿ (found == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  /* Return the last devlist entry we have seen with this dev_num */
  ಪ್ರತ್ಯಾಗಮ found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ಸ್ಥಿರ ಶೂನ್ಯ
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ಸಂರಚನಾ devlist *device_list = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ mount_list_size = 0;

  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, ರಿಕ್ತ,
                                   devlist_hash, devlist_compare, ರಿಕ್ತ);
  ಯದಿ (devlist_table == ರಿಕ್ತ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me;)
    {
      ಸಂರಚನಾ stat buf;
      ಸಂರಚನಾ mount_entry *discard_me = ರಿಕ್ತ;
      ಸಂರಚನಾ devlist *last_seen_dev = ರಿಕ್ತ, *seen_dev = ರಿಕ್ತ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ಯದಿ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ಅನ್ಯಥಾ
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          ಪ್ರತಿ (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_ಬೇರು = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_ಬೇರು = seen_dev->me->me_mntroot != ರಿಕ್ತ
                                       && me->me_mntroot != ರಿಕ್ತ
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              ಯದಿ (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ಅನ್ಯಥಾ ಯದಿ ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_ಬೇರು && ! source_below_ಬೇರು)
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
              ಅನ್ಯಥಾ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ಯದಿ (discard_me)
        {
          me = me->me_next;
          ಯದಿ (! devices_only)
            free_mount_entry (discard_me);
        }
      ಅನ್ಯಥಾ
        {
          /* Add the device number to the device_table.  */
          ಸಂರಚನಾ devlist *devlist = xmalloc (ಪರಿಮಾಣ *devlist);
          devlist->me = me;
          devlist->dev_ಅಂಕೆ = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          ಸಂರಚನಾ devlist *hash_entry = hash_ಸೇರಿಸು (devlist_table, devlist);
          ಯದಿ (hash_entry == ರಿಕ್ತ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ಯದಿ (! devices_only)
    {
      mount_list = ರಿಕ್ತ;
      ಯಾವತ್ (device_list)
        {
          /* Add the mount entry.  */
          ಸಂರಚನಾ mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          ಸಂರಚನಾ devlist *next = device_list->next;
          ಮುಕ್ತ (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = ರಿಕ್ತ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಸಂರಚನಾ mount_entry ಧ್ರುವ *
me_for_dev (dev_t dev)
{
  ಸಂರಚನಾ devlist *dl = devlist_for_dev (dev);
  ಯದಿ (dl)
        ಪ್ರತ್ಯಾಗಮ dl->me;

  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ಸ್ಥಿರ bool
known_ಮೌಲ್ಯ (uintmax_t n)
{
  ಪ್ರತ್ಯಾಗಮ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
df_readable (bool negative, uintmax_t n, ವರ್ಣ *buf,
             uintmax_t ಒಳಹರಿವು_units, uintmax_t output_units)
{
  ಯದಿ (! known_ಮೌಲ್ಯ (n) && !negative)
    ಪ್ರತ್ಯಾಗಮ "-";
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ಒಳಹರಿವು_units, output_units);
      ಯದಿ (negative)
        *--p = '-';
      ಪ್ರತ್ಯಾಗಮ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ಸ್ಥಿರ ಶೂನ್ಯ
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ಯದಿ (*dest_neg == src_neg)
    {
      *dest += src;
      ಪ್ರತ್ಯಾಗಮ;
    }

  ಯದಿ (*dest_neg)
    *dest = -*dest;

  ಯದಿ (src_neg)
    src = -src;

  ಯದಿ (src < *dest)
    *dest -= src;
  ಅನ್ಯಥಾ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ಯದಿ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
ಸ್ಥಿರ bool
has_uuid_suffix (ವರ್ಣ ಧ್ರುವ *s)
{
  size_t len = strlen (s);
  ಪ್ರತ್ಯಾಗಮ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_field_ಮೌಲ್ಯಗಳು (ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t *bv,
                  ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t *iv,
                  ಧ್ರುವ ಸಂರಚನಾ fs_usage *fsu)
{
  /* Inode values.  */
  iv->ಒಳಹರಿವು_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_ಬೇರು = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  ಯದಿ (known_ಮೌಲ್ಯ (iv->total) && known_ಮೌಲ್ಯ (iv->available_to_ಬೇರು))
    {
      iv->used = iv->total - iv->available_to_ಬೇರು;
      iv->negate_used = (iv->total < iv->available_to_ಬೇರು);
    }

  /* Block values.  */
  bv->ಒಳಹರಿವು_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_ಬೇರು = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_ತುದಿ_bit_set
                         && known_ಮೌಲ್ಯ (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  ಯದಿ (known_ಮೌಲ್ಯ (bv->total) && known_ಮೌಲ್ಯ (bv->available_to_ಬೇರು))
    {
      bv->used = bv->total - bv->available_to_ಬೇರು;
      bv->negate_used = (bv->total < bv->available_to_ಬೇರು);
    }
}

/* Add block and inode values to grand total.  */
ಸ್ಥಿರ ಶೂನ್ಯ
add_to_grand_total (ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t *bv, ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t *iv)
{
  ಯದಿ (known_ಮೌಲ್ಯ (iv->total))
    grand_fsu.fsu_files += iv->total;
  ಯದಿ (known_ಮೌಲ್ಯ (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  ಯದಿ (known_ಮೌಲ್ಯ (bv->total))
    grand_fsu.fsu_blocks += bv->ಒಳಹರಿವು_units * bv->total;
  ಯದಿ (known_ಮೌಲ್ಯ (bv->available_to_ಬೇರು))
    grand_fsu.fsu_bfree += bv->ಒಳಹರಿವು_units * bv->available_to_ಬೇರು;
  ಯದಿ (known_ಮೌಲ್ಯ (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_ತುದಿ_bit_set,
                            bv->ಒಳಹರಿವು_units * bv->available,
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

ಸ್ಥಿರ ಶೂನ್ಯ
get_dev (ವರ್ಣ ಧ್ರುವ *device, ವರ್ಣ ಧ್ರುವ *mount_point, ವರ್ಣ ಧ್ರುವ *file,
         ವರ್ಣ ಧ್ರುವ *stat_file, ವರ್ಣ ಧ್ರುವ *fstype,
         bool me_dummy, bool me_remote,
         ಧ್ರುವ ಸಂರಚನಾ fs_usage *force_fsu,
         bool process_all)
{
  ಯದಿ (me_remote && show_local_fs)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (me_dummy && !show_all_fs && !show_listed_fs)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (!selected_fstype (fstype) || excluded_fstype (fstype))
    ಪ್ರತ್ಯಾಗಮ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ಯದಿ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (!stat_file)
    stat_file = mount_point;

  ಸಂರಚನಾ fs_usage fsu;
  ಯದಿ (force_fsu)
    fsu = *force_fsu;
  ಅನ್ಯಥಾ ಯದಿ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ಯದಿ (process_all && (errno == EACCES || errno == ENOENT))
        {
          ಯದಿ (! show_all_fs)
            ಪ್ರತ್ಯಾಗಮ;

          fstype = "-";
          fsu.fsu_bavail_ತುದಿ_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ಅನ್ಯಥಾ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          ಪ್ರತ್ಯಾಗಮ;
        }
    }
  ಅನ್ಯಥಾ ಯದಿ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ಸಂರಚನಾ stat sb;
      ಯದಿ (stat (stat_file, &sb) == 0)
        {
          ಸಂರಚನಾ mount_entry ಧ್ರುವ * dev_me = me_for_dev (sb.st_dev);
          ಯದಿ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_ತುದಿ_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ಯದಿ (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  ಯದಿ (! device)
    device = "-";		/* unknown */

  ಯದಿ (! file)
    file = "-";			/* unspecified */

  ವರ್ಣ *dev_name = xstrdup (device);
  ವರ್ಣ *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ಯದಿ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      ಮುಕ್ತ (dev_name);
      dev_name = resolved_dev;
    }

  ಯದಿ (! fstype)
    fstype = "-";		/* unknown */

  ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t block_ಮೌಲ್ಯಗಳು;
  ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t inode_ಮೌಲ್ಯಗಳು;
  get_field_ಮೌಲ್ಯಗಳು (&block_ಮೌಲ್ಯಗಳು, &inode_ಮೌಲ್ಯಗಳು, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ಯದಿ (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_ಮೌಲ್ಯಗಳು, &inode_ಮೌಲ್ಯಗಳು);

  ಪ್ರತಿ (idx_t col = 0; col < table.ncolumns; col++)
    {
      ವರ್ಣ buf[LONGEST_HUMAN_READABLE + 2];
      ವರ್ಣ *cell;

      ಸಂರಚನಾ field_ಮೌಲ್ಯಗಳು_t *v;
      ವಿಕಲ್ಪ (table.columns[col]->field_type)
        {
        ಪಕ್ಷ BLOCK_FLD:
          v = &block_ಮೌಲ್ಯಗಳು;
          ಭಙ್ಗ;
        ಪಕ್ಷ INODE_FLD:
          v = &inode_ಮೌಲ್ಯಗಳು;
          ಭಙ್ಗ;
        ಪಕ್ಷ OTHER_FLD:
          v = ರಿಕ್ತ;
          ಭಙ್ಗ;
        ಸ್ವಭಾವ:
          affirm (!"bad field_type");
        }

      ವಿಕಲ್ಪ (table.columns[col]->field)
        {
        ಪಕ್ಷ SOURCE_FIELD:
          cell = xstrdup (dev_name);
          ಭಙ್ಗ;

        ಪಕ್ಷ FSTYPE_FIELD:
          cell = xstrdup (fstype);
          ಭಙ್ಗ;

        ಪಕ್ಷ SIZE_FIELD:
        ಪಕ್ಷ ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ಭಙ್ಗ;

        ಪಕ್ಷ USED_FIELD:
        ಪಕ್ಷ IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ಭಙ್ಗ;

        ಪಕ್ಷ AVAIL_FIELD:
        ಪಕ್ಷ IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ಭಙ್ಗ;

        ಪಕ್ಷ PCENT_FIELD:
        ಪಕ್ಷ IPCENT_FIELD:
          {
            ದ್ವಿಗುಣ pct = -1;
            ಯದಿ (! known_ಮೌಲ್ಯ (v->used) || ! known_ಮೌಲ್ಯ (v->available))
              ;
            ಅನ್ಯಥಾ ಯದಿ (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            ಅನ್ಯಥಾ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ದ್ವಿಗುಣ u = v->negate_used ? - (ದ್ವಿಗುಣ) - v->used : v->used;
                ದ್ವಿಗುಣ a = v->negate_available
                           ? - (ದ್ವಿಗುಣ) - v->available : v->available;
                ದ್ವಿಗುಣ nonroot_total = u + a;
                ಯದಿ (nonroot_total)
                  {
                    long ಪೂರ್ಣಾಙ್ಕ lipct = pct = u * 100 / nonroot_total;
                    ದ್ವಿಗುಣ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ಯದಿ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ FILE_FIELD:
          cell = xstrdup (file);
          ಭಙ್ಗ;

        ಪಕ್ಷ TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ಯದಿ (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          ಅನ್ಯಥಾ ಯದಿ (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          ಭಙ್ಗ;

        ಪಕ್ಷ INVALID_FIELD:
        ಸ್ವಭಾವ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      ಪೂರ್ಣಾಙ್ಕ cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  ಮುಕ್ತ (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ಸ್ಥಿರ ವರ್ಣ *
last_device_for_mount (ವರ್ಣ ಧ್ರುವ *mount)
{
  ಸಂರಚನಾ mount_entry ಧ್ರುವ *le = ರಿಕ್ತ;

  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry ಧ್ರುವ *me = mount_list; me; me = me->me_next)
    {
      ಯದಿ (streq (me->me_mountdir, mount))
        le = me;
    }

  ಯದಿ (le)
    {
      ವರ್ಣ *devname = le->me_devname;
      ವರ್ಣ *canon_dev = canonicalize_file_name (devname);
      ಯದಿ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        ಪ್ರತ್ಯಾಗಮ canon_dev;
      ಮುಕ್ತ (canon_dev);
      ಪ್ರತ್ಯಾಗಮ xstrdup (le->me_devname);
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ಸ್ಥಿರ bool
get_device (ವರ್ಣ ಧ್ರುವ *device)
{
  ಸಂರಚನಾ mount_entry ಧ್ರುವ *best_match = ರಿಕ್ತ;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  ವರ್ಣ ಧ್ರುವ *file = device;

  ವರ್ಣ *resolved = canonicalize_file_name (device);
  ಯದಿ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry ಧ್ರುವ *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      ವರ್ಣ *devname = me->me_devname;
      ವರ್ಣ *canon_dev = canonicalize_file_name (me->me_devname);
      ಯದಿ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ಯದಿ (streq (device, devname))
        {
          ವರ್ಣ *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          ಯದಿ (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              ಸಂರಚನಾ stat device_stats;
              bool this_match_accessible = false;

              ಯದಿ (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              ಯದಿ (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  ಯದಿ (len == 1) /* Traditional root.  */
                    {
                      ಮುಕ್ತ (last_device);
                      ಮುಕ್ತ (canon_dev);
                      ಭಙ್ಗ;
                    }
                  ಅನ್ಯಥಾ
                    best_match_len = len;
                }
            }

          ಮುಕ್ತ (last_device);
        }

      ಮುಕ್ತ (canon_dev);
    }

  ಮುಕ್ತ (resolved);

  ಯದಿ (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, ರಿಕ್ತ,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, ರಿಕ್ತ, false);
      ಪ್ರತ್ಯಾಗಮ true;
    }
  ಅನ್ಯಥಾ ಯದಿ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      ಪ್ರತ್ಯಾಗಮ true;
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_point (ವರ್ಣ ಧ್ರುವ *point, ಧ್ರುವ ಸಂರಚನಾ stat *statp)
{
  ಸಂರಚನಾ stat device_stats;
  ಸಂರಚನಾ mount_entry ಧ್ರುವ *best_match = ರಿಕ್ತ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  ವರ್ಣ *resolved = canonicalize_file_name (point);
  ಯದಿ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me; me = me->me_next)
        {
          ಯದಿ (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              ಯದಿ (best_match_len <= len && len <= resolved_len
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
  ಮುಕ್ತ (resolved);
  ಯದಿ (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = ರಿಕ್ತ;

  ಯದಿ (! best_match)
    ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me; me = me->me_next)
      {
        ಯದಿ (me->me_dev == (dev_t) -1)
          {
            ಯದಿ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ಅನ್ಯಥಾ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ಯದಿ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ಯದಿ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ಯದಿ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ಅನ್ಯಥಾ
              best_match = me;
          }
      }

  ಯದಿ (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             ರಿಕ್ತ, false);
  ಅನ್ಯಥಾ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      ವರ್ಣ *mp = find_mount_point (point, statp);
      ಯದಿ (mp)
        {
          get_dev (ರಿಕ್ತ, mp, point, ರಿಕ್ತ, ರಿಕ್ತ,
                   false, false, ರಿಕ್ತ, false);
          ಮುಕ್ತ (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_entry (ವರ್ಣ ಧ್ರುವ *name, ಸಂರಚನಾ stat ಧ್ರುವ *statp)
{
  ಯದಿ ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    ಪ್ರತ್ಯಾಗಮ;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_all_entries (ಶೂನ್ಯ)
{
  filter_mount_list (show_all_fs);

  ಪ್ರತಿ (ಸಂರಚನಾ mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, ರಿಕ್ತ, ರಿಕ್ತ, me->me_type,
             me->me_dummy, me->me_remote, ರಿಕ್ತ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ಸ್ಥಿರ ಶೂನ್ಯ
add_fs_type (ವರ್ಣ ಧ್ರುವ *fstype)
{
  ಸಂರಚನಾ fs_type_list *fsp;

  fsp = xmalloc (ಪರಿಮಾಣ *fsp);
  fsp->fs_name = (ವರ್ಣ *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ಸ್ಥಿರ ಶೂನ್ಯ
add_excluded_fs_type (ವರ್ಣ ಧ್ರುವ *fstype)
{
  ಸಂರಚನಾ fs_type_list *fsp;

  fsp = xmalloc (ಪರಿಮಾಣ *fsp);
  fsp->fs_name = (ವರ್ಣ *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by ಸ್ವಭಾವ.\n\
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
         ಕುರು not invoke sync before getting usage info (ಸ್ವಭಾವ)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields ಯದಿ FIELD_LIST is omitted\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಸಂರಚನಾ stat *stats = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  ವರ್ಣ ಧ್ರುವ *ಸಂದೇಶ_mut_excl = _("options %s and %s are mutually exclusive");

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ oi = -1;
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ಯದಿ (c == -1)
        ಭಙ್ಗ;

      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'a':
          show_all_fs = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'B':
          {
            ಗಣನಾ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ಯದಿ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          ಯದಿ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'l':
          show_local_fs = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'T':
          ಯದಿ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'P':
          ಯದಿ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ SYNC_OPTION:
          require_sync = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ NO_SYNC_OPTION:
          require_sync = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        ಪಕ್ಷ 't':
          add_fs_type (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'v':		/* For SysV compatibility.  */
          /* ignore */
          ಭಙ್ಗ;
        ಪಕ್ಷ 'x':
          add_excluded_fs_type (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ OUTPUT_OPTION:
          ಯದಿ (header_mode == INODES_MODE)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ಯದಿ (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ಯದಿ (print_type)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          ಯದಿ (optarg)
            decode_output_arg (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ TOTAL_OPTION:
          print_grand_total = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (human_output_opts == -1)
    {
      ಯದಿ (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ಅನ್ಯಥಾ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  ಯದಿ (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  ಅನ್ಯಥಾ ಯದಿ (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  ಅನ್ಯಥಾ ಯದಿ (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    ಪ್ರತಿ (ಸಂರಚನಾ fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        ಪ್ರತಿ (ಸಂರಚನಾ fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            ಯದಿ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                ಭಙ್ಗ;
              }
          }
      }
    ಯದಿ (match)
      ಪ್ರತ್ಯಾಗಮ EXIT_FAILURE;
  }

  ಯದಿ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, ಪರಿಮಾಣ *stats);
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; ++i)
        {
          ಪೂರ್ಣಾಙ್ಕ err = automount_stat_err (argv[i], &stats[i - optind]);
          ಯದಿ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = ರಿಕ್ತ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ರಿಕ್ತ
                            || fs_exclude_list != ರಿಕ್ತ
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  ಯದಿ (mount_list == ರಿಕ್ತ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      ಪೂರ್ಣಾಙ್ಕ status = 0;
      ಯದಿ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ರಿಕ್ತ
               || fs_exclude_list != ರಿಕ್ತ))
        {
          status = EXIT_FAILURE;
        }
      ವರ್ಣ ಧ್ರುವ *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ಯದಿ (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  ಯದಿ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; ++i)
        ಯದಿ (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  ಅನ್ಯಥಾ
    get_all_entries ();

  ಯದಿ (file_systems_processed)
    {
      ಯದಿ (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, false, false, &grand_fsu, false);

      print_table ();
    }
  ಅನ್ಯಥಾ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ಯದಿ (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
