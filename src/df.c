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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <c-ctype.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "canonicalize.h"
#অন্তর্ভাব "fsusage.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "mbswidth.h"
#অন্তর্ভাব "mountlist.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "find-mount-point.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "xstrtol-error.h"
#অন্তর্ভাব "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

সংরচনা devlist
{
  dev_t dev_অঙ্ক;
  সংরচনা mount_entry *me;
  সংরচনা devlist *next;
  সংরচনা devlist *next_same_dev;
  সংরচনা devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
স্থির Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
স্থির bool show_all_fs;

/* If true, show only local file systems.  */
স্থির bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
স্থির bool show_listed_fs;

/* Human-readable options for output.  */
স্থির পূর্ণাঙ্ক human_output_opts = -1;

/* The units to use when printing sizes.  */
স্থির uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
স্থির bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
স্থির bool require_sync;

/* Desired exit status.  */
স্থির পূর্ণাঙ্ক exit_status;

/* A file system type to display.  */

সংরচনা fs_type_list
{
  বর্ণ *fs_name;
  সংরচনা fs_type_list *fs_next;
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

স্থির সংরচনা fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

স্থির সংরচনা fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
স্থির সংরচনা mount_entry *mount_list;

/* If true, print file system type as well.  */
স্থির bool print_type;

/* If true, print a grand total at the end.  */
স্থির bool print_grand_total;

/* Grand total data.  */
স্থির সংরচনা fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
স্থির গণনা
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
প্রকারনির্দেশ গণনা
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
প্রকারনির্দেশ গণনা
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
সংরচনা field_data_t
{
  display_field_t field;
  বর্ণ ধ্রুব *arg;
  field_type_t field_type;
  বর্ণ ধ্রুব *caption;/* NULL means use default header of this field.  */
  পূর্ণাঙ্ক width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ডান;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
স্থির সংরচনা field_data_t field_data[] = {
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

স্থির বর্ণ ধ্রুব *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

স্থির সংরচনা
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  বর্ণ ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  সংরচনা field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
সংরচনা field_মানসমূহ_t
{
  uintmax_t প্রবেশ_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_মূল;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"all", no_argument, রিক্ত, 'a'},
  {"block-size", required_argument, রিক্ত, 'B'},
  {"inodes", no_argument, রিক্ত, 'i'},
  {"human-readable", no_argument, রিক্ত, 'h'},
  {"si", no_argument, রিক্ত, 'H'},
  {"local", no_argument, রিক্ত, 'l'},
  {"output", optional_argument, রিক্ত, OUTPUT_OPTION},
  {"portability", no_argument, রিক্ত, 'P'},
  {"print-type", no_argument, রিক্ত, 'T'},
  {"sync", no_argument, রিক্ত, SYNC_OPTION},
  {"no-sync", no_argument, রিক্ত, NO_SYNC_OPTION},
  {"total", no_argument, রিক্ত, TOTAL_OPTION},
  {"type", required_argument, রিক্ত, 't'},
  {"exclude-type", required_argument, রিক্ত, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

স্থির পূর্ণাঙ্ক
automount_stat_err (বর্ণ ধ্রুব *file, সংরচনা stat *st)
{
  পূর্ণাঙ্ক fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  যদি (fd < 0)
    {
      যদি (errno == ENOENT || errno == ENOTDIR)
        প্রত্যাগম errno;
      প্রত্যাগম stat (file, st) == 0 ? 0 : errno;
    }
  অন্যথা
    {
      পূর্ণাঙ্ক err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      প্রত্যাগম err;
    }
}

গণনা { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

স্থির শূন্য
replace_control_chars (বর্ণ *cell)
{
  বর্ণ *p = cell;
  যাবত্ (*p)
    {
      যদি (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

স্থির শূন্য
replace_invalid_chars (বর্ণ *cell)
{
  বর্ণ *srcend = cell + strlen (cell);
  বর্ণ *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  প্রতি (বর্ণ *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      যদি (ok)
        ok = !c32iscntrl (wc);
      অন্যথা
        n = 1;

      যদি (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      অন্যথা
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

স্থির শূন্য
replace_problematic_chars (বর্ণ *cell)
{
  স্থির পূর্ণাঙ্ক tty_out = -1;
  যদি (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

স্থির শূন্য
alloc_table_row (শূন্য)
{
  যদি (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          পরিমাণ *table.rows);
  idx_t ধ্রুব row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, পরিমাণ *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

স্থির শূন্য
print_table (শূন্য)
{
  প্রতি (idx_t row = 0; row < table.nrows; row++)
    {
      প্রতি (idx_t col = 0; col < table.ncolumns; col++)
        {
          বর্ণ *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          যদি (col != 0)
            putchar (' ');

          পূর্ণাঙ্ক width = mbswidth (cell, MBSWIDTH_FLAGS);
          পূর্ণাঙ্ক fill = width < 0 ? 0 : table.columns[col]->width - width;
          যদি (table.columns[col]->align_ডান)
            প্রতি (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          যদি (col + 1 < table.ncolumns)
            প্রতি (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

স্থির শূন্য
alloc_field (পূর্ণাঙ্ক f, বর্ণ ধ্রুব *c)
{
  যদি (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             পরিমাণ *table.columns);
  idx_t ধ্রুব column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  যদি (c != রিক্ত)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
স্থির শূন্য
decode_output_arg (বর্ণ ধ্রুব *arg)
{
  বর্ণ *arg_writable = xstrdup (arg);
  বর্ণ *s = arg_writable;
  কুরু
    {
      /* find next comma */
      বর্ণ *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      যদি (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      প্রতি (idx_t i = 0; i < countof (field_data); i++)
        {
          যদি (streq (field_data[i].arg, s))
            {
              field = i;
              ভঙ্গ;
            }
        }
      যদি (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      যদি (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      বিকল্প (field)
        {
        পক্ষ SOURCE_FIELD:
        পক্ষ FSTYPE_FIELD:
        পক্ষ USED_FIELD:
        পক্ষ PCENT_FIELD:
        পক্ষ ITOTAL_FIELD:
        পক্ষ IUSED_FIELD:
        পক্ষ IAVAIL_FIELD:
        পক্ষ IPCENT_FIELD:
        পক্ষ TARGET_FIELD:
        পক্ষ FILE_FIELD:
          alloc_field (field, রিক্ত);
          ভঙ্গ;

        পক্ষ SIZE_FIELD:
          alloc_field (field, N_("Size"));
          ভঙ্গ;

        পক্ষ AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          ভঙ্গ;

        পক্ষ INVALID_FIELD:
        স্বভাব:
          affirm (!"invalid field");
        }
      s = comma;
    }
  যাবত্ (s);

  মুক্ত (arg_writable);
}

/* Get the appropriate columns for the mode.  */
স্থির শূন্য
get_field_list (শূন্য)
{
  বিকল্প (header_mode)
    {
    পক্ষ DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, রিক্ত);
      যদি (print_type)
        alloc_field (FSTYPE_FIELD, রিক্ত);
      alloc_field (SIZE_FIELD,   রিক্ত);
      alloc_field (USED_FIELD,   রিক্ত);
      alloc_field (AVAIL_FIELD,  রিক্ত);
      alloc_field (PCENT_FIELD,  রিক্ত);
      alloc_field (TARGET_FIELD, রিক্ত);
      ভঙ্গ;

    পক্ষ HUMAN_MODE:
      alloc_field (SOURCE_FIELD, রিক্ত);
      যদি (print_type)
        alloc_field (FSTYPE_FIELD, রিক্ত);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   রিক্ত);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  রিক্ত);
      alloc_field (TARGET_FIELD, রিক্ত);
      ভঙ্গ;

    পক্ষ INODES_MODE:
      alloc_field (SOURCE_FIELD, রিক্ত);
      যদি (print_type)
        alloc_field (FSTYPE_FIELD, রিক্ত);
      alloc_field (ITOTAL_FIELD,  রিক্ত);
      alloc_field (IUSED_FIELD,   রিক্ত);
      alloc_field (IAVAIL_FIELD,  রিক্ত);
      alloc_field (IPCENT_FIELD,  রিক্ত);
      alloc_field (TARGET_FIELD,  রিক্ত);
      ভঙ্গ;

    পক্ষ POSIX_MODE:
      alloc_field (SOURCE_FIELD, রিক্ত);
      যদি (print_type)
        alloc_field (FSTYPE_FIELD, রিক্ত);
      alloc_field (SIZE_FIELD,   রিক্ত);
      alloc_field (USED_FIELD,   রিক্ত);
      alloc_field (AVAIL_FIELD,  রিক্ত);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, রিক্ত);
      ভঙ্গ;

    পক্ষ OUTPUT_MODE:
      যদি (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      ভঙ্গ;

    স্বভাব:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

স্থির শূন্য
get_header (শূন্য)
{
  alloc_table_row ();

  প্রতি (idx_t col = 0; col < table.ncolumns; col++)
    {
      বর্ণ *cell;
      বর্ণ ধ্রুব *header = _(table.columns[col]->caption);

      যদি (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          বর্ণ buf[LONGEST_HUMAN_READABLE + 1];

          পূর্ণাঙ্ক opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          কুরু
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          যাবত্ (divisible_by_1000 & divisible_by_1024);

          যদি (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          যদি (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          যদি (! (opts & human_base_1024))
            opts |= human_B;

          বর্ণ *অঙ্ক = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), অঙ্ক, header);
        }
      অন্যথা যদি (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      অন্যথা
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      পূর্ণাঙ্ক cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
স্থির bool
selected_fstype (বর্ণ ধ্রুব *fstype)
{
  যদি (fs_select_list == রিক্ত || fstype == রিক্ত)
    প্রত্যাগম true;
  প্রতি (ধ্রুব সংরচনা fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    যদি (streq (fstype, fsp->fs_name))
      প্রত্যাগম true;
  প্রত্যাগম false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
স্থির bool
excluded_fstype (বর্ণ ধ্রুব *fstype)
{
  যদি (fs_exclude_list == রিক্ত || fstype == রিক্ত)
    প্রত্যাগম false;
  প্রতি (ধ্রুব সংরচনা fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    যদি (streq (fstype, fsp->fs_name))
      প্রত্যাগম true;
  প্রত্যাগম false;
}

স্থির size_t
devlist_hash (শূন্য ধ্রুব *x, size_t table_size)
{
  সংরচনা devlist ধ্রুব *p = x;
  প্রত্যাগম (uintmax_t) {p->dev_অঙ্ক} % table_size;
}

স্থির bool
devlist_compare (শূন্য ধ্রুব *x, শূন্য ধ্রুব *y)
{
  সংরচনা devlist ধ্রুব *a = x;
  সংরচনা devlist ধ্রুব *b = y;
  প্রত্যাগম a->dev_অঙ্ক == b->dev_অঙ্ক;
}

স্থির সংরচনা devlist *
devlist_for_dev (dev_t dev)
{
  যদি (devlist_table == রিক্ত)
    প্রত্যাগম রিক্ত;
  সংরচনা devlist dev_entry;
  dev_entry.dev_অঙ্ক = dev;

  সংরচনা devlist *found = hash_lookup (devlist_table, &dev_entry);
  যদি (found == রিক্ত)
    প্রত্যাগম রিক্ত;

  /* Return the last devlist entry we have seen with this dev_num */
  প্রত্যাগম found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

স্থির শূন্য
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  সংরচনা devlist *device_list = রিক্ত;
  পূর্ণাঙ্ক mount_list_size = 0;

  প্রতি (সংরচনা mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, রিক্ত,
                                   devlist_hash, devlist_compare, রিক্ত);
  যদি (devlist_table == রিক্ত)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  প্রতি (সংরচনা mount_entry *me = mount_list; me;)
    {
      সংরচনা stat buf;
      সংরচনা mount_entry *discard_me = রিক্ত;
      সংরচনা devlist *last_seen_dev = রিক্ত, *seen_dev = রিক্ত;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      যদি ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      অন্যথা
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          প্রতি (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_মূল = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_মূল = seen_dev->me->me_mntroot != রিক্ত
                                       && me->me_mntroot != রিক্ত
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              যদি (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              অন্যথা যদি ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_মূল && ! source_below_মূল)
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
              অন্যথা
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      যদি (discard_me)
        {
          me = me->me_next;
          যদি (! devices_only)
            free_mount_entry (discard_me);
        }
      অন্যথা
        {
          /* Add the device number to the device_table.  */
          সংরচনা devlist *devlist = xmalloc (পরিমাণ *devlist);
          devlist->me = me;
          devlist->dev_অঙ্ক = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          সংরচনা devlist *hash_entry = hash_ঢোকাও (devlist_table, devlist);
          যদি (hash_entry == রিক্ত)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  যদি (! devices_only)
    {
      mount_list = রিক্ত;
      যাবত্ (device_list)
        {
          /* Add the mount entry.  */
          সংরচনা mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          সংরচনা devlist *next = device_list->next;
          মুক্ত (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = রিক্ত;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
স্থির সংরচনা mount_entry ধ্রুব *
me_for_dev (dev_t dev)
{
  সংরচনা devlist *dl = devlist_for_dev (dev);
  যদি (dl)
        প্রত্যাগম dl->me;

  প্রত্যাগম রিক্ত;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
স্থির bool
known_মান (uintmax_t n)
{
  প্রত্যাগম n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

স্থির বর্ণ ধ্রুব *
df_readable (bool negative, uintmax_t n, বর্ণ *buf,
             uintmax_t প্রবেশ_units, uintmax_t output_units)
{
  যদি (! known_মান (n) && !negative)
    প্রত্যাগম "-";
  অন্যথা
    {
      বর্ণ *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, প্রবেশ_units, output_units);
      যদি (negative)
        *--p = '-';
      প্রত্যাগম p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
স্থির শূন্য
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  যদি (*dest_neg == src_neg)
    {
      *dest += src;
      প্রত্যাগম;
    }

  যদি (*dest_neg)
    *dest = -*dest;

  যদি (src_neg)
    src = -src;

  যদি (src < *dest)
    *dest -= src;
  অন্যথা
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  যদি (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
স্থির bool
has_uuid_suffix (বর্ণ ধ্রুব *s)
{
  size_t len = strlen (s);
  প্রত্যাগম (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
স্থির শূন্য
get_field_মানসমূহ (সংরচনা field_মানসমূহ_t *bv,
                  সংরচনা field_মানসমূহ_t *iv,
                  ধ্রুব সংরচনা fs_usage *fsu)
{
  /* Inode values.  */
  iv->প্রবেশ_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_মূল = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  যদি (known_মান (iv->total) && known_মান (iv->available_to_মূল))
    {
      iv->used = iv->total - iv->available_to_মূল;
      iv->negate_used = (iv->total < iv->available_to_মূল);
    }

  /* Block values.  */
  bv->প্রবেশ_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_মূল = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_শীর্ষ_bit_set
                         && known_মান (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  যদি (known_মান (bv->total) && known_মান (bv->available_to_মূল))
    {
      bv->used = bv->total - bv->available_to_মূল;
      bv->negate_used = (bv->total < bv->available_to_মূল);
    }
}

/* Add block and inode values to grand total.  */
স্থির শূন্য
add_to_grand_total (সংরচনা field_মানসমূহ_t *bv, সংরচনা field_মানসমূহ_t *iv)
{
  যদি (known_মান (iv->total))
    grand_fsu.fsu_files += iv->total;
  যদি (known_মান (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  যদি (known_মান (bv->total))
    grand_fsu.fsu_blocks += bv->প্রবেশ_units * bv->total;
  যদি (known_মান (bv->available_to_মূল))
    grand_fsu.fsu_bfree += bv->প্রবেশ_units * bv->available_to_মূল;
  যদি (known_মান (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_শীর্ষ_bit_set,
                            bv->প্রবেশ_units * bv->available,
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

স্থির শূন্য
get_dev (বর্ণ ধ্রুব *device, বর্ণ ধ্রুব *mount_point, বর্ণ ধ্রুব *file,
         বর্ণ ধ্রুব *stat_file, বর্ণ ধ্রুব *fstype,
         bool me_dummy, bool me_remote,
         ধ্রুব সংরচনা fs_usage *force_fsu,
         bool process_all)
{
  যদি (me_remote && show_local_fs)
    প্রত্যাগম;

  যদি (me_dummy && !show_all_fs && !show_listed_fs)
    প্রত্যাগম;

  যদি (!selected_fstype (fstype) || excluded_fstype (fstype))
    প্রত্যাগম;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  যদি (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    প্রত্যাগম;

  যদি (!stat_file)
    stat_file = mount_point;

  সংরচনা fs_usage fsu;
  যদি (force_fsu)
    fsu = *force_fsu;
  অন্যথা যদি (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      যদি (process_all && (errno == EACCES || errno == ENOENT))
        {
          যদি (! show_all_fs)
            প্রত্যাগম;

          fstype = "-";
          fsu.fsu_bavail_শীর্ষ_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      অন্যথা
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          প্রত্যাগম;
        }
    }
  অন্যথা যদি (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      সংরচনা stat sb;
      যদি (stat (stat_file, &sb) == 0)
        {
          সংরচনা mount_entry ধ্রুব * dev_me = me_for_dev (sb.st_dev);
          যদি (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_শীর্ষ_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  যদি (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    প্রত্যাগম;

  যদি (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  যদি (! device)
    device = "-";		/* unknown */

  যদি (! file)
    file = "-";			/* unspecified */

  বর্ণ *dev_name = xstrdup (device);
  বর্ণ *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  যদি (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      মুক্ত (dev_name);
      dev_name = resolved_dev;
    }

  যদি (! fstype)
    fstype = "-";		/* unknown */

  সংরচনা field_মানসমূহ_t block_মানসমূহ;
  সংরচনা field_মানসমূহ_t inode_মানসমূহ;
  get_field_মানসমূহ (&block_মানসমূহ, &inode_মানসমূহ, &fsu);

  /* Add to grand total unless processing grand total line.  */
  যদি (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_মানসমূহ, &inode_মানসমূহ);

  প্রতি (idx_t col = 0; col < table.ncolumns; col++)
    {
      বর্ণ buf[LONGEST_HUMAN_READABLE + 2];
      বর্ণ *cell;

      সংরচনা field_মানসমূহ_t *v;
      বিকল্প (table.columns[col]->field_type)
        {
        পক্ষ BLOCK_FLD:
          v = &block_মানসমূহ;
          ভঙ্গ;
        পক্ষ INODE_FLD:
          v = &inode_মানসমূহ;
          ভঙ্গ;
        পক্ষ OTHER_FLD:
          v = রিক্ত;
          ভঙ্গ;
        স্বভাব:
          affirm (!"bad field_type");
        }

      বিকল্প (table.columns[col]->field)
        {
        পক্ষ SOURCE_FIELD:
          cell = xstrdup (dev_name);
          ভঙ্গ;

        পক্ষ FSTYPE_FIELD:
          cell = xstrdup (fstype);
          ভঙ্গ;

        পক্ষ SIZE_FIELD:
        পক্ষ ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->প্রবেশ_units, v->output_units));
          ভঙ্গ;

        পক্ষ USED_FIELD:
        পক্ষ IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->প্রবেশ_units, v->output_units));
          ভঙ্গ;

        পক্ষ AVAIL_FIELD:
        পক্ষ IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->প্রবেশ_units, v->output_units));
          ভঙ্গ;

        পক্ষ PCENT_FIELD:
        পক্ষ IPCENT_FIELD:
          {
            দ্বিগুণ pct = -1;
            যদি (! known_মান (v->used) || ! known_মান (v->available))
              ;
            অন্যথা যদি (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            অন্যথা
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                দ্বিগুণ u = v->negate_used ? - (দ্বিগুণ) - v->used : v->used;
                দ্বিগুণ a = v->negate_available
                           ? - (দ্বিগুণ) - v->available : v->available;
                দ্বিগুণ nonroot_total = u + a;
                যদি (nonroot_total)
                  {
                    long পূর্ণাঙ্ক lipct = pct = u * 100 / nonroot_total;
                    দ্বিগুণ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    যদি (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ভঙ্গ;
          }

        পক্ষ FILE_FIELD:
          cell = xstrdup (file);
          ভঙ্গ;

        পক্ষ TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          যদি (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          অন্যথা যদি (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          ভঙ্গ;

        পক্ষ INVALID_FIELD:
        স্বভাব:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      পূর্ণাঙ্ক cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  মুক্ত (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
স্থির বর্ণ *
last_device_for_mount (বর্ণ ধ্রুব *mount)
{
  সংরচনা mount_entry ধ্রুব *le = রিক্ত;

  প্রতি (সংরচনা mount_entry ধ্রুব *me = mount_list; me; me = me->me_next)
    {
      যদি (streq (me->me_mountdir, mount))
        le = me;
    }

  যদি (le)
    {
      বর্ণ *devname = le->me_devname;
      বর্ণ *canon_dev = canonicalize_file_name (devname);
      যদি (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        প্রত্যাগম canon_dev;
      মুক্ত (canon_dev);
      প্রত্যাগম xstrdup (le->me_devname);
    }
  অন্যথা
    প্রত্যাগম রিক্ত;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
স্থির bool
get_device (বর্ণ ধ্রুব *device)
{
  সংরচনা mount_entry ধ্রুব *best_match = রিক্ত;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  বর্ণ ধ্রুব *file = device;

  বর্ণ *resolved = canonicalize_file_name (device);
  যদি (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  প্রতি (সংরচনা mount_entry ধ্রুব *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      বর্ণ *devname = me->me_devname;
      বর্ণ *canon_dev = canonicalize_file_name (me->me_devname);
      যদি (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      যদি (streq (device, devname))
        {
          বর্ণ *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          যদি (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              সংরচনা stat device_stats;
              bool this_match_accessible = false;

              যদি (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              যদি (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  যদি (len == 1) /* Traditional root.  */
                    {
                      মুক্ত (last_device);
                      মুক্ত (canon_dev);
                      ভঙ্গ;
                    }
                  অন্যথা
                    best_match_len = len;
                }
            }

          মুক্ত (last_device);
        }

      মুক্ত (canon_dev);
    }

  মুক্ত (resolved);

  যদি (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, রিক্ত,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, রিক্ত, false);
      প্রত্যাগম true;
    }
  অন্যথা যদি (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      প্রত্যাগম true;
    }

  প্রত্যাগম false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
স্থির শূন্য
get_point (বর্ণ ধ্রুব *point, ধ্রুব সংরচনা stat *statp)
{
  সংরচনা stat device_stats;
  সংরচনা mount_entry ধ্রুব *best_match = রিক্ত;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  বর্ণ *resolved = canonicalize_file_name (point);
  যদি (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      প্রতি (সংরচনা mount_entry *me = mount_list; me; me = me->me_next)
        {
          যদি (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              যদি (best_match_len <= len && len <= resolved_len
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
  মুক্ত (resolved);
  যদি (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = রিক্ত;

  যদি (! best_match)
    প্রতি (সংরচনা mount_entry *me = mount_list; me; me = me->me_next)
      {
        যদি (me->me_dev == (dev_t) -1)
          {
            যদি (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            অন্যথা
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                যদি (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        যদি (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            যদি (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            অন্যথা
              best_match = me;
          }
      }

  যদি (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             রিক্ত, false);
  অন্যথা
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      বর্ণ *mp = find_mount_point (point, statp);
      যদি (mp)
        {
          get_dev (রিক্ত, mp, point, রিক্ত, রিক্ত,
                   false, false, রিক্ত, false);
          মুক্ত (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

স্থির শূন্য
get_entry (বর্ণ ধ্রুব *name, সংরচনা stat ধ্রুব *statp)
{
  যদি ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    প্রত্যাগম;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

স্থির শূন্য
get_all_entries (শূন্য)
{
  filter_mount_list (show_all_fs);

  প্রতি (সংরচনা mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, রিক্ত, রিক্ত, me->me_type,
             me->me_dummy, me->me_remote, রিক্ত, true);
}

/* Add FSTYPE to the list of file system types to display.  */

স্থির শূন্য
add_fs_type (বর্ণ ধ্রুব *fstype)
{
  সংরচনা fs_type_list *fsp;

  fsp = xmalloc (পরিমাণ *fsp);
  fsp->fs_name = (বর্ণ *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

স্থির শূন্য
add_excluded_fs_type (বর্ণ ধ্রুব *fstype)
{
  সংরচনা fs_type_list *fsp;

  fsp = xmalloc (পরিমাণ *fsp);
  fsp->fs_name = (বর্ণ *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by স্বভাব.\n\
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
         কুরু not invoke sync before getting usage info (স্বভাব)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields যদি FIELD_LIST is omitted\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  সংরচনা stat *stats = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  বর্ণ ধ্রুব *বার্তা_mut_excl = _("options %s and %s are mutually exclusive");

  যাবত্ (true)
    {
      পূর্ণাঙ্ক oi = -1;
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      যদি (c == -1)
        ভঙ্গ;

      বিকল্প (c)
        {
        পক্ষ 'a':
          show_all_fs = true;
          ভঙ্গ;
        পক্ষ 'B':
          {
            গণনা strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            যদি (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ভঙ্গ;
        পক্ষ 'i':
          যদি (header_mode == OUTPUT_MODE)
            {
              error (0, 0, বার্তা_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          ভঙ্গ;
        পক্ষ 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          ভঙ্গ;
        পক্ষ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          ভঙ্গ;
        পক্ষ 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          ভঙ্গ;
        পক্ষ 'l':
          show_local_fs = true;
          ভঙ্গ;
        পক্ষ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          ভঙ্গ;
        পক্ষ 'T':
          যদি (header_mode == OUTPUT_MODE)
            {
              error (0, 0, বার্তা_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ভঙ্গ;
        পক্ষ 'P':
          যদি (header_mode == OUTPUT_MODE)
            {
              error (0, 0, বার্তা_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ভঙ্গ;
        পক্ষ SYNC_OPTION:
          require_sync = true;
          ভঙ্গ;
        পক্ষ NO_SYNC_OPTION:
          require_sync = false;
          ভঙ্গ;

        পক্ষ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        পক্ষ 't':
          add_fs_type (optarg);
          ভঙ্গ;

        পক্ষ 'v':		/* For SysV compatibility.  */
          /* ignore */
          ভঙ্গ;
        পক্ষ 'x':
          add_excluded_fs_type (optarg);
          ভঙ্গ;

        পক্ষ OUTPUT_OPTION:
          যদি (header_mode == INODES_MODE)
            {
              error (0, 0, বার্তা_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          যদি (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, বার্তা_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          যদি (print_type)
            {
              error (0, 0, বার্তা_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          যদি (optarg)
            decode_output_arg (optarg);
          ভঙ্গ;

        পক্ষ TOTAL_OPTION:
          print_grand_total = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  যদি (human_output_opts == -1)
    {
      যদি (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      অন্যথা
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  যদি (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  অন্যথা যদি (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  অন্যথা যদি (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    প্রতি (সংরচনা fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        প্রতি (সংরচনা fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            যদি (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                ভঙ্গ;
              }
          }
      }
    যদি (match)
      প্রত্যাগম EXIT_FAILURE;
  }

  যদি (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, পরিমাণ *stats);
      প্রতি (পূর্ণাঙ্ক i = optind; i < argc; ++i)
        {
          পূর্ণাঙ্ক err = automount_stat_err (argv[i], &stats[i - optind]);
          যদি (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = রিক্ত;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != রিক্ত
                            || fs_exclude_list != রিক্ত
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  যদি (mount_list == রিক্ত)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      পূর্ণাঙ্ক status = 0;
      যদি ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != রিক্ত
               || fs_exclude_list != রিক্ত))
        {
          status = EXIT_FAILURE;
        }
      বর্ণ ধ্রুব *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  যদি (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  যদি (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      প্রতি (পূর্ণাঙ্ক i = optind; i < argc; ++i)
        যদি (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  অন্যথা
    get_all_entries ();

  যদি (file_systems_processed)
    {
      যদি (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 রিক্ত, রিক্ত, রিক্ত, false, false, &grand_fsu, false);

      print_table ();
    }
  অন্যথা
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      যদি (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
