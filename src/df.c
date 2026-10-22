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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <c-ctype.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "canonicalize.h"
#ଅନ୍ତର୍ଭାଵ "fsusage.h"
#ଅନ୍ତର୍ଭାଵ "human.h"
#ଅନ୍ତର୍ଭାଵ "mbswidth.h"
#ଅନ୍ତର୍ଭାଵ "mountlist.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "find-mount-point.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"
#ଅନ୍ତର୍ଭାଵ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ସଂରଚନା devlist
{
  dev_t dev_ଅଙ୍କ;
  ସଂରଚନା mount_entry *me;
  ସଂରଚନା devlist *next;
  ସଂରଚନା devlist *next_same_dev;
  ସଂରଚନା devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ସ୍ଥିର Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ସ୍ଥିର bool show_all_fs;

/* If true, show only local file systems.  */
ସ୍ଥିର bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ସ୍ଥିର bool show_listed_fs;

/* Human-readable options for output.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା human_output_opts = -1;

/* The units to use when printing sizes.  */
ସ୍ଥିର uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
ସ୍ଥିର bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ସ୍ଥିର bool require_sync;

/* Desired exit status.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status;

/* A file system type to display.  */

ସଂରଚନା fs_type_list
{
  ଅକ୍ଷର *fs_name;
  ସଂରଚନା fs_type_list *fs_next;
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

ସ୍ଥିର ସଂରଚନା fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ସ୍ଥିର ସଂରଚନା fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ସ୍ଥିର ସଂରଚନା mount_entry *mount_list;

/* If true, print file system type as well.  */
ସ୍ଥିର bool print_type;

/* If true, print a grand total at the end.  */
ସ୍ଥିର bool print_grand_total;

/* Grand total data.  */
ସ୍ଥିର ସଂରଚନା fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ସ୍ଥିର ଗଣନା
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
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
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
ସଂରଚନା field_data_t
{
  display_field_t field;
  ଅକ୍ଷର ଅଚଳ *arg;
  field_type_t field_type;
  ଅକ୍ଷର ଅଚଳ *caption;/* NULL means use default header of this field.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ଡାହାଣ;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
ସ୍ଥିର ସଂରଚନା field_data_t field_data[] = {
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ସ୍ଥିର ସଂରଚନା
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  ଅକ୍ଷର ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ସଂରଚନା field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  uintmax_t ନିବେଶ_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_ମୂଳ;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"all", no_argument, ଖାଲି, 'a'},
  {"block-size", required_argument, ଖାଲି, 'B'},
  {"inodes", no_argument, ଖାଲି, 'i'},
  {"human-readable", no_argument, ଖାଲି, 'h'},
  {"si", no_argument, ଖାଲି, 'H'},
  {"local", no_argument, ଖାଲି, 'l'},
  {"output", optional_argument, ଖାଲି, OUTPUT_OPTION},
  {"portability", no_argument, ଖାଲି, 'P'},
  {"print-type", no_argument, ଖାଲି, 'T'},
  {"sync", no_argument, ଖାଲି, SYNC_OPTION},
  {"no-sync", no_argument, ଖାଲି, NO_SYNC_OPTION},
  {"total", no_argument, ଖାଲି, TOTAL_OPTION},
  {"type", required_argument, ଖାଲି, 't'},
  {"exclude-type", required_argument, ଖାଲି, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
automount_stat_err (ଅକ୍ଷର ଅଚଳ *file, ସଂରଚନା stat *st)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ଯଦି (fd < 0)
    {
      ଯଦି (errno == ENOENT || errno == ENOTDIR)
        ଫେରନ୍ତୁ errno;
      ଫେରନ୍ତୁ stat (file, st) == 0 ? 0 : errno;
    }
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      ଫେରନ୍ତୁ err;
    }
}

ଗଣନା { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ସ୍ଥିର ଶୂନ୍ୟ
replace_control_chars (ଅକ୍ଷର *cell)
{
  ଅକ୍ଷର *p = cell;
  ଯେତେବେଳେ (*p)
    {
      ଯଦି (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ସ୍ଥିର ଶୂନ୍ୟ
replace_invalid_chars (ଅକ୍ଷର *cell)
{
  ଅକ୍ଷର *srcend = cell + strlen (cell);
  ଅକ୍ଷର *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  ପାଇଁ (ଅକ୍ଷର *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ଯଦି (ok)
        ok = !c32iscntrl (wc);
      ଅନ୍ୟଥା
        n = 1;

      ଯଦି (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ଅନ୍ୟଥା
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ସ୍ଥିର ଶୂନ୍ୟ
replace_problematic_chars (ଅକ୍ଷର *cell)
{
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tty_out = -1;
  ଯଦି (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ସ୍ଥିର ଶୂନ୍ୟ
alloc_table_row (ଶୂନ୍ୟ)
{
  ଯଦି (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          ଆକାର *table.rows);
  idx_t ଅଚଳ row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, ଆକାର *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_table (ଶୂନ୍ୟ)
{
  ପାଇଁ (idx_t row = 0; row < table.nrows; row++)
    {
      ପାଇଁ (idx_t col = 0; col < table.ncolumns; col++)
        {
          ଅକ୍ଷର *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ଯଦି (col != 0)
            putchar (' ');

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = mbswidth (cell, MBSWIDTH_FLAGS);
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fill = width < 0 ? 0 : table.columns[col]->width - width;
          ଯଦି (table.columns[col]->align_ଡାହାଣ)
            ପାଇଁ (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          ଯଦି (col + 1 < table.ncolumns)
            ପାଇଁ (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ସ୍ଥିର ଶୂନ୍ୟ
alloc_field (ପୂର୍ଣ୍ଣସଂଖ୍ୟା f, ଅକ୍ଷର ଅଚଳ *c)
{
  ଯଦି (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             ଆକାର *table.columns);
  idx_t ଅଚଳ column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  ଯଦି (c != ଖାଲି)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ସ୍ଥିର ଶୂନ୍ୟ
decode_output_arg (ଅକ୍ଷର ଅଚଳ *arg)
{
  ଅକ୍ଷର *arg_writable = xstrdup (arg);
  ଅକ୍ଷର *s = arg_writable;
  କର
    {
      /* find next comma */
      ଅକ୍ଷର *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ଯଦି (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      ପାଇଁ (idx_t i = 0; i < countof (field_data); i++)
        {
          ଯଦି (streq (field_data[i].arg, s))
            {
              field = i;
              ଭାଙ୍ଗ;
            }
        }
      ଯଦି (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ଯଦି (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      ବାଛ (field)
        {
        ସ୍ଥିତି SOURCE_FIELD:
        ସ୍ଥିତି FSTYPE_FIELD:
        ସ୍ଥିତି USED_FIELD:
        ସ୍ଥିତି PCENT_FIELD:
        ସ୍ଥିତି ITOTAL_FIELD:
        ସ୍ଥିତି IUSED_FIELD:
        ସ୍ଥିତି IAVAIL_FIELD:
        ସ୍ଥିତି IPCENT_FIELD:
        ସ୍ଥିତି TARGET_FIELD:
        ସ୍ଥିତି FILE_FIELD:
          alloc_field (field, ଖାଲି);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SIZE_FIELD:
          alloc_field (field, N_("Size"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି INVALID_FIELD:
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ଯେତେବେଳେ (s);

  ମୁକ୍ତ (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ସ୍ଥିର ଶୂନ୍ୟ
get_field_list (ଶୂନ୍ୟ)
{
  ବାଛ (header_mode)
    {
    ସ୍ଥିତି DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, ଖାଲି);
      ଯଦି (print_type)
        alloc_field (FSTYPE_FIELD, ଖାଲି);
      alloc_field (SIZE_FIELD,   ଖାଲି);
      alloc_field (USED_FIELD,   ଖାଲି);
      alloc_field (AVAIL_FIELD,  ଖାଲି);
      alloc_field (PCENT_FIELD,  ଖାଲି);
      alloc_field (TARGET_FIELD, ଖାଲି);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି HUMAN_MODE:
      alloc_field (SOURCE_FIELD, ଖାଲି);
      ଯଦି (print_type)
        alloc_field (FSTYPE_FIELD, ଖାଲି);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   ଖାଲି);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  ଖାଲି);
      alloc_field (TARGET_FIELD, ଖାଲି);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି INODES_MODE:
      alloc_field (SOURCE_FIELD, ଖାଲି);
      ଯଦି (print_type)
        alloc_field (FSTYPE_FIELD, ଖାଲି);
      alloc_field (ITOTAL_FIELD,  ଖାଲି);
      alloc_field (IUSED_FIELD,   ଖାଲି);
      alloc_field (IAVAIL_FIELD,  ଖାଲି);
      alloc_field (IPCENT_FIELD,  ଖାଲି);
      alloc_field (TARGET_FIELD,  ଖାଲି);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି POSIX_MODE:
      alloc_field (SOURCE_FIELD, ଖାଲି);
      ଯଦି (print_type)
        alloc_field (FSTYPE_FIELD, ଖାଲି);
      alloc_field (SIZE_FIELD,   ଖାଲି);
      alloc_field (USED_FIELD,   ଖାଲି);
      alloc_field (AVAIL_FIELD,  ଖାଲି);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, ଖାଲି);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OUTPUT_MODE:
      ଯଦି (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ସ୍ଥିର ଶୂନ୍ୟ
get_header (ଶୂନ୍ୟ)
{
  alloc_table_row ();

  ପାଇଁ (idx_t col = 0; col < table.ncolumns; col++)
    {
      ଅକ୍ଷର *cell;
      ଅକ୍ଷର ଅଚଳ *header = _(table.columns[col]->caption);

      ଯଦି (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          ଅକ୍ଷର buf[LONGEST_HUMAN_READABLE + 1];

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          କର
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ଯେତେବେଳେ (divisible_by_1000 & divisible_by_1024);

          ଯଦି (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          ଯଦି (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          ଯଦି (! (opts & human_base_1024))
            opts |= human_B;

          ଅକ୍ଷର *ଅଙ୍କ = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), ଅଙ୍କ, header);
        }
      ଅନ୍ୟଥା ଯଦି (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      ଅନ୍ୟଥା
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
selected_fstype (ଅକ୍ଷର ଅଚଳ *fstype)
{
  ଯଦି (fs_select_list == ଖାଲି || fstype == ଖାଲି)
    ଫେରନ୍ତୁ true;
  ପାଇଁ (ଅଚଳ ସଂରଚନା fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    ଯଦି (streq (fstype, fsp->fs_name))
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
excluded_fstype (ଅକ୍ଷର ଅଚଳ *fstype)
{
  ଯଦି (fs_exclude_list == ଖାଲି || fstype == ଖାଲି)
    ଫେରନ୍ତୁ false;
  ପାଇଁ (ଅଚଳ ସଂରଚନା fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    ଯଦି (streq (fstype, fsp->fs_name))
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର size_t
devlist_hash (ଶୂନ୍ୟ ଅଚଳ *x, size_t table_size)
{
  ସଂରଚନା devlist ଅଚଳ *p = x;
  ଫେରନ୍ତୁ (uintmax_t) {p->dev_ଅଙ୍କ} % table_size;
}

ସ୍ଥିର bool
devlist_compare (ଶୂନ୍ୟ ଅଚଳ *x, ଶୂନ୍ୟ ଅଚଳ *y)
{
  ସଂରଚନା devlist ଅଚଳ *a = x;
  ସଂରଚନା devlist ଅଚଳ *b = y;
  ଫେରନ୍ତୁ a->dev_ଅଙ୍କ == b->dev_ଅଙ୍କ;
}

ସ୍ଥିର ସଂରଚନା devlist *
devlist_for_dev (dev_t dev)
{
  ଯଦି (devlist_table == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;
  ସଂରଚନା devlist dev_entry;
  dev_entry.dev_ଅଙ୍କ = dev;

  ସଂରଚନା devlist *found = hash_lookup (devlist_table, &dev_entry);
  ଯଦି (found == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  /* Return the last devlist entry we have seen with this dev_num */
  ଫେରନ୍ତୁ found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ସ୍ଥିର ଶୂନ୍ୟ
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ସଂରଚନା devlist *device_list = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mount_list_size = 0;

  ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, ଖାଲି,
                                   devlist_hash, devlist_compare, ଖାଲି);
  ଯଦି (devlist_table == ଖାଲି)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me;)
    {
      ସଂରଚନା stat buf;
      ସଂରଚନା mount_entry *discard_me = ଖାଲି;
      ସଂରଚନା devlist *last_seen_dev = ଖାଲି, *seen_dev = ଖାଲି;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ଯଦି ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ଅନ୍ୟଥା
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          ପାଇଁ (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_ମୂଳ = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_ମୂଳ = seen_dev->me->me_mntroot != ଖାଲି
                                       && me->me_mntroot != ଖାଲି
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              ଯଦି (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ଅନ୍ୟଥା ଯଦି ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_ମୂଳ && ! source_below_ମୂଳ)
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
              ଅନ୍ୟଥା
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ଯଦି (discard_me)
        {
          me = me->me_next;
          ଯଦି (! devices_only)
            free_mount_entry (discard_me);
        }
      ଅନ୍ୟଥା
        {
          /* Add the device number to the device_table.  */
          ସଂରଚନା devlist *devlist = xmalloc (ଆକାର *devlist);
          devlist->me = me;
          devlist->dev_ଅଙ୍କ = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          ସଂରଚନା devlist *hash_entry = hash_ଭର୍ତ୍ତି (devlist_table, devlist);
          ଯଦି (hash_entry == ଖାଲି)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ଯଦି (! devices_only)
    {
      mount_list = ଖାଲି;
      ଯେତେବେଳେ (device_list)
        {
          /* Add the mount entry.  */
          ସଂରଚନା mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          ସଂରଚନା devlist *next = device_list->next;
          ମୁକ୍ତ (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = ଖାଲି;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
ସ୍ଥିର ସଂରଚନା mount_entry ଅଚଳ *
me_for_dev (dev_t dev)
{
  ସଂରଚନା devlist *dl = devlist_for_dev (dev);
  ଯଦି (dl)
        ଫେରନ୍ତୁ dl->me;

  ଫେରନ୍ତୁ ଖାଲି;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ସ୍ଥିର bool
known_ମୂଲ୍ୟ (uintmax_t n)
{
  ଫେରନ୍ତୁ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
df_readable (bool negative, uintmax_t n, ଅକ୍ଷର *buf,
             uintmax_t ନିବେଶ_units, uintmax_t output_units)
{
  ଯଦି (! known_ମୂଲ୍ୟ (n) && !negative)
    ଫେରନ୍ତୁ "-";
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ନିବେଶ_units, output_units);
      ଯଦି (negative)
        *--p = '-';
      ଫେରନ୍ତୁ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ସ୍ଥିର ଶୂନ୍ୟ
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ଯଦି (*dest_neg == src_neg)
    {
      *dest += src;
      ଫେରନ୍ତୁ;
    }

  ଯଦି (*dest_neg)
    *dest = -*dest;

  ଯଦି (src_neg)
    src = -src;

  ଯଦି (src < *dest)
    *dest -= src;
  ଅନ୍ୟଥା
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ଯଦି (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
ସ୍ଥିର bool
has_uuid_suffix (ଅକ୍ଷର ଅଚଳ *s)
{
  size_t len = strlen (s);
  ଫେରନ୍ତୁ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ସ୍ଥିର ଶୂନ୍ୟ
get_field_ମୂଲ୍ୟଗୁଡ଼ିକ (ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t *bv,
                  ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t *iv,
                  ଅଚଳ ସଂରଚନା fs_usage *fsu)
{
  /* Inode values.  */
  iv->ନିବେଶ_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_ମୂଳ = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  ଯଦି (known_ମୂଲ୍ୟ (iv->total) && known_ମୂଲ୍ୟ (iv->available_to_ମୂଳ))
    {
      iv->used = iv->total - iv->available_to_ମୂଳ;
      iv->negate_used = (iv->total < iv->available_to_ମୂଳ);
    }

  /* Block values.  */
  bv->ନିବେଶ_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_ମୂଳ = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_ଶୀର୍ଷ_bit_set
                         && known_ମୂଲ୍ୟ (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  ଯଦି (known_ମୂଲ୍ୟ (bv->total) && known_ମୂଲ୍ୟ (bv->available_to_ମୂଳ))
    {
      bv->used = bv->total - bv->available_to_ମୂଳ;
      bv->negate_used = (bv->total < bv->available_to_ମୂଳ);
    }
}

/* Add block and inode values to grand total.  */
ସ୍ଥିର ଶୂନ୍ୟ
add_to_grand_total (ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t *bv, ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t *iv)
{
  ଯଦି (known_ମୂଲ୍ୟ (iv->total))
    grand_fsu.fsu_files += iv->total;
  ଯଦି (known_ମୂଲ୍ୟ (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  ଯଦି (known_ମୂଲ୍ୟ (bv->total))
    grand_fsu.fsu_blocks += bv->ନିବେଶ_units * bv->total;
  ଯଦି (known_ମୂଲ୍ୟ (bv->available_to_ମୂଳ))
    grand_fsu.fsu_bfree += bv->ନିବେଶ_units * bv->available_to_ମୂଳ;
  ଯଦି (known_ମୂଲ୍ୟ (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_ଶୀର୍ଷ_bit_set,
                            bv->ନିବେଶ_units * bv->available,
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

ସ୍ଥିର ଶୂନ୍ୟ
get_dev (ଅକ୍ଷର ଅଚଳ *device, ଅକ୍ଷର ଅଚଳ *mount_point, ଅକ୍ଷର ଅଚଳ *file,
         ଅକ୍ଷର ଅଚଳ *stat_file, ଅକ୍ଷର ଅଚଳ *fstype,
         bool me_dummy, bool me_remote,
         ଅଚଳ ସଂରଚନା fs_usage *force_fsu,
         bool process_all)
{
  ଯଦି (me_remote && show_local_fs)
    ଫେରନ୍ତୁ;

  ଯଦି (me_dummy && !show_all_fs && !show_listed_fs)
    ଫେରନ୍ତୁ;

  ଯଦି (!selected_fstype (fstype) || excluded_fstype (fstype))
    ଫେରନ୍ତୁ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ଯଦି (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    ଫେରନ୍ତୁ;

  ଯଦି (!stat_file)
    stat_file = mount_point;

  ସଂରଚନା fs_usage fsu;
  ଯଦି (force_fsu)
    fsu = *force_fsu;
  ଅନ୍ୟଥା ଯଦି (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ଯଦି (process_all && (errno == EACCES || errno == ENOENT))
        {
          ଯଦି (! show_all_fs)
            ଫେରନ୍ତୁ;

          fstype = "-";
          fsu.fsu_bavail_ଶୀର୍ଷ_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ଅନ୍ୟଥା
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          ଫେରନ୍ତୁ;
        }
    }
  ଅନ୍ୟଥା ଯଦି (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ସଂରଚନା stat sb;
      ଯଦି (stat (stat_file, &sb) == 0)
        {
          ସଂରଚନା mount_entry ଅଚଳ * dev_me = me_for_dev (sb.st_dev);
          ଯଦି (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_ଶୀର୍ଷ_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ଯଦି (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    ଫେରନ୍ତୁ;

  ଯଦି (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  ଯଦି (! device)
    device = "-";		/* unknown */

  ଯଦି (! file)
    file = "-";			/* unspecified */

  ଅକ୍ଷର *dev_name = xstrdup (device);
  ଅକ୍ଷର *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ଯଦି (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      ମୁକ୍ତ (dev_name);
      dev_name = resolved_dev;
    }

  ଯଦି (! fstype)
    fstype = "-";		/* unknown */

  ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t block_ମୂଲ୍ୟଗୁଡ଼ିକ;
  ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t inode_ମୂଲ୍ୟଗୁଡ଼ିକ;
  get_field_ମୂଲ୍ୟଗୁଡ଼ିକ (&block_ମୂଲ୍ୟଗୁଡ଼ିକ, &inode_ମୂଲ୍ୟଗୁଡ଼ିକ, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ଯଦି (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_ମୂଲ୍ୟଗୁଡ଼ିକ, &inode_ମୂଲ୍ୟଗୁଡ଼ିକ);

  ପାଇଁ (idx_t col = 0; col < table.ncolumns; col++)
    {
      ଅକ୍ଷର buf[LONGEST_HUMAN_READABLE + 2];
      ଅକ୍ଷର *cell;

      ସଂରଚନା field_ମୂଲ୍ୟଗୁଡ଼ିକ_t *v;
      ବାଛ (table.columns[col]->field_type)
        {
        ସ୍ଥିତି BLOCK_FLD:
          v = &block_ମୂଲ୍ୟଗୁଡ଼ିକ;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି INODE_FLD:
          v = &inode_ମୂଲ୍ୟଗୁଡ଼ିକ;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି OTHER_FLD:
          v = ଖାଲି;
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          affirm (!"bad field_type");
        }

      ବାଛ (table.columns[col]->field)
        {
        ସ୍ଥିତି SOURCE_FIELD:
          cell = xstrdup (dev_name);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FSTYPE_FIELD:
          cell = xstrdup (fstype);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SIZE_FIELD:
        ସ୍ଥିତି ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->ନିବେଶ_units, v->output_units));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି USED_FIELD:
        ସ୍ଥିତି IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->ନିବେଶ_units, v->output_units));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି AVAIL_FIELD:
        ସ୍ଥିତି IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->ନିବେଶ_units, v->output_units));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PCENT_FIELD:
        ସ୍ଥିତି IPCENT_FIELD:
          {
            ଦ୍ୱିଗୁଣ pct = -1;
            ଯଦି (! known_ମୂଲ୍ୟ (v->used) || ! known_ମୂଲ୍ୟ (v->available))
              ;
            ଅନ୍ୟଥା ଯଦି (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            ଅନ୍ୟଥା
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ଦ୍ୱିଗୁଣ u = v->negate_used ? - (ଦ୍ୱିଗୁଣ) - v->used : v->used;
                ଦ୍ୱିଗୁଣ a = v->negate_available
                           ? - (ଦ୍ୱିଗୁଣ) - v->available : v->available;
                ଦ୍ୱିଗୁଣ nonroot_total = u + a;
                ଯଦି (nonroot_total)
                  {
                    long ପୂର୍ଣ୍ଣସଂଖ୍ୟା lipct = pct = u * 100 / nonroot_total;
                    ଦ୍ୱିଗୁଣ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ଯଦି (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି FILE_FIELD:
          cell = xstrdup (file);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ଯଦି (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          ଅନ୍ୟଥା ଯଦି (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି INVALID_FIELD:
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  ମୁକ୍ତ (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ସ୍ଥିର ଅକ୍ଷର *
last_device_for_mount (ଅକ୍ଷର ଅଚଳ *mount)
{
  ସଂରଚନା mount_entry ଅଚଳ *le = ଖାଲି;

  ପାଇଁ (ସଂରଚନା mount_entry ଅଚଳ *me = mount_list; me; me = me->me_next)
    {
      ଯଦି (streq (me->me_mountdir, mount))
        le = me;
    }

  ଯଦି (le)
    {
      ଅକ୍ଷର *devname = le->me_devname;
      ଅକ୍ଷର *canon_dev = canonicalize_file_name (devname);
      ଯଦି (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        ଫେରନ୍ତୁ canon_dev;
      ମୁକ୍ତ (canon_dev);
      ଫେରନ୍ତୁ xstrdup (le->me_devname);
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ଖାଲି;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ସ୍ଥିର bool
get_device (ଅକ୍ଷର ଅଚଳ *device)
{
  ସଂରଚନା mount_entry ଅଚଳ *best_match = ଖାଲି;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  ଅକ୍ଷର ଅଚଳ *file = device;

  ଅକ୍ଷର *resolved = canonicalize_file_name (device);
  ଯଦି (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  ପାଇଁ (ସଂରଚନା mount_entry ଅଚଳ *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      ଅକ୍ଷର *devname = me->me_devname;
      ଅକ୍ଷର *canon_dev = canonicalize_file_name (me->me_devname);
      ଯଦି (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ଯଦି (streq (device, devname))
        {
          ଅକ୍ଷର *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          ଯଦି (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              ସଂରଚନା stat device_stats;
              bool this_match_accessible = false;

              ଯଦି (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              ଯଦି (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  ଯଦି (len == 1) /* Traditional root.  */
                    {
                      ମୁକ୍ତ (last_device);
                      ମୁକ୍ତ (canon_dev);
                      ଭାଙ୍ଗ;
                    }
                  ଅନ୍ୟଥା
                    best_match_len = len;
                }
            }

          ମୁକ୍ତ (last_device);
        }

      ମୁକ୍ତ (canon_dev);
    }

  ମୁକ୍ତ (resolved);

  ଯଦି (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, ଖାଲି,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, ଖାଲି, false);
      ଫେରନ୍ତୁ true;
    }
  ଅନ୍ୟଥା ଯଦି (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      ଫେରନ୍ତୁ true;
    }

  ଫେରନ୍ତୁ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ସ୍ଥିର ଶୂନ୍ୟ
get_point (ଅକ୍ଷର ଅଚଳ *point, ଅଚଳ ସଂରଚନା stat *statp)
{
  ସଂରଚନା stat device_stats;
  ସଂରଚନା mount_entry ଅଚଳ *best_match = ଖାଲି;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  ଅକ୍ଷର *resolved = canonicalize_file_name (point);
  ଯଦି (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me; me = me->me_next)
        {
          ଯଦି (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              ଯଦି (best_match_len <= len && len <= resolved_len
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
  ମୁକ୍ତ (resolved);
  ଯଦି (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = ଖାଲି;

  ଯଦି (! best_match)
    ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me; me = me->me_next)
      {
        ଯଦି (me->me_dev == (dev_t) -1)
          {
            ଯଦି (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ଅନ୍ୟଥା
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ଯଦି (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ଯଦି (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ଯଦି (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ଅନ୍ୟଥା
              best_match = me;
          }
      }

  ଯଦି (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             ଖାଲି, false);
  ଅନ୍ୟଥା
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      ଅକ୍ଷର *mp = find_mount_point (point, statp);
      ଯଦି (mp)
        {
          get_dev (ଖାଲି, mp, point, ଖାଲି, ଖାଲି,
                   false, false, ଖାଲି, false);
          ମୁକ୍ତ (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ସ୍ଥିର ଶୂନ୍ୟ
get_entry (ଅକ୍ଷର ଅଚଳ *name, ସଂରଚନା stat ଅଚଳ *statp)
{
  ଯଦି ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    ଫେରନ୍ତୁ;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ସ୍ଥିର ଶୂନ୍ୟ
get_all_entries (ଶୂନ୍ୟ)
{
  filter_mount_list (show_all_fs);

  ପାଇଁ (ସଂରଚନା mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, ଖାଲି, ଖାଲି, me->me_type,
             me->me_dummy, me->me_remote, ଖାଲି, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ସ୍ଥିର ଶୂନ୍ୟ
add_fs_type (ଅକ୍ଷର ଅଚଳ *fstype)
{
  ସଂରଚନା fs_type_list *fsp;

  fsp = xmalloc (ଆକାର *fsp);
  fsp->fs_name = (ଅକ୍ଷର *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ସ୍ଥିର ଶୂନ୍ୟ
add_excluded_fs_type (ଅକ୍ଷର ଅଚଳ *fstype)
{
  ସଂରଚନା fs_type_list *fsp;

  fsp = xmalloc (ଆକାର *fsp);
  fsp->fs_name = (ଅକ୍ଷର *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by ପୂର୍ବନିର୍ଦ୍ଧାରିତ.\n\
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
         କର not invoke sync before getting usage info (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields ଯଦି FIELD_LIST is omitted\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ସଂରଚନା stat *stats = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  ଅକ୍ଷର ଅଚଳ *ସନ୍ଦେଶ_mut_excl = _("options %s and %s are mutually exclusive");

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି 'a':
          show_all_fs = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'B':
          {
            ଗଣନା strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ଯଦି (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          ଯଦି (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'l':
          show_local_fs = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'T':
          ଯଦି (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'P':
          ଯଦି (header_mode == OUTPUT_MODE)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି SYNC_OPTION:
          require_sync = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି NO_SYNC_OPTION:
          require_sync = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        ସ୍ଥିତି 't':
          add_fs_type (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':		/* For SysV compatibility.  */
          /* ignore */
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'x':
          add_excluded_fs_type (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି OUTPUT_OPTION:
          ଯଦି (header_mode == INODES_MODE)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ଯଦି (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ଯଦି (print_type)
            {
              error (0, 0, ସନ୍ଦେଶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          ଯଦି (optarg)
            decode_output_arg (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TOTAL_OPTION:
          print_grand_total = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (human_output_opts == -1)
    {
      ଯଦି (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ଅନ୍ୟଥା
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  ଯଦି (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  ଅନ୍ୟଥା ଯଦି (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  ଅନ୍ୟଥା ଯଦି (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    ପାଇଁ (ସଂରଚନା fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        ପାଇଁ (ସଂରଚନା fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            ଯଦି (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                ଭାଙ୍ଗ;
              }
          }
      }
    ଯଦି (match)
      ଫେରନ୍ତୁ EXIT_FAILURE;
  }

  ଯଦି (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, ଆକାର *stats);
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; ++i)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = automount_stat_err (argv[i], &stats[i - optind]);
          ଯଦି (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = ଖାଲି;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ଖାଲି
                            || fs_exclude_list != ଖାଲି
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  ଯଦି (mount_list == ଖାଲି)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = 0;
      ଯଦି ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ଖାଲି
               || fs_exclude_list != ଖାଲି))
        {
          status = EXIT_FAILURE;
        }
      ଅକ୍ଷର ଅଚଳ *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ଯଦି (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  ଯଦି (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; ++i)
        ଯଦି (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  ଅନ୍ୟଥା
    get_all_entries ();

  ଯଦି (file_systems_processed)
    {
      ଯଦି (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 ଖାଲି, ଖାଲି, ଖାଲି, false, false, &grand_fsu, false);

      print_table ();
    }
  ଅନ୍ୟଥା
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ଯଦି (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
