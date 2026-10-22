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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>
#include <c-ctype.h>

#include "system.h"
#include "assure.h"
#include "canonicalize.h"
#include "fsusage.h"
#include "human.h"
#include "mbswidth.h"
#include "mountlist.h"
#include "quote.h"
#include "find-mount-point.h"
#include "hash.h"
#include "xstrtol-error.h"
#include "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

sa_nracanaa devlist
{
  dev_t dev_num;
  sa_nracanaa mount_entry *me;
  sa_nracanaa devlist *next;
  sa_nracanaa devlist *next_same_dev;
  sa_nracanaa devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
static Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
static bool show_all_fs;

/* If true, show only local file systems.  */
static bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
static bool show_listed_fs;

/* Human-readable options for output.  */
static puur_naa_nka human_output_opts = -1;

/* The units to use when printing sizes.  */
static uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
static bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
static bool require_sync;

/* Desired exit status.  */
static puur_naa_nka exit_status;

/* A file system type to display.  */

sa_nracanaa fs_type_list
{
  var_na *fs_name;
  sa_nracanaa fs_type_list *fs_next;
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

static sa_nracanaa fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

static sa_nracanaa fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
static sa_nracanaa mount_entry *mount_list;

/* If true, print file system type as well.  */
static bool print_type;

/* If true, print a grand total at the end.  */
static bool print_grand_total;

/* Grand total data.  */
static sa_nracanaa fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
static ga_nanaa
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
typedef ga_nanaa
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
typedef ga_nanaa
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
sa_nracanaa field_data_t
{
  display_field_t field;
  var_na const *arg;
  field_type_t field_type;
  var_na const *caption;/* NULL means use default header of this field.  */
  puur_naa_nka width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_right;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
static sa_nracanaa field_data_t field_data[] = {
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

static var_na const *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

static sa_nracanaa
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  var_na ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  sa_nracanaa field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
sa_nracanaa field_values_t
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
ga_nanaa
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"all", no_argument, NULL, 'a'},
  {"block-size", required_argument, NULL, 'B'},
  {"inodes", no_argument, NULL, 'i'},
  {"human-readable", no_argument, NULL, 'h'},
  {"si", no_argument, NULL, 'H'},
  {"local", no_argument, NULL, 'l'},
  {"output", optional_argument, NULL, OUTPUT_OPTION},
  {"portability", no_argument, NULL, 'P'},
  {"print-type", no_argument, NULL, 'T'},
  {"sync", no_argument, NULL, SYNC_OPTION},
  {"no-sync", no_argument, NULL, NO_SYNC_OPTION},
  {"total", no_argument, NULL, TOTAL_OPTION},
  {"type", required_argument, NULL, 't'},
  {"exclude-type", required_argument, NULL, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

static puur_naa_nka
automount_stat_err (var_na const *file, sa_nracanaa stat *st)
{
  puur_naa_nka fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ᤀᤡ᤺ (fd < 0)
    {
      ᤀᤡ᤺ (errno == ENOENT || errno == ENOTDIR)
        ᤀᤡᤴ errno;
      ᤀᤡᤴ stat (file, st) == 0 ? 0 : errno;
    }
  ᤀᤡ᤺ᤆᤡ
    {
      puur_naa_nka err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      ᤀᤡᤴ err;
    }
}

ga_nanaa { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

static void
replace_control_chars (var_na *cell)
{
  var_na *p = cell;
  ᤀᤠ᤺ (*p)
    {
      ᤀᤡ᤺ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

static void
replace_invalid_chars (var_na *cell)
{
  var_na *srcend = cell + strlen (cell);
  var_na *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  for (var_na *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ᤀᤡ᤺ (ok)
        ok = !c32iscntrl (wc);
      ᤀᤡ᤺ᤆᤡ
        n = 1;

      ᤀᤡ᤺ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ᤀᤡ᤺ᤆᤡ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

static void
replace_problematic_chars (var_na *cell)
{
  static puur_naa_nka tty_out = -1;
  ᤀᤡ᤺ (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

static void
alloc_table_row (void)
{
  ᤀᤡ᤺ (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          parimaa_na *table.rows);
  idx_t const row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, parimaa_na *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

static void
print_table (void)
{
  for (idx_t row = 0; row < table.nrows; row++)
    {
      for (idx_t col = 0; col < table.ncolumns; col++)
        {
          var_na *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ᤀᤡ᤺ (col != 0)
            putchar (' ');

          puur_naa_nka width = mbswidth (cell, MBSWIDTH_FLAGS);
          puur_naa_nka fill = width < 0 ? 0 : table.columns[col]->width - width;
          ᤀᤡ᤺ (table.columns[col]->align_right)
            for (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          ᤀᤡ᤺ (col + 1 < table.ncolumns)
            for (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

static void
alloc_field (puur_naa_nka f, var_na const *c)
{
  ᤀᤡ᤺ (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             parimaa_na *table.columns);
  idx_t const column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  ᤀᤡ᤺ (c != NULL)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
static void
decode_output_arg (var_na const *arg)
{
  var_na *arg_writable = xstrdup (arg);
  var_na *s = arg_writable;
  do
    {
      /* find next comma */
      var_na *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ᤀᤡ᤺ (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      for (idx_t i = 0; i < countof (field_data); i++)
        {
          ᤀᤡ᤺ (streq (field_data[i].arg, s))
            {
              field = i;
              bha_nga;
            }
        }
      ᤀᤡ᤺ (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ᤀᤡ᤺ (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      switch (field)
        {
        pak_sa SOURCE_FIELD:
        pak_sa FSTYPE_FIELD:
        pak_sa USED_FIELD:
        pak_sa PCENT_FIELD:
        pak_sa ITOTAL_FIELD:
        pak_sa IUSED_FIELD:
        pak_sa IAVAIL_FIELD:
        pak_sa IPCENT_FIELD:
        pak_sa TARGET_FIELD:
        pak_sa FILE_FIELD:
          alloc_field (field, NULL);
          bha_nga;

        pak_sa SIZE_FIELD:
          alloc_field (field, N_("Size"));
          bha_nga;

        pak_sa AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          bha_nga;

        pak_sa INVALID_FIELD:
        default:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ᤀᤠ᤺ (s);

  free (arg_writable);
}

/* Get the appropriate columns for the mode.  */
static void
get_field_list (void)
{
  switch (header_mode)
    {
    pak_sa DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, NULL);
      ᤀᤡ᤺ (print_type)
        alloc_field (FSTYPE_FIELD, NULL);
      alloc_field (SIZE_FIELD,   NULL);
      alloc_field (USED_FIELD,   NULL);
      alloc_field (AVAIL_FIELD,  NULL);
      alloc_field (PCENT_FIELD,  NULL);
      alloc_field (TARGET_FIELD, NULL);
      bha_nga;

    pak_sa HUMAN_MODE:
      alloc_field (SOURCE_FIELD, NULL);
      ᤀᤡ᤺ (print_type)
        alloc_field (FSTYPE_FIELD, NULL);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   NULL);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  NULL);
      alloc_field (TARGET_FIELD, NULL);
      bha_nga;

    pak_sa INODES_MODE:
      alloc_field (SOURCE_FIELD, NULL);
      ᤀᤡ᤺ (print_type)
        alloc_field (FSTYPE_FIELD, NULL);
      alloc_field (ITOTAL_FIELD,  NULL);
      alloc_field (IUSED_FIELD,   NULL);
      alloc_field (IAVAIL_FIELD,  NULL);
      alloc_field (IPCENT_FIELD,  NULL);
      alloc_field (TARGET_FIELD,  NULL);
      bha_nga;

    pak_sa POSIX_MODE:
      alloc_field (SOURCE_FIELD, NULL);
      ᤀᤡ᤺ (print_type)
        alloc_field (FSTYPE_FIELD, NULL);
      alloc_field (SIZE_FIELD,   NULL);
      alloc_field (USED_FIELD,   NULL);
      alloc_field (AVAIL_FIELD,  NULL);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, NULL);
      bha_nga;

    pak_sa OUTPUT_MODE:
      ᤀᤡ᤺ (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      bha_nga;

    default:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

static void
get_header (void)
{
  alloc_table_row ();

  for (idx_t col = 0; col < table.ncolumns; col++)
    {
      var_na *cell;
      var_na const *header = _(table.columns[col]->caption);

      ᤀᤡ᤺ (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          var_na buf[LONGEST_HUMAN_READABLE + 1];

          puur_naa_nka opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          do
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ᤀᤠ᤺ (divisible_by_1000 & divisible_by_1024);

          ᤀᤡ᤺ (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          ᤀᤡ᤺ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          ᤀᤡ᤺ (! (opts & human_base_1024))
            opts |= human_B;

          var_na *num = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), num, header);
        }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      ᤀᤡ᤺ᤆᤡ
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      puur_naa_nka cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
static bool
selected_fstype (var_na const *fstype)
{
  ᤀᤡ᤺ (fs_select_list == NULL || fstype == NULL)
    ᤀᤡᤴ true;
  for (const sa_nracanaa fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    ᤀᤡ᤺ (streq (fstype, fsp->fs_name))
      ᤀᤡᤴ true;
  ᤀᤡᤴ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
static bool
excluded_fstype (var_na const *fstype)
{
  ᤀᤡ᤺ (fs_exclude_list == NULL || fstype == NULL)
    ᤀᤡᤴ false;
  for (const sa_nracanaa fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    ᤀᤡ᤺ (streq (fstype, fsp->fs_name))
      ᤀᤡᤴ true;
  ᤀᤡᤴ false;
}

static size_t
devlist_hash (void const *x, size_t table_size)
{
  sa_nracanaa devlist const *p = x;
  ᤀᤡᤴ (uintmax_t) {p->dev_num} % table_size;
}

static bool
devlist_compare (void const *x, void const *y)
{
  sa_nracanaa devlist const *a = x;
  sa_nracanaa devlist const *b = y;
  ᤀᤡᤴ a->dev_num == b->dev_num;
}

static sa_nracanaa devlist *
devlist_for_dev (dev_t dev)
{
  ᤀᤡ᤺ (devlist_table == NULL)
    ᤀᤡᤴ NULL;
  sa_nracanaa devlist dev_entry;
  dev_entry.dev_num = dev;

  sa_nracanaa devlist *found = hash_lookup (devlist_table, &dev_entry);
  ᤀᤡ᤺ (found == NULL)
    ᤀᤡᤴ NULL;

  /* Return the last devlist entry we have seen with this dev_num */
  ᤀᤡᤴ found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

static void
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  sa_nracanaa devlist *device_list = NULL;
  puur_naa_nka mount_list_size = 0;

  for (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, NULL,
                                   devlist_hash, devlist_compare, NULL);
  ᤀᤡ᤺ (devlist_table == NULL)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  for (sa_nracanaa mount_entry *me = mount_list; me;)
    {
      sa_nracanaa stat buf;
      sa_nracanaa mount_entry *discard_me = NULL;
      sa_nracanaa devlist *last_seen_dev = NULL, *seen_dev = NULL;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ᤀᤡ᤺ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ᤀᤡ᤺ᤆᤡ
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          for (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_root = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_root = seen_dev->me->me_mntroot != NULL
                                       && me->me_mntroot != NULL
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              ᤀᤡ᤺ (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ ((strchr (me->me_devname, '/')
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
              ᤀᤡ᤺ᤆᤡ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ᤀᤡ᤺ (discard_me)
        {
          me = me->me_next;
          ᤀᤡ᤺ (! devices_only)
            free_mount_entry (discard_me);
        }
      ᤀᤡ᤺ᤆᤡ
        {
          /* Add the device number to the device_table.  */
          sa_nracanaa devlist *devlist = xmalloc (parimaa_na *devlist);
          devlist->me = me;
          devlist->dev_num = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          sa_nracanaa devlist *hash_entry = hash_insert (devlist_table, devlist);
          ᤀᤡ᤺ (hash_entry == NULL)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ᤀᤡ᤺ (! devices_only)
    {
      mount_list = NULL;
      ᤀᤠ᤺ (device_list)
        {
          /* Add the mount entry.  */
          sa_nracanaa mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          sa_nracanaa devlist *next = device_list->next;
          free (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = NULL;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
static sa_nracanaa mount_entry const *
me_for_dev (dev_t dev)
{
  sa_nracanaa devlist *dl = devlist_for_dev (dev);
  ᤀᤡ᤺ (dl)
        ᤀᤡᤴ dl->me;

  ᤀᤡᤴ NULL;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
static bool
known_value (uintmax_t n)
{
  ᤀᤡᤴ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

static var_na const *
df_readable (bool negative, uintmax_t n, var_na *buf,
             uintmax_t input_units, uintmax_t output_units)
{
  ᤀᤡ᤺ (! known_value (n) && !negative)
    ᤀᤡᤴ "-";
  ᤀᤡ᤺ᤆᤡ
    {
      var_na *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, input_units, output_units);
      ᤀᤡ᤺ (negative)
        *--p = '-';
      ᤀᤡᤴ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
static void
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ᤀᤡ᤺ (*dest_neg == src_neg)
    {
      *dest += src;
      ᤀᤡᤴ;
    }

  ᤀᤡ᤺ (*dest_neg)
    *dest = -*dest;

  ᤀᤡ᤺ (src_neg)
    src = -src;

  ᤀᤡ᤺ (src < *dest)
    *dest -= src;
  ᤀᤡ᤺ᤆᤡ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ᤀᤡ᤺ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
static bool
has_uuid_suffix (var_na const *s)
{
  size_t len = strlen (s);
  ᤀᤡᤴ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
static void
get_field_values (sa_nracanaa field_values_t *bv,
                  sa_nracanaa field_values_t *iv,
                  const sa_nracanaa fs_usage *fsu)
{
  /* Inode values.  */
  iv->input_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_root = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  ᤀᤡ᤺ (known_value (iv->total) && known_value (iv->available_to_root))
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
  ᤀᤡ᤺ (known_value (bv->total) && known_value (bv->available_to_root))
    {
      bv->used = bv->total - bv->available_to_root;
      bv->negate_used = (bv->total < bv->available_to_root);
    }
}

/* Add block and inode values to grand total.  */
static void
add_to_grand_total (sa_nracanaa field_values_t *bv, sa_nracanaa field_values_t *iv)
{
  ᤀᤡ᤺ (known_value (iv->total))
    grand_fsu.fsu_files += iv->total;
  ᤀᤡ᤺ (known_value (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  ᤀᤡ᤺ (known_value (bv->total))
    grand_fsu.fsu_blocks += bv->input_units * bv->total;
  ᤀᤡ᤺ (known_value (bv->available_to_root))
    grand_fsu.fsu_bfree += bv->input_units * bv->available_to_root;
  ᤀᤡ᤺ (known_value (bv->available))
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

static void
get_dev (var_na const *device, var_na const *mount_point, var_na const *file,
         var_na const *stat_file, var_na const *fstype,
         bool me_dummy, bool me_remote,
         const sa_nracanaa fs_usage *force_fsu,
         bool process_all)
{
  ᤀᤡ᤺ (me_remote && show_local_fs)
    ᤀᤡᤴ;

  ᤀᤡ᤺ (me_dummy && !show_all_fs && !show_listed_fs)
    ᤀᤡᤴ;

  ᤀᤡ᤺ (!selected_fstype (fstype) || excluded_fstype (fstype))
    ᤀᤡᤴ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ᤀᤡ᤺ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    ᤀᤡᤴ;

  ᤀᤡ᤺ (!stat_file)
    stat_file = mount_point;

  sa_nracanaa fs_usage fsu;
  ᤀᤡ᤺ (force_fsu)
    fsu = *force_fsu;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ᤀᤡ᤺ (process_all && (errno == EACCES || errno == ENOENT))
        {
          ᤀᤡ᤺ (! show_all_fs)
            ᤀᤡᤴ;

          fstype = "-";
          fsu.fsu_bavail_top_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ᤀᤡ᤺ᤆᤡ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          ᤀᤡᤴ;
        }
    }
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      sa_nracanaa stat sb;
      ᤀᤡ᤺ (stat (stat_file, &sb) == 0)
        {
          sa_nracanaa mount_entry const * dev_me = me_for_dev (sb.st_dev);
          ᤀᤡ᤺ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_top_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ᤀᤡ᤺ (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    ᤀᤡᤴ;

  ᤀᤡ᤺ (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  ᤀᤡ᤺ (! device)
    device = "-";		/* unknown */

  ᤀᤡ᤺ (! file)
    file = "-";			/* unspecified */

  var_na *dev_name = xstrdup (device);
  var_na *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ᤀᤡ᤺ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      free (dev_name);
      dev_name = resolved_dev;
    }

  ᤀᤡ᤺ (! fstype)
    fstype = "-";		/* unknown */

  sa_nracanaa field_values_t block_values;
  sa_nracanaa field_values_t inode_values;
  get_field_values (&block_values, &inode_values, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ᤀᤡ᤺ (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_values, &inode_values);

  for (idx_t col = 0; col < table.ncolumns; col++)
    {
      var_na buf[LONGEST_HUMAN_READABLE + 2];
      var_na *cell;

      sa_nracanaa field_values_t *v;
      switch (table.columns[col]->field_type)
        {
        pak_sa BLOCK_FLD:
          v = &block_values;
          bha_nga;
        pak_sa INODE_FLD:
          v = &inode_values;
          bha_nga;
        pak_sa OTHER_FLD:
          v = NULL;
          bha_nga;
        default:
          affirm (!"bad field_type");
        }

      switch (table.columns[col]->field)
        {
        pak_sa SOURCE_FIELD:
          cell = xstrdup (dev_name);
          bha_nga;

        pak_sa FSTYPE_FIELD:
          cell = xstrdup (fstype);
          bha_nga;

        pak_sa SIZE_FIELD:
        pak_sa ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->input_units, v->output_units));
          bha_nga;

        pak_sa USED_FIELD:
        pak_sa IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->input_units, v->output_units));
          bha_nga;

        pak_sa AVAIL_FIELD:
        pak_sa IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->input_units, v->output_units));
          bha_nga;

        pak_sa PCENT_FIELD:
        pak_sa IPCENT_FIELD:
          {
            dviguu_na pct = -1;
            ᤀᤡ᤺ (! known_value (v->used) || ! known_value (v->available))
              ;
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            ᤀᤡ᤺ᤆᤡ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                dviguu_na u = v->negate_used ? - (dviguu_na) - v->used : v->used;
                dviguu_na a = v->negate_available
                           ? - (dviguu_na) - v->available : v->available;
                dviguu_na nonroot_total = u + a;
                ᤀᤡ᤺ (nonroot_total)
                  {
                    long puur_naa_nka lipct = pct = u * 100 / nonroot_total;
                    dviguu_na ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ᤀᤡ᤺ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            bha_nga;
          }

        pak_sa FILE_FIELD:
          cell = xstrdup (file);
          bha_nga;

        pak_sa TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ᤀᤡ᤺ (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          bha_nga;

        pak_sa INVALID_FIELD:
        default:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      puur_naa_nka cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  free (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
static var_na *
last_device_for_mount (var_na const *mount)
{
  sa_nracanaa mount_entry const *le = NULL;

  for (sa_nracanaa mount_entry const *me = mount_list; me; me = me->me_next)
    {
      ᤀᤡ᤺ (streq (me->me_mountdir, mount))
        le = me;
    }

  ᤀᤡ᤺ (le)
    {
      var_na *devname = le->me_devname;
      var_na *canon_dev = canonicalize_file_name (devname);
      ᤀᤡ᤺ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        ᤀᤡᤴ canon_dev;
      free (canon_dev);
      ᤀᤡᤴ xstrdup (le->me_devname);
    }
  ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ NULL;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
static bool
get_device (var_na const *device)
{
  sa_nracanaa mount_entry const *best_match = NULL;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  var_na const *file = device;

  var_na *resolved = canonicalize_file_name (device);
  ᤀᤡ᤺ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  for (sa_nracanaa mount_entry const *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      var_na *devname = me->me_devname;
      var_na *canon_dev = canonicalize_file_name (me->me_devname);
      ᤀᤡ᤺ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ᤀᤡ᤺ (streq (device, devname))
        {
          var_na *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          ᤀᤡ᤺ (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              sa_nracanaa stat device_stats;
              bool this_match_accessible = false;

              ᤀᤡ᤺ (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              ᤀᤡ᤺ (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  ᤀᤡ᤺ (len == 1) /* Traditional root.  */
                    {
                      free (last_device);
                      free (canon_dev);
                      bha_nga;
                    }
                  ᤀᤡ᤺ᤆᤡ
                    best_match_len = len;
                }
            }

          free (last_device);
        }

      free (canon_dev);
    }

  free (resolved);

  ᤀᤡ᤺ (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, NULL,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, NULL, false);
      ᤀᤡᤴ true;
    }
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      ᤀᤡᤴ true;
    }

  ᤀᤡᤴ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
static void
get_point (var_na const *point, const sa_nracanaa stat *statp)
{
  sa_nracanaa stat device_stats;
  sa_nracanaa mount_entry const *best_match = NULL;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  var_na *resolved = canonicalize_file_name (point);
  ᤀᤡ᤺ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      for (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
        {
          ᤀᤡ᤺ (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              ᤀᤡ᤺ (best_match_len <= len && len <= resolved_len
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
  free (resolved);
  ᤀᤡ᤺ (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = NULL;

  ᤀᤡ᤺ (! best_match)
    for (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
      {
        ᤀᤡ᤺ (me->me_dev == (dev_t) -1)
          {
            ᤀᤡ᤺ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ᤀᤡ᤺ᤆᤡ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ᤀᤡ᤺ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ᤀᤡ᤺ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ᤀᤡ᤺ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ᤀᤡ᤺ᤆᤡ
              best_match = me;
          }
      }

  ᤀᤡ᤺ (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             NULL, false);
  ᤀᤡ᤺ᤆᤡ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      var_na *mp = find_mount_point (point, statp);
      ᤀᤡ᤺ (mp)
        {
          get_dev (NULL, mp, point, NULL, NULL,
                   false, false, NULL, false);
          free (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

static void
get_entry (var_na const *name, sa_nracanaa stat const *statp)
{
  ᤀᤡ᤺ ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    ᤀᤡᤴ;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

static void
get_all_entries (void)
{
  filter_mount_list (show_all_fs);

  for (sa_nracanaa mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, NULL, NULL, me->me_type,
             me->me_dummy, me->me_remote, NULL, true);
}

/* Add FSTYPE to the list of file system types to display.  */

static void
add_fs_type (var_na const *fstype)
{
  sa_nracanaa fs_type_list *fsp;

  fsp = xmalloc (parimaa_na *fsp);
  fsp->fs_name = (var_na *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

static void
add_excluded_fs_type (var_na const *fstype)
{
  sa_nracanaa fs_type_list *fsp;

  fsp = xmalloc (parimaa_na *fsp);
  fsp->fs_name = (var_na *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

void
usage (puur_naa_nka status)
{
  ᤀᤡ᤺ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᤀᤡ᤺ᤆᤡ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by default.\n\
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
         do not invoke sync before getting usage info (default)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields ᤀᤡ᤺ FIELD_LIST is omitted\n\
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

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  sa_nracanaa stat *stats = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  var_na const *msg_mut_excl = _("options %s and %s are mutually exclusive");

  ᤀᤠ᤺ (true)
    {
      puur_naa_nka oi = -1;
      puur_naa_nka c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ᤀᤡ᤺ (c == -1)
        bha_nga;

      switch (c)
        {
        pak_sa 'a':
          show_all_fs = true;
          bha_nga;
        pak_sa 'B':
          {
            ga_nanaa strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ᤀᤡ᤺ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          bha_nga;
        pak_sa 'i':
          ᤀᤡ᤺ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          bha_nga;
        pak_sa 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          bha_nga;
        pak_sa 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          bha_nga;
        pak_sa 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          bha_nga;
        pak_sa 'l':
          show_local_fs = true;
          bha_nga;
        pak_sa 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          bha_nga;
        pak_sa 'T':
          ᤀᤡ᤺ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          bha_nga;
        pak_sa 'P':
          ᤀᤡ᤺ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, msg_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          bha_nga;
        pak_sa SYNC_OPTION:
          require_sync = true;
          bha_nga;
        pak_sa NO_SYNC_OPTION:
          require_sync = false;
          bha_nga;

        pak_sa 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        pak_sa 't':
          add_fs_type (optarg);
          bha_nga;

        pak_sa 'v':		/* For SysV compatibility.  */
          /* ignore */
          bha_nga;
        pak_sa 'x':
          add_excluded_fs_type (optarg);
          bha_nga;

        pak_sa OUTPUT_OPTION:
          ᤀᤡ᤺ (header_mode == INODES_MODE)
            {
              error (0, 0, msg_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ᤀᤡ᤺ (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, msg_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ᤀᤡ᤺ (print_type)
            {
              error (0, 0, msg_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          ᤀᤡ᤺ (optarg)
            decode_output_arg (optarg);
          bha_nga;

        pak_sa TOTAL_OPTION:
          print_grand_total = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ᤀᤡ᤺ (human_output_opts == -1)
    {
      ᤀᤡ᤺ (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ᤀᤡ᤺ᤆᤡ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  ᤀᤡ᤺ (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    for (sa_nracanaa fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        for (sa_nracanaa fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            ᤀᤡ᤺ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                bha_nga;
              }
          }
      }
    ᤀᤡ᤺ (match)
      ᤀᤡᤴ EXIT_FAILURE;
  }

  ᤀᤡ᤺ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, parimaa_na *stats);
      for (puur_naa_nka i = optind; i < argc; ++i)
        {
          puur_naa_nka err = automount_stat_err (argv[i], &stats[i - optind]);
          ᤀᤡ᤺ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = NULL;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != NULL
                            || fs_exclude_list != NULL
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  ᤀᤡ᤺ (mount_list == NULL)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      puur_naa_nka status = 0;
      ᤀᤡ᤺ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != NULL
               || fs_exclude_list != NULL))
        {
          status = EXIT_FAILURE;
        }
      var_na const *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ᤀᤡ᤺ (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  ᤀᤡ᤺ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      for (puur_naa_nka i = optind; i < argc; ++i)
        ᤀᤡ᤺ (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  ᤀᤡ᤺ᤆᤡ
    get_all_entries ();

  ᤀᤡ᤺ (file_systems_processed)
    {
      ᤀᤡ᤺ (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 NULL, NULL, NULL, false, false, &grand_fsu, false);

      print_table ();
    }
  ᤀᤡ᤺ᤆᤡ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ᤀᤡ᤺ (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
