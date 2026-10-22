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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <c-ctype.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "canonicalize.h"
#અન્તર્ભાવ "fsusage.h"
#અન્તર્ભાવ "human.h"
#અન્તર્ભાવ "mbswidth.h"
#અન્તર્ભાવ "mountlist.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "find-mount-point.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "xstrtol-error.h"
#અન્તર્ભાવ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

સંરચના devlist
{
  dev_t dev_અંક;
  સંરચના mount_entry *me;
  સંરચના devlist *next;
  સંરચના devlist *next_same_dev;
  સંરચના devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
સ્થિર Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
સ્થિર bool show_all_fs;

/* If true, show only local file systems.  */
સ્થિર bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
સ્થિર bool show_listed_fs;

/* Human-readable options for output.  */
સ્થિર પૂર્ણાંક human_output_opts = -1;

/* The units to use when printing sizes.  */
સ્થિર uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
સ્થિર bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
સ્થિર bool require_sync;

/* Desired exit status.  */
સ્થિર પૂર્ણાંક exit_status;

/* A file system type to display.  */

સંરચના fs_type_list
{
  અક્ષર *fs_name;
  સંરચના fs_type_list *fs_next;
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

સ્થિર સંરચના fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

સ્થિર સંરચના fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
સ્થિર સંરચના mount_entry *mount_list;

/* If true, print file system type as well.  */
સ્થિર bool print_type;

/* If true, print a grand total at the end.  */
સ્થિર bool print_grand_total;

/* Grand total data.  */
સ્થિર સંરચના fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
સ્થિર ગણના
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
પ્રકારવ્યાખ્યા ગણના
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
પ્રકારવ્યાખ્યા ગણના
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
સંરચના field_data_t
{
  display_field_t field;
  અક્ષર અચલ *arg;
  field_type_t field_type;
  અક્ષર અચલ *caption;/* NULL means use default header of this field.  */
  પૂર્ણાંક width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_જમણું;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
સ્થિર સંરચના field_data_t field_data[] = {
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

સ્થિર અક્ષર અચલ *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

સ્થિર સંરચના
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  અક્ષર ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  સંરચના field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
સંરચના field_કિંમતો_t
{
  uintmax_t આગત_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_મૂળ;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"block-size", required_argument, ખાલી, 'B'},
  {"inodes", no_argument, ખાલી, 'i'},
  {"human-readable", no_argument, ખાલી, 'h'},
  {"si", no_argument, ખાલી, 'H'},
  {"local", no_argument, ખાલી, 'l'},
  {"output", optional_argument, ખાલી, OUTPUT_OPTION},
  {"portability", no_argument, ખાલી, 'P'},
  {"print-type", no_argument, ખાલી, 'T'},
  {"sync", no_argument, ખાલી, SYNC_OPTION},
  {"no-sync", no_argument, ખાલી, NO_SYNC_OPTION},
  {"total", no_argument, ખાલી, TOTAL_OPTION},
  {"type", required_argument, ખાલી, 't'},
  {"exclude-type", required_argument, ખાલી, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

સ્થિર પૂર્ણાંક
automount_stat_err (અક્ષર અચલ *file, સંરચના stat *st)
{
  પૂર્ણાંક fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  જો (fd < 0)
    {
      જો (errno == ENOENT || errno == ENOTDIR)
        પાછા errno;
      પાછા stat (file, st) == 0 ? 0 : errno;
    }
  નહીંતર
    {
      પૂર્ણાંક err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      પાછા err;
    }
}

ગણના { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

સ્થિર શૂન્ય
replace_control_chars (અક્ષર *cell)
{
  અક્ષર *p = cell;
  જ્યારે (*p)
    {
      જો (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

સ્થિર શૂન્ય
replace_invalid_chars (અક્ષર *cell)
{
  અક્ષર *srcend = cell + strlen (cell);
  અક્ષર *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  માટે (અક્ષર *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      જો (ok)
        ok = !c32iscntrl (wc);
      નહીંતર
        n = 1;

      જો (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      નહીંતર
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

સ્થિર શૂન્ય
replace_problematic_chars (અક્ષર *cell)
{
  સ્થિર પૂર્ણાંક tty_out = -1;
  જો (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

સ્થિર શૂન્ય
alloc_table_row (શૂન્ય)
{
  જો (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          કદ *table.rows);
  idx_t અચલ row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, કદ *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

સ્થિર શૂન્ય
print_table (શૂન્ય)
{
  માટે (idx_t row = 0; row < table.nrows; row++)
    {
      માટે (idx_t col = 0; col < table.ncolumns; col++)
        {
          અક્ષર *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          જો (col != 0)
            putchar (' ');

          પૂર્ણાંક width = mbswidth (cell, MBSWIDTH_FLAGS);
          પૂર્ણાંક fill = width < 0 ? 0 : table.columns[col]->width - width;
          જો (table.columns[col]->align_જમણું)
            માટે (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          જો (col + 1 < table.ncolumns)
            માટે (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

સ્થિર શૂન્ય
alloc_field (પૂર્ણાંક f, અક્ષર અચલ *c)
{
  જો (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             કદ *table.columns);
  idx_t અચલ column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  જો (c != ખાલી)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
સ્થિર શૂન્ય
decode_output_arg (અક્ષર અચલ *arg)
{
  અક્ષર *arg_writable = xstrdup (arg);
  અક્ષર *s = arg_writable;
  કરો
    {
      /* find next comma */
      અક્ષર *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      જો (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      માટે (idx_t i = 0; i < countof (field_data); i++)
        {
          જો (streq (field_data[i].arg, s))
            {
              field = i;
              તોડો;
            }
        }
      જો (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      જો (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      પસંદગી (field)
        {
        સ્થિતિ SOURCE_FIELD:
        સ્થિતિ FSTYPE_FIELD:
        સ્થિતિ USED_FIELD:
        સ્થિતિ PCENT_FIELD:
        સ્થિતિ ITOTAL_FIELD:
        સ્થિતિ IUSED_FIELD:
        સ્થિતિ IAVAIL_FIELD:
        સ્થિતિ IPCENT_FIELD:
        સ્થિતિ TARGET_FIELD:
        સ્થિતિ FILE_FIELD:
          alloc_field (field, ખાલી);
          તોડો;

        સ્થિતિ SIZE_FIELD:
          alloc_field (field, N_("Size"));
          તોડો;

        સ્થિતિ AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          તોડો;

        સ્થિતિ INVALID_FIELD:
        મૂળભૂત:
          affirm (!"invalid field");
        }
      s = comma;
    }
  જ્યારે (s);

  મુક્ત (arg_writable);
}

/* Get the appropriate columns for the mode.  */
સ્થિર શૂન્ય
get_field_list (શૂન્ય)
{
  પસંદગી (header_mode)
    {
    સ્થિતિ DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, ખાલી);
      જો (print_type)
        alloc_field (FSTYPE_FIELD, ખાલી);
      alloc_field (SIZE_FIELD,   ખાલી);
      alloc_field (USED_FIELD,   ખાલી);
      alloc_field (AVAIL_FIELD,  ખાલી);
      alloc_field (PCENT_FIELD,  ખાલી);
      alloc_field (TARGET_FIELD, ખાલી);
      તોડો;

    સ્થિતિ HUMAN_MODE:
      alloc_field (SOURCE_FIELD, ખાલી);
      જો (print_type)
        alloc_field (FSTYPE_FIELD, ખાલી);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   ખાલી);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  ખાલી);
      alloc_field (TARGET_FIELD, ખાલી);
      તોડો;

    સ્થિતિ INODES_MODE:
      alloc_field (SOURCE_FIELD, ખાલી);
      જો (print_type)
        alloc_field (FSTYPE_FIELD, ખાલી);
      alloc_field (ITOTAL_FIELD,  ખાલી);
      alloc_field (IUSED_FIELD,   ખાલી);
      alloc_field (IAVAIL_FIELD,  ખાલી);
      alloc_field (IPCENT_FIELD,  ખાલી);
      alloc_field (TARGET_FIELD,  ખાલી);
      તોડો;

    સ્થિતિ POSIX_MODE:
      alloc_field (SOURCE_FIELD, ખાલી);
      જો (print_type)
        alloc_field (FSTYPE_FIELD, ખાલી);
      alloc_field (SIZE_FIELD,   ખાલી);
      alloc_field (USED_FIELD,   ખાલી);
      alloc_field (AVAIL_FIELD,  ખાલી);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, ખાલી);
      તોડો;

    સ્થિતિ OUTPUT_MODE:
      જો (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      તોડો;

    મૂળભૂત:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

સ્થિર શૂન્ય
get_header (શૂન્ય)
{
  alloc_table_row ();

  માટે (idx_t col = 0; col < table.ncolumns; col++)
    {
      અક્ષર *cell;
      અક્ષર અચલ *header = _(table.columns[col]->caption);

      જો (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          અક્ષર buf[LONGEST_HUMAN_READABLE + 1];

          પૂર્ણાંક opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          કરો
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          જ્યારે (divisible_by_1000 & divisible_by_1024);

          જો (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          જો (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          જો (! (opts & human_base_1024))
            opts |= human_B;

          અક્ષર *અંક = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), અંક, header);
        }
      નહીંતર જો (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      નહીંતર
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      પૂર્ણાંક cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
સ્થિર bool
selected_fstype (અક્ષર અચલ *fstype)
{
  જો (fs_select_list == ખાલી || fstype == ખાલી)
    પાછા true;
  માટે (અચલ સંરચના fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    જો (streq (fstype, fsp->fs_name))
      પાછા true;
  પાછા false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
સ્થિર bool
excluded_fstype (અક્ષર અચલ *fstype)
{
  જો (fs_exclude_list == ખાલી || fstype == ખાલી)
    પાછા false;
  માટે (અચલ સંરચના fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    જો (streq (fstype, fsp->fs_name))
      પાછા true;
  પાછા false;
}

સ્થિર size_t
devlist_hash (શૂન્ય અચલ *x, size_t table_size)
{
  સંરચના devlist અચલ *p = x;
  પાછા (uintmax_t) {p->dev_અંક} % table_size;
}

સ્થિર bool
devlist_compare (શૂન્ય અચલ *x, શૂન્ય અચલ *y)
{
  સંરચના devlist અચલ *a = x;
  સંરચના devlist અચલ *b = y;
  પાછા a->dev_અંક == b->dev_અંક;
}

સ્થિર સંરચના devlist *
devlist_for_dev (dev_t dev)
{
  જો (devlist_table == ખાલી)
    પાછા ખાલી;
  સંરચના devlist dev_entry;
  dev_entry.dev_અંક = dev;

  સંરચના devlist *found = hash_lookup (devlist_table, &dev_entry);
  જો (found == ખાલી)
    પાછા ખાલી;

  /* Return the last devlist entry we have seen with this dev_num */
  પાછા found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

સ્થિર શૂન્ય
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  સંરચના devlist *device_list = ખાલી;
  પૂર્ણાંક mount_list_size = 0;

  માટે (સંરચના mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, ખાલી,
                                   devlist_hash, devlist_compare, ખાલી);
  જો (devlist_table == ખાલી)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  માટે (સંરચના mount_entry *me = mount_list; me;)
    {
      સંરચના stat buf;
      સંરચના mount_entry *discard_me = ખાલી;
      સંરચના devlist *last_seen_dev = ખાલી, *seen_dev = ખાલી;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      જો ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      નહીંતર
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          માટે (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_મૂળ = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_મૂળ = seen_dev->me->me_mntroot != ખાલી
                                       && me->me_mntroot != ખાલી
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              જો (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              નહીંતર જો ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_મૂળ && ! source_below_મૂળ)
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
              નહીંતર
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      જો (discard_me)
        {
          me = me->me_next;
          જો (! devices_only)
            free_mount_entry (discard_me);
        }
      નહીંતર
        {
          /* Add the device number to the device_table.  */
          સંરચના devlist *devlist = xmalloc (કદ *devlist);
          devlist->me = me;
          devlist->dev_અંક = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          સંરચના devlist *hash_entry = hash_ઉમેરો (devlist_table, devlist);
          જો (hash_entry == ખાલી)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  જો (! devices_only)
    {
      mount_list = ખાલી;
      જ્યારે (device_list)
        {
          /* Add the mount entry.  */
          સંરચના mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          સંરચના devlist *next = device_list->next;
          મુક્ત (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = ખાલી;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
સ્થિર સંરચના mount_entry અચલ *
me_for_dev (dev_t dev)
{
  સંરચના devlist *dl = devlist_for_dev (dev);
  જો (dl)
        પાછા dl->me;

  પાછા ખાલી;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
સ્થિર bool
known_કિંમત (uintmax_t n)
{
  પાછા n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

સ્થિર અક્ષર અચલ *
df_readable (bool negative, uintmax_t n, અક્ષર *buf,
             uintmax_t આગત_units, uintmax_t output_units)
{
  જો (! known_કિંમત (n) && !negative)
    પાછા "-";
  નહીંતર
    {
      અક્ષર *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, આગત_units, output_units);
      જો (negative)
        *--p = '-';
      પાછા p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
સ્થિર શૂન્ય
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  જો (*dest_neg == src_neg)
    {
      *dest += src;
      પાછા;
    }

  જો (*dest_neg)
    *dest = -*dest;

  જો (src_neg)
    src = -src;

  જો (src < *dest)
    *dest -= src;
  નહીંતર
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  જો (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
સ્થિર bool
has_uuid_suffix (અક્ષર અચલ *s)
{
  size_t len = strlen (s);
  પાછા (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
સ્થિર શૂન્ય
get_field_કિંમતો (સંરચના field_કિંમતો_t *bv,
                  સંરચના field_કિંમતો_t *iv,
                  અચલ સંરચના fs_usage *fsu)
{
  /* Inode values.  */
  iv->આગત_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_મૂળ = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  જો (known_કિંમત (iv->total) && known_કિંમત (iv->available_to_મૂળ))
    {
      iv->used = iv->total - iv->available_to_મૂળ;
      iv->negate_used = (iv->total < iv->available_to_મૂળ);
    }

  /* Block values.  */
  bv->આગત_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_મૂળ = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_ટોચ_bit_set
                         && known_કિંમત (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  જો (known_કિંમત (bv->total) && known_કિંમત (bv->available_to_મૂળ))
    {
      bv->used = bv->total - bv->available_to_મૂળ;
      bv->negate_used = (bv->total < bv->available_to_મૂળ);
    }
}

/* Add block and inode values to grand total.  */
સ્થિર શૂન્ય
add_to_grand_total (સંરચના field_કિંમતો_t *bv, સંરચના field_કિંમતો_t *iv)
{
  જો (known_કિંમત (iv->total))
    grand_fsu.fsu_files += iv->total;
  જો (known_કિંમત (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  જો (known_કિંમત (bv->total))
    grand_fsu.fsu_blocks += bv->આગત_units * bv->total;
  જો (known_કિંમત (bv->available_to_મૂળ))
    grand_fsu.fsu_bfree += bv->આગત_units * bv->available_to_મૂળ;
  જો (known_કિંમત (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_ટોચ_bit_set,
                            bv->આગત_units * bv->available,
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

સ્થિર શૂન્ય
get_dev (અક્ષર અચલ *device, અક્ષર અચલ *mount_point, અક્ષર અચલ *file,
         અક્ષર અચલ *stat_file, અક્ષર અચલ *fstype,
         bool me_dummy, bool me_remote,
         અચલ સંરચના fs_usage *force_fsu,
         bool process_all)
{
  જો (me_remote && show_local_fs)
    પાછા;

  જો (me_dummy && !show_all_fs && !show_listed_fs)
    પાછા;

  જો (!selected_fstype (fstype) || excluded_fstype (fstype))
    પાછા;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  જો (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    પાછા;

  જો (!stat_file)
    stat_file = mount_point;

  સંરચના fs_usage fsu;
  જો (force_fsu)
    fsu = *force_fsu;
  નહીંતર જો (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      જો (process_all && (errno == EACCES || errno == ENOENT))
        {
          જો (! show_all_fs)
            પાછા;

          fstype = "-";
          fsu.fsu_bavail_ટોચ_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      નહીંતર
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          પાછા;
        }
    }
  નહીંતર જો (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      સંરચના stat sb;
      જો (stat (stat_file, &sb) == 0)
        {
          સંરચના mount_entry અચલ * dev_me = me_for_dev (sb.st_dev);
          જો (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_ટોચ_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  જો (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    પાછા;

  જો (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  જો (! device)
    device = "-";		/* unknown */

  જો (! file)
    file = "-";			/* unspecified */

  અક્ષર *dev_name = xstrdup (device);
  અક્ષર *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  જો (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      મુક્ત (dev_name);
      dev_name = resolved_dev;
    }

  જો (! fstype)
    fstype = "-";		/* unknown */

  સંરચના field_કિંમતો_t block_કિંમતો;
  સંરચના field_કિંમતો_t inode_કિંમતો;
  get_field_કિંમતો (&block_કિંમતો, &inode_કિંમતો, &fsu);

  /* Add to grand total unless processing grand total line.  */
  જો (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_કિંમતો, &inode_કિંમતો);

  માટે (idx_t col = 0; col < table.ncolumns; col++)
    {
      અક્ષર buf[LONGEST_HUMAN_READABLE + 2];
      અક્ષર *cell;

      સંરચના field_કિંમતો_t *v;
      પસંદગી (table.columns[col]->field_type)
        {
        સ્થિતિ BLOCK_FLD:
          v = &block_કિંમતો;
          તોડો;
        સ્થિતિ INODE_FLD:
          v = &inode_કિંમતો;
          તોડો;
        સ્થિતિ OTHER_FLD:
          v = ખાલી;
          તોડો;
        મૂળભૂત:
          affirm (!"bad field_type");
        }

      પસંદગી (table.columns[col]->field)
        {
        સ્થિતિ SOURCE_FIELD:
          cell = xstrdup (dev_name);
          તોડો;

        સ્થિતિ FSTYPE_FIELD:
          cell = xstrdup (fstype);
          તોડો;

        સ્થિતિ SIZE_FIELD:
        સ્થિતિ ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->આગત_units, v->output_units));
          તોડો;

        સ્થિતિ USED_FIELD:
        સ્થિતિ IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->આગત_units, v->output_units));
          તોડો;

        સ્થિતિ AVAIL_FIELD:
        સ્થિતિ IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->આગત_units, v->output_units));
          તોડો;

        સ્થિતિ PCENT_FIELD:
        સ્થિતિ IPCENT_FIELD:
          {
            દ્વિગુણ pct = -1;
            જો (! known_કિંમત (v->used) || ! known_કિંમત (v->available))
              ;
            નહીંતર જો (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            નહીંતર
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                દ્વિગુણ u = v->negate_used ? - (દ્વિગુણ) - v->used : v->used;
                દ્વિગુણ a = v->negate_available
                           ? - (દ્વિગુણ) - v->available : v->available;
                દ્વિગુણ nonroot_total = u + a;
                જો (nonroot_total)
                  {
                    long પૂર્ણાંક lipct = pct = u * 100 / nonroot_total;
                    દ્વિગુણ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    જો (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            તોડો;
          }

        સ્થિતિ FILE_FIELD:
          cell = xstrdup (file);
          તોડો;

        સ્થિતિ TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          જો (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          નહીંતર જો (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          તોડો;

        સ્થિતિ INVALID_FIELD:
        મૂળભૂત:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      પૂર્ણાંક cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  મુક્ત (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
સ્થિર અક્ષર *
last_device_for_mount (અક્ષર અચલ *mount)
{
  સંરચના mount_entry અચલ *le = ખાલી;

  માટે (સંરચના mount_entry અચલ *me = mount_list; me; me = me->me_next)
    {
      જો (streq (me->me_mountdir, mount))
        le = me;
    }

  જો (le)
    {
      અક્ષર *devname = le->me_devname;
      અક્ષર *canon_dev = canonicalize_file_name (devname);
      જો (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        પાછા canon_dev;
      મુક્ત (canon_dev);
      પાછા xstrdup (le->me_devname);
    }
  નહીંતર
    પાછા ખાલી;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
સ્થિર bool
get_device (અક્ષર અચલ *device)
{
  સંરચના mount_entry અચલ *best_match = ખાલી;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  અક્ષર અચલ *file = device;

  અક્ષર *resolved = canonicalize_file_name (device);
  જો (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  માટે (સંરચના mount_entry અચલ *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      અક્ષર *devname = me->me_devname;
      અક્ષર *canon_dev = canonicalize_file_name (me->me_devname);
      જો (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      જો (streq (device, devname))
        {
          અક્ષર *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          જો (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              સંરચના stat device_stats;
              bool this_match_accessible = false;

              જો (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              જો (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  જો (len == 1) /* Traditional root.  */
                    {
                      મુક્ત (last_device);
                      મુક્ત (canon_dev);
                      તોડો;
                    }
                  નહીંતર
                    best_match_len = len;
                }
            }

          મુક્ત (last_device);
        }

      મુક્ત (canon_dev);
    }

  મુક્ત (resolved);

  જો (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, ખાલી,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, ખાલી, false);
      પાછા true;
    }
  નહીંતર જો (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      પાછા true;
    }

  પાછા false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
સ્થિર શૂન્ય
get_point (અક્ષર અચલ *point, અચલ સંરચના stat *statp)
{
  સંરચના stat device_stats;
  સંરચના mount_entry અચલ *best_match = ખાલી;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  અક્ષર *resolved = canonicalize_file_name (point);
  જો (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      માટે (સંરચના mount_entry *me = mount_list; me; me = me->me_next)
        {
          જો (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              જો (best_match_len <= len && len <= resolved_len
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
  મુક્ત (resolved);
  જો (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = ખાલી;

  જો (! best_match)
    માટે (સંરચના mount_entry *me = mount_list; me; me = me->me_next)
      {
        જો (me->me_dev == (dev_t) -1)
          {
            જો (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            નહીંતર
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                જો (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        જો (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            જો (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            નહીંતર
              best_match = me;
          }
      }

  જો (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             ખાલી, false);
  નહીંતર
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      અક્ષર *mp = find_mount_point (point, statp);
      જો (mp)
        {
          get_dev (ખાલી, mp, point, ખાલી, ખાલી,
                   false, false, ખાલી, false);
          મુક્ત (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

સ્થિર શૂન્ય
get_entry (અક્ષર અચલ *name, સંરચના stat અચલ *statp)
{
  જો ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    પાછા;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

સ્થિર શૂન્ય
get_all_entries (શૂન્ય)
{
  filter_mount_list (show_all_fs);

  માટે (સંરચના mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, ખાલી, ખાલી, me->me_type,
             me->me_dummy, me->me_remote, ખાલી, true);
}

/* Add FSTYPE to the list of file system types to display.  */

સ્થિર શૂન્ય
add_fs_type (અક્ષર અચલ *fstype)
{
  સંરચના fs_type_list *fsp;

  fsp = xmalloc (કદ *fsp);
  fsp->fs_name = (અક્ષર *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

સ્થિર શૂન્ય
add_excluded_fs_type (અક્ષર અચલ *fstype)
{
  સંરચના fs_type_list *fsp;

  fsp = xmalloc (કદ *fsp);
  fsp->fs_name = (અક્ષર *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by મૂળભૂત.\n\
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
         કરો not invoke sync before getting usage info (મૂળભૂત)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields જો FIELD_LIST is omitted\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  સંરચના stat *stats = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  અક્ષર અચલ *સંદેશ_mut_excl = _("options %s and %s are mutually exclusive");

  જ્યારે (true)
    {
      પૂર્ણાંક oi = -1;
      પૂર્ણાંક c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'a':
          show_all_fs = true;
          તોડો;
        સ્થિતિ 'B':
          {
            ગણના strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            જો (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          તોડો;
        સ્થિતિ 'i':
          જો (header_mode == OUTPUT_MODE)
            {
              error (0, 0, સંદેશ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          તોડો;
        સ્થિતિ 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          તોડો;
        સ્થિતિ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          તોડો;
        સ્થિતિ 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          તોડો;
        સ્થિતિ 'l':
          show_local_fs = true;
          તોડો;
        સ્થિતિ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          તોડો;
        સ્થિતિ 'T':
          જો (header_mode == OUTPUT_MODE)
            {
              error (0, 0, સંદેશ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          તોડો;
        સ્થિતિ 'P':
          જો (header_mode == OUTPUT_MODE)
            {
              error (0, 0, સંદેશ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          તોડો;
        સ્થિતિ SYNC_OPTION:
          require_sync = true;
          તોડો;
        સ્થિતિ NO_SYNC_OPTION:
          require_sync = false;
          તોડો;

        સ્થિતિ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        સ્થિતિ 't':
          add_fs_type (optarg);
          તોડો;

        સ્થિતિ 'v':		/* For SysV compatibility.  */
          /* ignore */
          તોડો;
        સ્થિતિ 'x':
          add_excluded_fs_type (optarg);
          તોડો;

        સ્થિતિ OUTPUT_OPTION:
          જો (header_mode == INODES_MODE)
            {
              error (0, 0, સંદેશ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          જો (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, સંદેશ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          જો (print_type)
            {
              error (0, 0, સંદેશ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          જો (optarg)
            decode_output_arg (optarg);
          તોડો;

        સ્થિતિ TOTAL_OPTION:
          print_grand_total = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (human_output_opts == -1)
    {
      જો (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      નહીંતર
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  જો (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  નહીંતર જો (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  નહીંતર જો (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    માટે (સંરચના fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        માટે (સંરચના fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            જો (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                તોડો;
              }
          }
      }
    જો (match)
      પાછા EXIT_FAILURE;
  }

  જો (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, કદ *stats);
      માટે (પૂર્ણાંક i = optind; i < argc; ++i)
        {
          પૂર્ણાંક err = automount_stat_err (argv[i], &stats[i - optind]);
          જો (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = ખાલી;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ખાલી
                            || fs_exclude_list != ખાલી
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  જો (mount_list == ખાલી)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      પૂર્ણાંક status = 0;
      જો ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ખાલી
               || fs_exclude_list != ખાલી))
        {
          status = EXIT_FAILURE;
        }
      અક્ષર અચલ *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  જો (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  જો (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      માટે (પૂર્ણાંક i = optind; i < argc; ++i)
        જો (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  નહીંતર
    get_all_entries ();

  જો (file_systems_processed)
    {
      જો (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 ખાલી, ખાલી, ખાલી, false, false, &grand_fsu, false);

      print_table ();
    }
  નહીંતર
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      જો (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
