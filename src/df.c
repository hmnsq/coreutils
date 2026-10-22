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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <c-ctype.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "canonicalize.h"
#അന്തര്ഭാവ "fsusage.h"
#അന്തര്ഭാവ "human.h"
#അന്തര്ഭാവ "mbswidth.h"
#അന്തര്ഭാവ "mountlist.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "find-mount-point.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "xstrtol-error.h"
#അന്തര്ഭാവ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ഘടന devlist
{
  dev_t dev_അക്കം;
  ഘടന mount_entry *me;
  ഘടന devlist *next;
  ഘടന devlist *next_same_dev;
  ഘടന devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
സ്ഥിതം Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
സ്ഥിതം bool show_all_fs;

/* If true, show only local file systems.  */
സ്ഥിതം bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
സ്ഥിതം bool show_listed_fs;

/* Human-readable options for output.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ human_output_opts = -1;

/* The units to use when printing sizes.  */
സ്ഥിതം uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
സ്ഥിതം bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
സ്ഥിതം bool require_sync;

/* Desired exit status.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ exit_status;

/* A file system type to display.  */

ഘടന fs_type_list
{
  അക്ഷരം *fs_name;
  ഘടന fs_type_list *fs_next;
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

സ്ഥിതം ഘടന fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

സ്ഥിതം ഘടന fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
സ്ഥിതം ഘടന mount_entry *mount_list;

/* If true, print file system type as well.  */
സ്ഥിതം bool print_type;

/* If true, print a grand total at the end.  */
സ്ഥിതം bool print_grand_total;

/* Grand total data.  */
സ്ഥിതം ഘടന fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
സ്ഥിതം എണ്ണൽ
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
തരനിർവചനം എണ്ണൽ
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
തരനിർവചനം എണ്ണൽ
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
ഘടന field_data_t
{
  display_field_t field;
  അക്ഷരം സ്ഥിരം *arg;
  field_type_t field_type;
  അക്ഷരം സ്ഥിരം *caption;/* NULL means use default header of this field.  */
  പൂർണ്ണസംഖ്യ width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_വലത്;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
സ്ഥിതം ഘടന field_data_t field_data[] = {
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

സ്ഥിതം അക്ഷരം സ്ഥിരം *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

സ്ഥിതം ഘടന
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  അക്ഷരം ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ഘടന field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
ഘടന field_മൂല്യങ്ങൾ_t
{
  uintmax_t നിവേശം_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_വേര്;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"all", no_argument, ശൂന്യ, 'a'},
  {"block-size", required_argument, ശൂന്യ, 'B'},
  {"inodes", no_argument, ശൂന്യ, 'i'},
  {"human-readable", no_argument, ശൂന്യ, 'h'},
  {"si", no_argument, ശൂന്യ, 'H'},
  {"local", no_argument, ശൂന്യ, 'l'},
  {"output", optional_argument, ശൂന്യ, OUTPUT_OPTION},
  {"portability", no_argument, ശൂന്യ, 'P'},
  {"print-type", no_argument, ശൂന്യ, 'T'},
  {"sync", no_argument, ശൂന്യ, SYNC_OPTION},
  {"no-sync", no_argument, ശൂന്യ, NO_SYNC_OPTION},
  {"total", no_argument, ശൂന്യ, TOTAL_OPTION},
  {"type", required_argument, ശൂന്യ, 't'},
  {"exclude-type", required_argument, ശൂന്യ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
automount_stat_err (അക്ഷരം സ്ഥിരം *file, ഘടന stat *st)
{
  പൂർണ്ണസംഖ്യ fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  എങ്കിൽ (fd < 0)
    {
      എങ്കിൽ (errno == ENOENT || errno == ENOTDIR)
        തിരികെ errno;
      തിരികെ stat (file, st) == 0 ? 0 : errno;
    }
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      തിരികെ err;
    }
}

എണ്ണൽ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

സ്ഥിതം ശൂന്യം
replace_control_chars (അക്ഷരം *cell)
{
  അക്ഷരം *p = cell;
  ആകുമ്പോൾ (*p)
    {
      എങ്കിൽ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

സ്ഥിതം ശൂന്യം
replace_invalid_chars (അക്ഷരം *cell)
{
  അക്ഷരം *srcend = cell + strlen (cell);
  അക്ഷരം *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  വേണ്ടി (അക്ഷരം *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      എങ്കിൽ (ok)
        ok = !c32iscntrl (wc);
      അല്ലെങ്കിൽ
        n = 1;

      എങ്കിൽ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      അല്ലെങ്കിൽ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

സ്ഥിതം ശൂന്യം
replace_problematic_chars (അക്ഷരം *cell)
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ tty_out = -1;
  എങ്കിൽ (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

സ്ഥിതം ശൂന്യം
alloc_table_row (ശൂന്യം)
{
  എങ്കിൽ (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          വലുപ്പം *table.rows);
  idx_t സ്ഥിരം row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, വലുപ്പം *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

സ്ഥിതം ശൂന്യം
print_table (ശൂന്യം)
{
  വേണ്ടി (idx_t row = 0; row < table.nrows; row++)
    {
      വേണ്ടി (idx_t col = 0; col < table.ncolumns; col++)
        {
          അക്ഷരം *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          എങ്കിൽ (col != 0)
            putchar (' ');

          പൂർണ്ണസംഖ്യ width = mbswidth (cell, MBSWIDTH_FLAGS);
          പൂർണ്ണസംഖ്യ fill = width < 0 ? 0 : table.columns[col]->width - width;
          എങ്കിൽ (table.columns[col]->align_വലത്)
            വേണ്ടി (; 0 < fill; fill--)
              putchar (' ');
          fputs (cell, stdout);
          എങ്കിൽ (col + 1 < table.ncolumns)
            വേണ്ടി (; 0 < fill; fill--)
              putchar (' ');
        }
      putchar ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

സ്ഥിതം ശൂന്യം
alloc_field (പൂർണ്ണസംഖ്യ f, അക്ഷരം സ്ഥിരം *c)
{
  എങ്കിൽ (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             വലുപ്പം *table.columns);
  idx_t സ്ഥിരം column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  എങ്കിൽ (c != ശൂന്യ)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
സ്ഥിതം ശൂന്യം
decode_output_arg (അക്ഷരം സ്ഥിരം *arg)
{
  അക്ഷരം *arg_writable = xstrdup (arg);
  അക്ഷരം *s = arg_writable;
  ചെയ്യുക
    {
      /* find next comma */
      അക്ഷരം *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      എങ്കിൽ (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      വേണ്ടി (idx_t i = 0; i < countof (field_data); i++)
        {
          എങ്കിൽ (streq (field_data[i].arg, s))
            {
              field = i;
              നിർത്തുക;
            }
        }
      എങ്കിൽ (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      എങ്കിൽ (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (EXIT_FAILURE);
        }

      തിരഞ്ഞെടുപ്പ് (field)
        {
        അവസ്ഥ SOURCE_FIELD:
        അവസ്ഥ FSTYPE_FIELD:
        അവസ്ഥ USED_FIELD:
        അവസ്ഥ PCENT_FIELD:
        അവസ്ഥ ITOTAL_FIELD:
        അവസ്ഥ IUSED_FIELD:
        അവസ്ഥ IAVAIL_FIELD:
        അവസ്ഥ IPCENT_FIELD:
        അവസ്ഥ TARGET_FIELD:
        അവസ്ഥ FILE_FIELD:
          alloc_field (field, ശൂന്യ);
          നിർത്തുക;

        അവസ്ഥ SIZE_FIELD:
          alloc_field (field, N_("Size"));
          നിർത്തുക;

        അവസ്ഥ AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          നിർത്തുക;

        അവസ്ഥ INVALID_FIELD:
        സ്ഥിരസ്ഥിതി:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ആകുമ്പോൾ (s);

  സ്വതന്ത്രം (arg_writable);
}

/* Get the appropriate columns for the mode.  */
സ്ഥിതം ശൂന്യം
get_field_list (ശൂന്യം)
{
  തിരഞ്ഞെടുപ്പ് (header_mode)
    {
    അവസ്ഥ DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, ശൂന്യ);
      എങ്കിൽ (print_type)
        alloc_field (FSTYPE_FIELD, ശൂന്യ);
      alloc_field (SIZE_FIELD,   ശൂന്യ);
      alloc_field (USED_FIELD,   ശൂന്യ);
      alloc_field (AVAIL_FIELD,  ശൂന്യ);
      alloc_field (PCENT_FIELD,  ശൂന്യ);
      alloc_field (TARGET_FIELD, ശൂന്യ);
      നിർത്തുക;

    അവസ്ഥ HUMAN_MODE:
      alloc_field (SOURCE_FIELD, ശൂന്യ);
      എങ്കിൽ (print_type)
        alloc_field (FSTYPE_FIELD, ശൂന്യ);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   ശൂന്യ);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  ശൂന്യ);
      alloc_field (TARGET_FIELD, ശൂന്യ);
      നിർത്തുക;

    അവസ്ഥ INODES_MODE:
      alloc_field (SOURCE_FIELD, ശൂന്യ);
      എങ്കിൽ (print_type)
        alloc_field (FSTYPE_FIELD, ശൂന്യ);
      alloc_field (ITOTAL_FIELD,  ശൂന്യ);
      alloc_field (IUSED_FIELD,   ശൂന്യ);
      alloc_field (IAVAIL_FIELD,  ശൂന്യ);
      alloc_field (IPCENT_FIELD,  ശൂന്യ);
      alloc_field (TARGET_FIELD,  ശൂന്യ);
      നിർത്തുക;

    അവസ്ഥ POSIX_MODE:
      alloc_field (SOURCE_FIELD, ശൂന്യ);
      എങ്കിൽ (print_type)
        alloc_field (FSTYPE_FIELD, ശൂന്യ);
      alloc_field (SIZE_FIELD,   ശൂന്യ);
      alloc_field (USED_FIELD,   ശൂന്യ);
      alloc_field (AVAIL_FIELD,  ശൂന്യ);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, ശൂന്യ);
      നിർത്തുക;

    അവസ്ഥ OUTPUT_MODE:
      എങ്കിൽ (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

സ്ഥിതം ശൂന്യം
get_header (ശൂന്യം)
{
  alloc_table_row ();

  വേണ്ടി (idx_t col = 0; col < table.ncolumns; col++)
    {
      അക്ഷരം *cell;
      അക്ഷരം സ്ഥിരം *header = _(table.columns[col]->caption);

      എങ്കിൽ (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          അക്ഷരം buf[LONGEST_HUMAN_READABLE + 1];

          പൂർണ്ണസംഖ്യ opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          ചെയ്യുക
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ആകുമ്പോൾ (divisible_by_1000 & divisible_by_1024);

          എങ്കിൽ (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          എങ്കിൽ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          എങ്കിൽ (! (opts & human_base_1024))
            opts |= human_B;

          അക്ഷരം *അക്കം = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), അക്കം, header);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      അല്ലെങ്കിൽ
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      പൂർണ്ണസംഖ്യ cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
സ്ഥിതം bool
selected_fstype (അക്ഷരം സ്ഥിരം *fstype)
{
  എങ്കിൽ (fs_select_list == ശൂന്യ || fstype == ശൂന്യ)
    തിരികെ true;
  വേണ്ടി (സ്ഥിരം ഘടന fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    എങ്കിൽ (streq (fstype, fsp->fs_name))
      തിരികെ true;
  തിരികെ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
സ്ഥിതം bool
excluded_fstype (അക്ഷരം സ്ഥിരം *fstype)
{
  എങ്കിൽ (fs_exclude_list == ശൂന്യ || fstype == ശൂന്യ)
    തിരികെ false;
  വേണ്ടി (സ്ഥിരം ഘടന fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    എങ്കിൽ (streq (fstype, fsp->fs_name))
      തിരികെ true;
  തിരികെ false;
}

സ്ഥിതം size_t
devlist_hash (ശൂന്യം സ്ഥിരം *x, size_t table_size)
{
  ഘടന devlist സ്ഥിരം *p = x;
  തിരികെ (uintmax_t) {p->dev_അക്കം} % table_size;
}

സ്ഥിതം bool
devlist_compare (ശൂന്യം സ്ഥിരം *x, ശൂന്യം സ്ഥിരം *y)
{
  ഘടന devlist സ്ഥിരം *a = x;
  ഘടന devlist സ്ഥിരം *b = y;
  തിരികെ a->dev_അക്കം == b->dev_അക്കം;
}

സ്ഥിതം ഘടന devlist *
devlist_for_dev (dev_t dev)
{
  എങ്കിൽ (devlist_table == ശൂന്യ)
    തിരികെ ശൂന്യ;
  ഘടന devlist dev_entry;
  dev_entry.dev_അക്കം = dev;

  ഘടന devlist *found = hash_lookup (devlist_table, &dev_entry);
  എങ്കിൽ (found == ശൂന്യ)
    തിരികെ ശൂന്യ;

  /* Return the last devlist entry we have seen with this dev_num */
  തിരികെ found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

സ്ഥിതം ശൂന്യം
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ഘടന devlist *device_list = ശൂന്യ;
  പൂർണ്ണസംഖ്യ mount_list_size = 0;

  വേണ്ടി (ഘടന mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, ശൂന്യ,
                                   devlist_hash, devlist_compare, ശൂന്യ);
  എങ്കിൽ (devlist_table == ശൂന്യ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  വേണ്ടി (ഘടന mount_entry *me = mount_list; me;)
    {
      ഘടന stat buf;
      ഘടന mount_entry *discard_me = ശൂന്യ;
      ഘടന devlist *last_seen_dev = ശൂന്യ, *seen_dev = ശൂന്യ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      എങ്കിൽ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      അല്ലെങ്കിൽ
        {
          /* If we've already seen this device...  */
          last_seen_dev = seen_dev = devlist_for_dev (buf.st_dev);

          വേണ്ടി (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_വേര് = strlen (seen_dev->me->me_mountdir)
                                        > strlen (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_വേര് = seen_dev->me->me_mntroot != ശൂന്യ
                                       && me->me_mntroot != ശൂന്യ
                                       && (strlen (seen_dev->me->me_mntroot)
                                           < strlen (me->me_mntroot));
              എങ്കിൽ (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              അല്ലെങ്കിൽ എങ്കിൽ ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_വേര് && ! source_below_വേര്)
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
              അല്ലെങ്കിൽ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      എങ്കിൽ (discard_me)
        {
          me = me->me_next;
          എങ്കിൽ (! devices_only)
            free_mount_entry (discard_me);
        }
      അല്ലെങ്കിൽ
        {
          /* Add the device number to the device_table.  */
          ഘടന devlist *devlist = xmalloc (വലുപ്പം *devlist);
          devlist->me = me;
          devlist->dev_അക്കം = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          ഘടന devlist *hash_entry = hash_തിരുകുക (devlist_table, devlist);
          എങ്കിൽ (hash_entry == ശൂന്യ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  എങ്കിൽ (! devices_only)
    {
      mount_list = ശൂന്യ;
      ആകുമ്പോൾ (device_list)
        {
          /* Add the mount entry.  */
          ഘടന mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          ഘടന devlist *next = device_list->next;
          സ്വതന്ത്രം (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = ശൂന്യ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
സ്ഥിതം ഘടന mount_entry സ്ഥിരം *
me_for_dev (dev_t dev)
{
  ഘടന devlist *dl = devlist_for_dev (dev);
  എങ്കിൽ (dl)
        തിരികെ dl->me;

  തിരികെ ശൂന്യ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
സ്ഥിതം bool
known_മൂല്യം (uintmax_t n)
{
  തിരികെ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
df_readable (bool negative, uintmax_t n, അക്ഷരം *buf,
             uintmax_t നിവേശം_units, uintmax_t output_units)
{
  എങ്കിൽ (! known_മൂല്യം (n) && !negative)
    തിരികെ "-";
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, നിവേശം_units, output_units);
      എങ്കിൽ (negative)
        *--p = '-';
      തിരികെ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
സ്ഥിതം ശൂന്യം
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  എങ്കിൽ (*dest_neg == src_neg)
    {
      *dest += src;
      തിരികെ;
    }

  എങ്കിൽ (*dest_neg)
    *dest = -*dest;

  എങ്കിൽ (src_neg)
    src = -src;

  എങ്കിൽ (src < *dest)
    *dest -= src;
  അല്ലെങ്കിൽ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  എങ്കിൽ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
സ്ഥിതം bool
has_uuid_suffix (അക്ഷരം സ്ഥിരം *s)
{
  size_t len = strlen (s);
  തിരികെ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
സ്ഥിതം ശൂന്യം
get_field_മൂല്യങ്ങൾ (ഘടന field_മൂല്യങ്ങൾ_t *bv,
                  ഘടന field_മൂല്യങ്ങൾ_t *iv,
                  സ്ഥിരം ഘടന fs_usage *fsu)
{
  /* Inode values.  */
  iv->നിവേശം_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_വേര് = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  എങ്കിൽ (known_മൂല്യം (iv->total) && known_മൂല്യം (iv->available_to_വേര്))
    {
      iv->used = iv->total - iv->available_to_വേര്;
      iv->negate_used = (iv->total < iv->available_to_വേര്);
    }

  /* Block values.  */
  bv->നിവേശം_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_വേര് = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_മുകൾ_bit_set
                         && known_മൂല്യം (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  എങ്കിൽ (known_മൂല്യം (bv->total) && known_മൂല്യം (bv->available_to_വേര്))
    {
      bv->used = bv->total - bv->available_to_വേര്;
      bv->negate_used = (bv->total < bv->available_to_വേര്);
    }
}

/* Add block and inode values to grand total.  */
സ്ഥിതം ശൂന്യം
add_to_grand_total (ഘടന field_മൂല്യങ്ങൾ_t *bv, ഘടന field_മൂല്യങ്ങൾ_t *iv)
{
  എങ്കിൽ (known_മൂല്യം (iv->total))
    grand_fsu.fsu_files += iv->total;
  എങ്കിൽ (known_മൂല്യം (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  എങ്കിൽ (known_മൂല്യം (bv->total))
    grand_fsu.fsu_blocks += bv->നിവേശം_units * bv->total;
  എങ്കിൽ (known_മൂല്യം (bv->available_to_വേര്))
    grand_fsu.fsu_bfree += bv->നിവേശം_units * bv->available_to_വേര്;
  എങ്കിൽ (known_മൂല്യം (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_മുകൾ_bit_set,
                            bv->നിവേശം_units * bv->available,
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

സ്ഥിതം ശൂന്യം
get_dev (അക്ഷരം സ്ഥിരം *device, അക്ഷരം സ്ഥിരം *mount_point, അക്ഷരം സ്ഥിരം *file,
         അക്ഷരം സ്ഥിരം *stat_file, അക്ഷരം സ്ഥിരം *fstype,
         bool me_dummy, bool me_remote,
         സ്ഥിരം ഘടന fs_usage *force_fsu,
         bool process_all)
{
  എങ്കിൽ (me_remote && show_local_fs)
    തിരികെ;

  എങ്കിൽ (me_dummy && !show_all_fs && !show_listed_fs)
    തിരികെ;

  എങ്കിൽ (!selected_fstype (fstype) || excluded_fstype (fstype))
    തിരികെ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  എങ്കിൽ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    തിരികെ;

  എങ്കിൽ (!stat_file)
    stat_file = mount_point;

  ഘടന fs_usage fsu;
  എങ്കിൽ (force_fsu)
    fsu = *force_fsu;
  അല്ലെങ്കിൽ എങ്കിൽ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      എങ്കിൽ (process_all && (errno == EACCES || errno == ENOENT))
        {
          എങ്കിൽ (! show_all_fs)
            തിരികെ;

          fstype = "-";
          fsu.fsu_bavail_മുകൾ_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      അല്ലെങ്കിൽ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_status = EXIT_FAILURE;
          തിരികെ;
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ഘടന stat sb;
      എങ്കിൽ (stat (stat_file, &sb) == 0)
        {
          ഘടന mount_entry സ്ഥിരം * dev_me = me_for_dev (sb.st_dev);
          എങ്കിൽ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_മുകൾ_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  എങ്കിൽ (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    തിരികെ;

  എങ്കിൽ (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  എങ്കിൽ (! device)
    device = "-";		/* unknown */

  എങ്കിൽ (! file)
    file = "-";			/* unspecified */

  അക്ഷരം *dev_name = xstrdup (device);
  അക്ഷരം *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  എങ്കിൽ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      സ്വതന്ത്രം (dev_name);
      dev_name = resolved_dev;
    }

  എങ്കിൽ (! fstype)
    fstype = "-";		/* unknown */

  ഘടന field_മൂല്യങ്ങൾ_t block_മൂല്യങ്ങൾ;
  ഘടന field_മൂല്യങ്ങൾ_t inode_മൂല്യങ്ങൾ;
  get_field_മൂല്യങ്ങൾ (&block_മൂല്യങ്ങൾ, &inode_മൂല്യങ്ങൾ, &fsu);

  /* Add to grand total unless processing grand total line.  */
  എങ്കിൽ (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_മൂല്യങ്ങൾ, &inode_മൂല്യങ്ങൾ);

  വേണ്ടി (idx_t col = 0; col < table.ncolumns; col++)
    {
      അക്ഷരം buf[LONGEST_HUMAN_READABLE + 2];
      അക്ഷരം *cell;

      ഘടന field_മൂല്യങ്ങൾ_t *v;
      തിരഞ്ഞെടുപ്പ് (table.columns[col]->field_type)
        {
        അവസ്ഥ BLOCK_FLD:
          v = &block_മൂല്യങ്ങൾ;
          നിർത്തുക;
        അവസ്ഥ INODE_FLD:
          v = &inode_മൂല്യങ്ങൾ;
          നിർത്തുക;
        അവസ്ഥ OTHER_FLD:
          v = ശൂന്യ;
          നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
          affirm (!"bad field_type");
        }

      തിരഞ്ഞെടുപ്പ് (table.columns[col]->field)
        {
        അവസ്ഥ SOURCE_FIELD:
          cell = xstrdup (dev_name);
          നിർത്തുക;

        അവസ്ഥ FSTYPE_FIELD:
          cell = xstrdup (fstype);
          നിർത്തുക;

        അവസ്ഥ SIZE_FIELD:
        അവസ്ഥ ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->നിവേശം_units, v->output_units));
          നിർത്തുക;

        അവസ്ഥ USED_FIELD:
        അവസ്ഥ IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->നിവേശം_units, v->output_units));
          നിർത്തുക;

        അവസ്ഥ AVAIL_FIELD:
        അവസ്ഥ IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->നിവേശം_units, v->output_units));
          നിർത്തുക;

        അവസ്ഥ PCENT_FIELD:
        അവസ്ഥ IPCENT_FIELD:
          {
            ഇരട്ട pct = -1;
            എങ്കിൽ (! known_മൂല്യം (v->used) || ! known_മൂല്യം (v->available))
              ;
            അല്ലെങ്കിൽ എങ്കിൽ (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            അല്ലെങ്കിൽ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ഇരട്ട u = v->negate_used ? - (ഇരട്ട) - v->used : v->used;
                ഇരട്ട a = v->negate_available
                           ? - (ഇരട്ട) - v->available : v->available;
                ഇരട്ട nonroot_total = u + a;
                എങ്കിൽ (nonroot_total)
                  {
                    long പൂർണ്ണസംഖ്യ lipct = pct = u * 100 / nonroot_total;
                    ഇരട്ട ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    എങ്കിൽ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            നിർത്തുക;
          }

        അവസ്ഥ FILE_FIELD:
          cell = xstrdup (file);
          നിർത്തുക;

        അവസ്ഥ TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          എങ്കിൽ (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          അല്ലെങ്കിൽ എങ്കിൽ (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          നിർത്തുക;

        അവസ്ഥ INVALID_FIELD:
        സ്ഥിരസ്ഥിതി:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      പൂർണ്ണസംഖ്യ cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  സ്വതന്ത്രം (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
സ്ഥിതം അക്ഷരം *
last_device_for_mount (അക്ഷരം സ്ഥിരം *mount)
{
  ഘടന mount_entry സ്ഥിരം *le = ശൂന്യ;

  വേണ്ടി (ഘടന mount_entry സ്ഥിരം *me = mount_list; me; me = me->me_next)
    {
      എങ്കിൽ (streq (me->me_mountdir, mount))
        le = me;
    }

  എങ്കിൽ (le)
    {
      അക്ഷരം *devname = le->me_devname;
      അക്ഷരം *canon_dev = canonicalize_file_name (devname);
      എങ്കിൽ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        തിരികെ canon_dev;
      സ്വതന്ത്രം (canon_dev);
      തിരികെ xstrdup (le->me_devname);
    }
  അല്ലെങ്കിൽ
    തിരികെ ശൂന്യ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
സ്ഥിതം bool
get_device (അക്ഷരം സ്ഥിരം *device)
{
  ഘടന mount_entry സ്ഥിരം *best_match = ശൂന്യ;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  അക്ഷരം സ്ഥിരം *file = device;

  അക്ഷരം *resolved = canonicalize_file_name (device);
  എങ്കിൽ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_match_len = SIZE_MAX;
  വേണ്ടി (ഘടന mount_entry സ്ഥിരം *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      അക്ഷരം *devname = me->me_devname;
      അക്ഷരം *canon_dev = canonicalize_file_name (me->me_devname);
      എങ്കിൽ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      എങ്കിൽ (streq (device, devname))
        {
          അക്ഷരം *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          size_t len = strlen (me->me_mountdir);

          എങ്കിൽ (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              ഘടന stat device_stats;
              bool this_match_accessible = false;

              എങ്കിൽ (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              എങ്കിൽ (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  എങ്കിൽ (len == 1) /* Traditional root.  */
                    {
                      സ്വതന്ത്രം (last_device);
                      സ്വതന്ത്രം (canon_dev);
                      നിർത്തുക;
                    }
                  അല്ലെങ്കിൽ
                    best_match_len = len;
                }
            }

          സ്വതന്ത്രം (last_device);
        }

      സ്വതന്ത്രം (canon_dev);
    }

  സ്വതന്ത്രം (resolved);

  എങ്കിൽ (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, ശൂന്യ,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, ശൂന്യ, false);
      തിരികെ true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = EXIT_FAILURE;
      തിരികെ true;
    }

  തിരികെ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
സ്ഥിതം ശൂന്യം
get_point (അക്ഷരം സ്ഥിരം *point, സ്ഥിരം ഘടന stat *statp)
{
  ഘടന stat device_stats;
  ഘടന mount_entry സ്ഥിരം *best_match = ശൂന്യ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  അക്ഷരം *resolved = canonicalize_file_name (point);
  എങ്കിൽ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = strlen (resolved);
      size_t best_match_len = 0;

      വേണ്ടി (ഘടന mount_entry *me = mount_list; me; me = me->me_next)
        {
          എങ്കിൽ (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              size_t len = strlen (me->me_mountdir);
              എങ്കിൽ (best_match_len <= len && len <= resolved_len
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
  സ്വതന്ത്രം (resolved);
  എങ്കിൽ (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = ശൂന്യ;

  എങ്കിൽ (! best_match)
    വേണ്ടി (ഘടന mount_entry *me = mount_list; me; me = me->me_next)
      {
        എങ്കിൽ (me->me_dev == (dev_t) -1)
          {
            എങ്കിൽ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            അല്ലെങ്കിൽ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                എങ്കിൽ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_status = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        എങ്കിൽ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            എങ്കിൽ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            അല്ലെങ്കിൽ
              best_match = me;
          }
      }

  എങ്കിൽ (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             ശൂന്യ, false);
  അല്ലെങ്കിൽ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      അക്ഷരം *mp = find_mount_point (point, statp);
      എങ്കിൽ (mp)
        {
          get_dev (ശൂന്യ, mp, point, ശൂന്യ, ശൂന്യ,
                   false, false, ശൂന്യ, false);
          സ്വതന്ത്രം (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

സ്ഥിതം ശൂന്യം
get_entry (അക്ഷരം സ്ഥിരം *name, ഘടന stat സ്ഥിരം *statp)
{
  എങ്കിൽ ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    തിരികെ;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

സ്ഥിതം ശൂന്യം
get_all_entries (ശൂന്യം)
{
  filter_mount_list (show_all_fs);

  വേണ്ടി (ഘടന mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, ശൂന്യ, ശൂന്യ, me->me_type,
             me->me_dummy, me->me_remote, ശൂന്യ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

സ്ഥിതം ശൂന്യം
add_fs_type (അക്ഷരം സ്ഥിരം *fstype)
{
  ഘടന fs_type_list *fsp;

  fsp = xmalloc (വലുപ്പം *fsp);
  fsp->fs_name = (അക്ഷരം *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

സ്ഥിതം ശൂന്യം
add_excluded_fs_type (അക്ഷരം സ്ഥിരം *fstype)
{
  ഘടന fs_type_list *fsp;

  fsp = xmalloc (വലുപ്പം *fsp);
  fsp->fs_name = (അക്ഷരം *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by സ്ഥിരസ്ഥിതി.\n\
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
         ചെയ്യുക not invoke sync before getting usage info (സ്ഥിരസ്ഥിതി)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields എങ്കിൽ FIELD_LIST is omitted\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ഘടന stat *stats = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  അക്ഷരം സ്ഥിരം *സന്ദേശം_mut_excl = _("options %s and %s are mutually exclusive");

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ oi = -1;
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'a':
          show_all_fs = true;
          നിർത്തുക;
        അവസ്ഥ 'B':
          {
            എണ്ണൽ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            എങ്കിൽ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          നിർത്തുക;
        അവസ്ഥ 'i':
          എങ്കിൽ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = INODES_MODE;
          നിർത്തുക;
        അവസ്ഥ 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          നിർത്തുക;
        അവസ്ഥ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          നിർത്തുക;
        അവസ്ഥ 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          നിർത്തുക;
        അവസ്ഥ 'l':
          show_local_fs = true;
          നിർത്തുക;
        അവസ്ഥ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          നിർത്തുക;
        അവസ്ഥ 'T':
          എങ്കിൽ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          നിർത്തുക;
        അവസ്ഥ 'P':
          എങ്കിൽ (header_mode == OUTPUT_MODE)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          നിർത്തുക;
        അവസ്ഥ SYNC_OPTION:
          require_sync = true;
          നിർത്തുക;
        അവസ്ഥ NO_SYNC_OPTION:
          require_sync = false;
          നിർത്തുക;

        അവസ്ഥ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        അവസ്ഥ 't':
          add_fs_type (optarg);
          നിർത്തുക;

        അവസ്ഥ 'v':		/* For SysV compatibility.  */
          /* ignore */
          നിർത്തുക;
        അവസ്ഥ 'x':
          add_excluded_fs_type (optarg);
          നിർത്തുക;

        അവസ്ഥ OUTPUT_OPTION:
          എങ്കിൽ (header_mode == INODES_MODE)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          എങ്കിൽ (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          എങ്കിൽ (print_type)
            {
              error (0, 0, സന്ദേശം_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          header_mode = OUTPUT_MODE;
          എങ്കിൽ (optarg)
            decode_output_arg (optarg);
          നിർത്തുക;

        അവസ്ഥ TOTAL_OPTION:
          print_grand_total = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (human_output_opts == -1)
    {
      എങ്കിൽ (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      അല്ലെങ്കിൽ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  എങ്കിൽ (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  അല്ലെങ്കിൽ എങ്കിൽ (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  അല്ലെങ്കിൽ എങ്കിൽ (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    വേണ്ടി (ഘടന fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        വേണ്ടി (ഘടന fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            എങ്കിൽ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                നിർത്തുക;
              }
          }
      }
    എങ്കിൽ (match)
      തിരികെ EXIT_FAILURE;
  }

  എങ്കിൽ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, വലുപ്പം *stats);
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; ++i)
        {
          പൂർണ്ണസംഖ്യ err = automount_stat_err (argv[i], &stats[i - optind]);
          എങ്കിൽ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = EXIT_FAILURE;
              argv[i] = ശൂന്യ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ശൂന്യ
                            || fs_exclude_list != ശൂന്യ
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  എങ്കിൽ (mount_list == ശൂന്യ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      പൂർണ്ണസംഖ്യ status = 0;
      എങ്കിൽ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ശൂന്യ
               || fs_exclude_list != ശൂന്യ))
        {
          status = EXIT_FAILURE;
        }
      അക്ഷരം സ്ഥിരം *warning = (status == 0 ? _("Warning: ") : "");
      error (status, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  എങ്കിൽ (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  എങ്കിൽ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; ++i)
        എങ്കിൽ (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  അല്ലെങ്കിൽ
    get_all_entries ();

  എങ്കിൽ (file_systems_processed)
    {
      എങ്കിൽ (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 ശൂന്യ, ശൂന്യ, ശൂന്യ, false, false, &grand_fsu, false);

      print_table ();
    }
  അല്ലെങ്കിൽ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      എങ്കിൽ (exit_status == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
