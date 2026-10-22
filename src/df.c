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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <c-ctype.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "fsusage.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "mbswidth.h"
#अन्तर्भाव "mountlist.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "find-mount-point.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "xstrtol-error.h"
#अन्तर्भाव "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "df"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

संरचना devlist
{
  dev_t dev_अङ्क;
  संरचना mount_entry *me;
  संरचना devlist *next;
  संरचना devlist *next_same_dev;
  संरचना devlist *seen_last; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
स्थिर Hash_table *devlist_table;

/* If true, show even file systems with zero size or
   uninteresting types.  */
स्थिर bool show_all_fs;

/* If true, show only local file systems.  */
स्थिर bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
स्थिर bool show_listed_fs;

/* Human-readable options for output.  */
स्थिर पूर्णाङ्क human_output_opts = -1;

/* The units to use when printing sizes.  */
स्थिर uintmax_t output_block_size;

/* True if a file system has been processed for output.  */
स्थिर bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
स्थिर bool require_sync;

/* Desired exit status.  */
स्थिर पूर्णाङ्क exit_status;

/* A file system type to display.  */

संरचना fs_type_list
{
  वर्ण *fs_name;
  संरचना fs_type_list *fs_next;
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

स्थिर संरचना fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

स्थिर संरचना fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
स्थिर संरचना mount_entry *mount_list;

/* If true, print file system type as well.  */
स्थिर bool print_type;

/* If true, print a grand total at the end.  */
स्थिर bool print_grand_total;

/* Grand total data.  */
स्थिर संरचना fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
स्थिर गणना
{
  DEFAULT_MODE,
  INODES_MODE,
  HUMAN_MODE,
  POSIX_MODE,
  OUTPUT_MODE
} header_mode = DEFAULT_MODE;

/* Displayable fields.  */
प्रकारनिर्देश गणना
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
प्रकारनिर्देश गणना
{
  BLOCK_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} field_type_t;

/* Attributes of a display field.  */
संरचना field_data_t
{
  display_field_t field;
  वर्ण अचर *arg;
  field_type_t field_type;
  वर्ण अचर *caption;/* NULL means use default header of this field.  */
  पूर्णाङ्क width;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_दायाँ;   /* Whether to right-align columns, not left-align.  */
  bool used;
};

/* Header strings, minimum width and alignment for the above fields.  */
स्थिर संरचना field_data_t field_data[] = {
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

स्थिर वर्ण अचर *all_args_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

स्थिर संरचना
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  वर्ण ***rows COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  संरचना field_data_t **columns COUNTED_BY (ncolumns);
} table;

/* Field values.  */
संरचना field_मानहरू_t
{
  uintmax_t आगत_units;
  uintmax_t output_units;
  uintmax_t total;
  uintmax_t available;
  bool negate_available;
  uintmax_t available_to_जरा;
  uintmax_t used;
  bool negate_used;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  NO_SYNC_OPTION = अक्षर_उच्च + 1,
  SYNC_OPTION,
  TOTAL_OPTION,
  OUTPUT_OPTION
};

स्थिर संरचना option अचर long_options[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"block-size", required_argument, रिक्त, 'B'},
  {"inodes", no_argument, रिक्त, 'i'},
  {"human-readable", no_argument, रिक्त, 'h'},
  {"si", no_argument, रिक्त, 'H'},
  {"local", no_argument, रिक्त, 'l'},
  {"output", optional_argument, रिक्त, OUTPUT_OPTION},
  {"portability", no_argument, रिक्त, 'P'},
  {"print-type", no_argument, रिक्त, 'T'},
  {"sync", no_argument, रिक्त, SYNC_OPTION},
  {"no-sync", no_argument, रिक्त, NO_SYNC_OPTION},
  {"total", no_argument, रिक्त, TOTAL_OPTION},
  {"type", required_argument, रिक्त, 't'},
  {"exclude-type", required_argument, रिक्त, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

स्थिर पूर्णाङ्क
automount_stat_err (वर्ण अचर *file, संरचना stat *st)
{
  पूर्णाङ्क fd = खोलो (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  यदि (fd < 0)
    {
      यदि (त्रुटि_सं == ENOENT || त्रुटि_सं == ENOTDIR)
        फर्काऊ त्रुटि_सं;
      फर्काऊ stat (file, st) == 0 ? 0 : त्रुटि_सं;
    }
  अन्यथा
    {
      पूर्णाङ्क err = ख_स्थिति (fd, st) == 0 ? 0 : त्रुटि_सं;
      बंद (fd);
      फर्काऊ err;
    }
}

गणना { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

स्थिर शून्य
replace_control_chars (वर्ण *cell)
{
  वर्ण *p = cell;
  जबसम्म (*p)
    {
      यदि (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

स्थिर शून्य
replace_invalid_chars (वर्ण *cell)
{
  वर्ण *srcend = cell + म_माप (cell);
  वर्ण *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  माप_प्रकार n;

  लागि (वर्ण *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      माप_प्रकार srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      यदि (ok)
        ok = !c32iscntrl (wc);
      अन्यथा
        n = 1;

      यदि (ok)
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

स्थिर शून्य
replace_problematic_chars (वर्ण *cell)
{
  स्थिर पूर्णाङ्क tty_out = -1;
  यदि (tty_out < 0)
    tty_out = isatty (STDOUT_FILENO);

  (tty_out ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

स्थिर शून्य
alloc_table_row (शून्य)
{
  यदि (table.nrows == table.nrows_alloc)
    table.rows = xpalloc (table.rows, &table.nrows_alloc, 1, -1,
                          परिमाण *table.rows);
  idx_t अचर row_idx = table.nrows++;
  table.rows[row_idx] = xinmalloc (table.ncolumns, परिमाण *table.rows[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

स्थिर शून्य
print_table (शून्य)
{
  लागि (idx_t row = 0; row < table.nrows; row++)
    {
      लागि (idx_t col = 0; col < table.ncolumns; col++)
        {
          वर्ण *cell = table.rows[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          यदि (col != 0)
            अक्षर_दो (' ');

          पूर्णाङ्क width = mbswidth (cell, MBSWIDTH_FLAGS);
          पूर्णाङ्क fill = width < 0 ? 0 : table.columns[col]->width - width;
          यदि (table.columns[col]->align_दायाँ)
            लागि (; 0 < fill; fill--)
              अक्षर_दो (' ');
          ख_माला_दो (cell, मानक_निकास);
          यदि (col + 1 < table.ncolumns)
            लागि (; 0 < fill; fill--)
              अक्षर_दो (' ');
        }
      अक्षर_दो ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

स्थिर शून्य
alloc_field (पूर्णाङ्क f, वर्ण अचर *c)
{
  यदि (table.ncolumns == table.ncolumns_alloc)
    table.columns = xpalloc (table.columns, &table.ncolumns_alloc, 1, -1,
                             परिमाण *table.columns);
  idx_t अचर column_idx = table.ncolumns++;
  table.columns[column_idx] = &field_data[f];
  यदि (c != रिक्त)
    field_data[f].caption = c;

  affirm (!field_data[f].used);

  /* Mark field as used.  */
  field_data[f].used = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
स्थिर शून्य
decode_output_arg (वर्ण अचर *arg)
{
  वर्ण *arg_writable = xstrdup (arg);
  वर्ण *s = arg_writable;
  गर
    {
      /* find next comma */
      वर्ण *comma = म_अक्षर (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      यदि (comma)
        *comma++ = 0;

      /* process S.  */
      display_field_t field = INVALID_FIELD;
      लागि (idx_t i = 0; i < countof (field_data); i++)
        {
          यदि (streq (field_data[i].arg, s))
            {
              field = i;
              रोक;
            }
        }
      यदि (field == INVALID_FIELD)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (निकास_त्रुटि);
        }

      यदि (field_data[field].used)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (field_data[field].arg));
          usage (निकास_त्रुटि);
        }

      विकल्प (field)
        {
        पक्ष SOURCE_FIELD:
        पक्ष FSTYPE_FIELD:
        पक्ष USED_FIELD:
        पक्ष PCENT_FIELD:
        पक्ष ITOTAL_FIELD:
        पक्ष IUSED_FIELD:
        पक्ष IAVAIL_FIELD:
        पक्ष IPCENT_FIELD:
        पक्ष TARGET_FIELD:
        पक्ष FILE_FIELD:
          alloc_field (field, रिक्त);
          रोक;

        पक्ष SIZE_FIELD:
          alloc_field (field, N_("Size"));
          रोक;

        पक्ष AVAIL_FIELD:
          alloc_field (field, N_("Avail"));
          रोक;

        पक्ष INVALID_FIELD:
        स्वभाव:
          affirm (!"invalid field");
        }
      s = comma;
    }
  जबसम्म (s);

  मुक्त (arg_writable);
}

/* Get the appropriate columns for the mode.  */
स्थिर शून्य
get_field_list (शून्य)
{
  विकल्प (header_mode)
    {
    पक्ष DEFAULT_MODE:
      alloc_field (SOURCE_FIELD, रिक्त);
      यदि (print_type)
        alloc_field (FSTYPE_FIELD, रिक्त);
      alloc_field (SIZE_FIELD,   रिक्त);
      alloc_field (USED_FIELD,   रिक्त);
      alloc_field (AVAIL_FIELD,  रिक्त);
      alloc_field (PCENT_FIELD,  रिक्त);
      alloc_field (TARGET_FIELD, रिक्त);
      रोक;

    पक्ष HUMAN_MODE:
      alloc_field (SOURCE_FIELD, रिक्त);
      यदि (print_type)
        alloc_field (FSTYPE_FIELD, रिक्त);

      alloc_field (SIZE_FIELD,   N_("Size"));
      alloc_field (USED_FIELD,   रिक्त);
      alloc_field (AVAIL_FIELD,  N_("Avail"));
      alloc_field (PCENT_FIELD,  रिक्त);
      alloc_field (TARGET_FIELD, रिक्त);
      रोक;

    पक्ष INODES_MODE:
      alloc_field (SOURCE_FIELD, रिक्त);
      यदि (print_type)
        alloc_field (FSTYPE_FIELD, रिक्त);
      alloc_field (ITOTAL_FIELD,  रिक्त);
      alloc_field (IUSED_FIELD,   रिक्त);
      alloc_field (IAVAIL_FIELD,  रिक्त);
      alloc_field (IPCENT_FIELD,  रिक्त);
      alloc_field (TARGET_FIELD,  रिक्त);
      रोक;

    पक्ष POSIX_MODE:
      alloc_field (SOURCE_FIELD, रिक्त);
      यदि (print_type)
        alloc_field (FSTYPE_FIELD, रिक्त);
      alloc_field (SIZE_FIELD,   रिक्त);
      alloc_field (USED_FIELD,   रिक्त);
      alloc_field (AVAIL_FIELD,  रिक्त);
      alloc_field (PCENT_FIELD,  N_("Capacity"));
      alloc_field (TARGET_FIELD, रिक्त);
      रोक;

    पक्ष OUTPUT_MODE:
      यदि (!table.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_args_string);
        }
      रोक;

    स्वभाव:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

स्थिर शून्य
get_header (शून्य)
{
  alloc_table_row ();

  लागि (idx_t col = 0; col < table.ncolumns; col++)
    {
      वर्ण *cell;
      वर्ण अचर *header = _(table.columns[col]->caption);

      यदि (table.columns[col]->field == SIZE_FIELD
          && (header_mode == DEFAULT_MODE
              || (header_mode == OUTPUT_MODE
                  && !(human_output_opts & human_autoscale))))
        {
          वर्ण buf[LONGEST_HUMAN_READABLE + 1];

          पूर्णाङ्क opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_group_digits | human_base_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_block_size;
          uintmax_t q1024 = output_block_size;
          bool divisible_by_1000;
          bool divisible_by_1024;

          गर
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          जबसम्म (divisible_by_1000 & divisible_by_1024);

          यदि (divisible_by_1000 < divisible_by_1024)
            opts |= human_base_1024;
          यदि (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_base_1024;
          यदि (! (opts & human_base_1024))
            opts |= human_B;

          वर्ण *अङ्क = human_readable (output_block_size, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          header = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), अङ्क, header);
        }
      अन्यथा यदि (header_mode == POSIX_MODE
               && table.columns[col]->field == SIZE_FIELD)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_block_size, header);
        }
      अन्यथा
        cell = xstrdup (header);

      replace_problematic_chars (cell);

      table.rows[table.nrows - 1][col] = cell;

      पूर्णाङ्क cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_PURE
स्थिर bool
selected_fstype (वर्ण अचर *fstype)
{
  यदि (fs_select_list == रिक्त || fstype == रिक्त)
    फर्काऊ true;
  लागि (अचर संरचना fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_next)
    यदि (streq (fstype, fsp->fs_name))
      फर्काऊ true;
  फर्काऊ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_PURE
स्थिर bool
excluded_fstype (वर्ण अचर *fstype)
{
  यदि (fs_exclude_list == रिक्त || fstype == रिक्त)
    फर्काऊ false;
  लागि (अचर संरचना fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_next)
    यदि (streq (fstype, fsp->fs_name))
      फर्काऊ true;
  फर्काऊ false;
}

स्थिर माप_प्रकार
devlist_hash (शून्य अचर *x, माप_प्रकार table_size)
{
  संरचना devlist अचर *p = x;
  फर्काऊ (uintmax_t) {p->dev_अङ्क} % table_size;
}

स्थिर bool
devlist_compare (शून्य अचर *x, शून्य अचर *y)
{
  संरचना devlist अचर *a = x;
  संरचना devlist अचर *b = y;
  फर्काऊ a->dev_अङ्क == b->dev_अङ्क;
}

स्थिर संरचना devlist *
devlist_for_dev (dev_t dev)
{
  यदि (devlist_table == रिक्त)
    फर्काऊ रिक्त;
  संरचना devlist dev_entry;
  dev_entry.dev_अङ्क = dev;

  संरचना devlist *found = hash_lookup (devlist_table, &dev_entry);
  यदि (found == रिक्त)
    फर्काऊ रिक्त;

  /* Return the last devlist entry we have seen with this dev_num */
  फर्काऊ found->seen_last;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

स्थिर शून्य
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  संरचना devlist *device_list = रिक्त;
  पूर्णाङ्क mount_list_size = 0;

  लागि (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    mount_list_size++;

  devlist_table = hash_initialize (mount_list_size, रिक्त,
                                   devlist_hash, devlist_compare, रिक्त);
  यदि (devlist_table == रिक्त)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  लागि (संरचना mount_entry *me = mount_list; me;)
    {
      संरचना stat buf;
      संरचना mount_entry *discard_me = रिक्त;
      संरचना devlist *last_seen_dev = रिक्त, *seen_dev = रिक्त;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      यदि ((me->me_remote && show_local_fs)
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

          लागि (; seen_dev && ! discard_me; seen_dev = seen_dev->next_same_dev)
            {
              bool target_nearer_जरा = म_माप (seen_dev->me->me_mountdir)
                                        > म_माप (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_below_जरा = seen_dev->me->me_mntroot != रिक्त
                                       && me->me_mntroot != रिक्त
                                       && (म_माप (seen_dev->me->me_mntroot)
                                           < म_माप (me->me_mntroot));
              यदि (! print_grand_total
                  && me->me_remote && seen_dev->me->me_remote
                  && ! streq (seen_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              अन्यथा यदि ((म_अक्षर (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! म_अक्षर (seen_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_जरा && ! source_below_जरा)
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

      यदि (discard_me)
        {
          me = me->me_next;
          यदि (! devices_only)
            free_mount_entry (discard_me);
        }
      अन्यथा
        {
          /* Add the device number to the device_table.  */
          संरचना devlist *devlist = xmalloc (परिमाण *devlist);
          devlist->me = me;
          devlist->dev_अङ्क = buf.st_dev;
          devlist->next_same_dev = last_seen_dev;
          devlist->next = device_list;
          device_list = devlist;

          संरचना devlist *hash_entry = hash_घुसाउ (devlist_table, devlist);
          यदि (hash_entry == रिक्त)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_entry->seen_last = devlist;

          me = me->me_next;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  यदि (! devices_only)
    {
      mount_list = रिक्त;
      जबसम्म (device_list)
        {
          /* Add the mount entry.  */
          संरचना mount_entry *me = device_list->me;
          me->me_next = mount_list;
          mount_list = me;
          संरचना devlist *next = device_list->next;
          मुक्त (device_list);
          device_list = next;
        }
      hash_free (devlist_table);
      devlist_table = रिक्त;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_PURE
स्थिर संरचना mount_entry अचर *
me_for_dev (dev_t dev)
{
  संरचना devlist *dl = devlist_for_dev (dev);
  यदि (dl)
        फर्काऊ dl->me;

  फर्काऊ रिक्त;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
स्थिर bool
known_मान (uintmax_t n)
{
  फर्काऊ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

स्थिर वर्ण अचर *
df_readable (bool negative, uintmax_t n, वर्ण *buf,
             uintmax_t आगत_units, uintmax_t output_units)
{
  यदि (! known_मान (n) && !negative)
    फर्काऊ "-";
  अन्यथा
    {
      वर्ण *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, आगत_units, output_units);
      यदि (negative)
        *--p = '-';
      फर्काऊ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
स्थिर शून्य
add_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  यदि (*dest_neg == src_neg)
    {
      *dest += src;
      फर्काऊ;
    }

  यदि (*dest_neg)
    *dest = -*dest;

  यदि (src_neg)
    src = -src;

  यदि (src < *dest)
    *dest -= src;
  अन्यथा
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  यदि (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_PURE
स्थिर bool
has_uuid_suffix (वर्ण अचर *s)
{
  माप_प्रकार len = म_माप (s);
  फर्काऊ (36 < len
          && म_अखोज (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
स्थिर शून्य
get_field_मानहरू (संरचना field_मानहरू_t *bv,
                  संरचना field_मानहरू_t *iv,
                  अचर संरचना fs_usage *fsu)
{
  /* Inode values.  */
  iv->आगत_units = iv->output_units = 1;
  iv->total = fsu->fsu_files;
  iv->available = iv->available_to_जरा = fsu->fsu_ffree;
  iv->negate_available = false;

  iv->used = UINTMAX_MAX;
  iv->negate_used = false;
  यदि (known_मान (iv->total) && known_मान (iv->available_to_जरा))
    {
      iv->used = iv->total - iv->available_to_जरा;
      iv->negate_used = (iv->total < iv->available_to_जरा);
    }

  /* Block values.  */
  bv->आगत_units = fsu->fsu_blocksize;
  bv->output_units = output_block_size;
  bv->total = fsu->fsu_blocks;
  bv->available = fsu->fsu_bavail;
  bv->available_to_जरा = fsu->fsu_bfree;
  bv->negate_available = (fsu->fsu_bavail_शीर्ष_bit_set
                         && known_मान (fsu->fsu_bavail));

  bv->used = UINTMAX_MAX;
  bv->negate_used = false;
  यदि (known_मान (bv->total) && known_मान (bv->available_to_जरा))
    {
      bv->used = bv->total - bv->available_to_जरा;
      bv->negate_used = (bv->total < bv->available_to_जरा);
    }
}

/* Add block and inode values to grand total.  */
स्थिर शून्य
add_to_grand_total (संरचना field_मानहरू_t *bv, संरचना field_मानहरू_t *iv)
{
  यदि (known_मान (iv->total))
    grand_fsu.fsu_files += iv->total;
  यदि (known_मान (iv->available))
    grand_fsu.fsu_ffree += iv->available;

  यदि (known_मान (bv->total))
    grand_fsu.fsu_blocks += bv->आगत_units * bv->total;
  यदि (known_मान (bv->available_to_जरा))
    grand_fsu.fsu_bfree += bv->आगत_units * bv->available_to_जरा;
  यदि (known_मान (bv->available))
    add_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_शीर्ष_bit_set,
                            bv->आगत_units * bv->available,
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

स्थिर शून्य
get_dev (वर्ण अचर *device, वर्ण अचर *mount_point, वर्ण अचर *file,
         वर्ण अचर *stat_file, वर्ण अचर *fstype,
         bool me_dummy, bool me_remote,
         अचर संरचना fs_usage *force_fsu,
         bool process_all)
{
  यदि (me_remote && show_local_fs)
    फर्काऊ;

  यदि (me_dummy && !show_all_fs && !show_listed_fs)
    फर्काऊ;

  यदि (!selected_fstype (fstype) || excluded_fstype (fstype))
    फर्काऊ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  यदि (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    फर्काऊ;

  यदि (!stat_file)
    stat_file = mount_point;

  संरचना fs_usage fsu;
  यदि (force_fsu)
    fsu = *force_fsu;
  अन्यथा यदि (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      यदि (process_all && (त्रुटि_सं == EACCES || त्रुटि_सं == ENOENT))
        {
          यदि (! show_all_fs)
            फर्काऊ;

          fstype = "-";
          fsu.fsu_bavail_शीर्ष_bit_set = false;
          fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      अन्यथा
        {
          error (0, त्रुटि_सं, "%s", quotef (stat_file));
          exit_status = निकास_त्रुटि;
          फर्काऊ;
        }
    }
  अन्यथा यदि (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      संरचना stat sb;
      यदि (stat (stat_file, &sb) == 0)
        {
          संरचना mount_entry अचर * dev_me = me_for_dev (sb.st_dev);
          यदि (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_शीर्ष_bit_set = false;
              fsu.fsu_blocksize = fsu.fsu_blocks = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  यदि (fsu.fsu_blocks == 0 && !show_all_fs && !show_listed_fs)
    फर्काऊ;

  यदि (! force_fsu)
    file_systems_processed = true;

  alloc_table_row ();

  यदि (! device)
    device = "-";		/* unknown */

  यदि (! file)
    file = "-";			/* unspecified */

  वर्ण *dev_name = xstrdup (device);
  वर्ण *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  यदि (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_mode (dev_name, CAN_EXISTING)))
    {
      मुक्त (dev_name);
      dev_name = resolved_dev;
    }

  यदि (! fstype)
    fstype = "-";		/* unknown */

  संरचना field_मानहरू_t block_मानहरू;
  संरचना field_मानहरू_t inode_मानहरू;
  get_field_मानहरू (&block_मानहरू, &inode_मानहरू, &fsu);

  /* Add to grand total unless processing grand total line.  */
  यदि (print_grand_total && ! force_fsu)
    add_to_grand_total (&block_मानहरू, &inode_मानहरू);

  लागि (idx_t col = 0; col < table.ncolumns; col++)
    {
      वर्ण buf[LONGEST_HUMAN_READABLE + 2];
      वर्ण *cell;

      संरचना field_मानहरू_t *v;
      विकल्प (table.columns[col]->field_type)
        {
        पक्ष BLOCK_FLD:
          v = &block_मानहरू;
          रोक;
        पक्ष INODE_FLD:
          v = &inode_मानहरू;
          रोक;
        पक्ष OTHER_FLD:
          v = रिक्त;
          रोक;
        स्वभाव:
          affirm (!"bad field_type");
        }

      विकल्प (table.columns[col]->field)
        {
        पक्ष SOURCE_FIELD:
          cell = xstrdup (dev_name);
          रोक;

        पक्ष FSTYPE_FIELD:
          cell = xstrdup (fstype);
          रोक;

        पक्ष SIZE_FIELD:
        पक्ष ITOTAL_FIELD:
          cell = xstrdup (df_readable (false, v->total, buf,
                                       v->आगत_units, v->output_units));
          रोक;

        पक्ष USED_FIELD:
        पक्ष IUSED_FIELD:
          cell = xstrdup (df_readable (v->negate_used, v->used, buf,
                                       v->आगत_units, v->output_units));
          रोक;

        पक्ष AVAIL_FIELD:
        पक्ष IAVAIL_FIELD:
          cell = xstrdup (df_readable (v->negate_available, v->available, buf,
                                       v->आगत_units, v->output_units));
          रोक;

        पक्ष PCENT_FIELD:
        पक्ष IPCENT_FIELD:
          {
            द्विगुण pct = -1;
            यदि (! known_मान (v->used) || ! known_मान (v->available))
              ;
            अन्यथा यदि (!v->negate_used
                     && v->used <= UINTMAX_MAX / 100
                     && v->used + v->available != 0
                     && (v->used + v->available < v->used)
                     == v->negate_available)
              {
                uintmax_t u100 = v->used * 100;
                uintmax_t nonroot_total = v->used + v->available;
                pct = u100 / nonroot_total + (u100 % nonroot_total != 0);
              }
            अन्यथा
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                द्विगुण u = v->negate_used ? - (द्विगुण) - v->used : v->used;
                द्विगुण a = v->negate_available
                           ? - (द्विगुण) - v->available : v->available;
                द्विगुण nonroot_total = u + a;
                यदि (nonroot_total)
                  {
                    दीर्घ पूर्णाङ्क lipct = pct = u * 100 / nonroot_total;
                    द्विगुण ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    यदि (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            रोक;
          }

        पक्ष FILE_FIELD:
          cell = xstrdup (file);
          रोक;

        पक्ष TARGET_FIELD:
#ifdef HIDE_AUTOMOUNT_PREFIX
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          यदि (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          अन्यथा यदि (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#endif
          cell = xstrdup (mount_point);
          रोक;

        पक्ष INVALID_FIELD:
        स्वभाव:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      पूर्णाङ्क cell_width = mbswidth (cell, MBSWIDTH_FLAGS);
      table.columns[col]->width = MAX (table.columns[col]->width, cell_width);
      table.rows[table.nrows - 1][col] = cell;
    }
  मुक्त (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
स्थिर वर्ण *
last_device_for_mount (वर्ण अचर *mount)
{
  संरचना mount_entry अचर *le = रिक्त;

  लागि (संरचना mount_entry अचर *me = mount_list; me; me = me->me_next)
    {
      यदि (streq (me->me_mountdir, mount))
        le = me;
    }

  यदि (le)
    {
      वर्ण *devname = le->me_devname;
      वर्ण *canon_dev = canonicalize_file_name (devname);
      यदि (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        फर्काऊ canon_dev;
      मुक्त (canon_dev);
      फर्काऊ xstrdup (le->me_devname);
    }
  अन्यथा
    फर्काऊ रिक्त;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
स्थिर bool
get_device (वर्ण अचर *device)
{
  संरचना mount_entry अचर *best_match = रिक्त;
  bool best_match_accessible = false;
  bool eclipsed_device = false;
  वर्ण अचर *file = device;

  वर्ण *resolved = canonicalize_file_name (device);
  यदि (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  माप_प्रकार best_match_len = SIZE_MAX;
  लागि (संरचना mount_entry अचर *me = mount_list; me; me = me->me_next)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      वर्ण *devname = me->me_devname;
      वर्ण *canon_dev = canonicalize_file_name (me->me_devname);
      यदि (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      यदि (streq (device, devname))
        {
          वर्ण *last_device = last_device_for_mount (me->me_mountdir);
          eclipsed_device = last_device && ! streq (last_device, devname);
          माप_प्रकार len = म_माप (me->me_mountdir);

          यदि (! eclipsed_device
              && (! best_match_accessible || len < best_match_len))
            {
              संरचना stat device_stats;
              bool this_match_accessible = false;

              यदि (stat (me->me_mountdir, &device_stats) == 0)
                best_match_accessible = this_match_accessible = true;

              यदि (this_match_accessible
                  || (! best_match_accessible && len < best_match_len))
                {
                  best_match = me;
                  यदि (len == 1) /* Traditional root.  */
                    {
                      मुक्त (last_device);
                      मुक्त (canon_dev);
                      रोक;
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

  यदि (best_match)
    {
      get_dev (best_match->me_devname, best_match->me_mountdir, file, रिक्त,
               best_match->me_type, best_match->me_dummy,
               best_match->me_remote, रिक्त, false);
      फर्काऊ true;
    }
  अन्यथा यदि (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_status = निकास_त्रुटि;
      फर्काऊ true;
    }

  फर्काऊ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
स्थिर शून्य
get_point (वर्ण अचर *point, अचर संरचना stat *statp)
{
  संरचना stat device_stats;
  संरचना mount_entry अचर *best_match = रिक्त;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  वर्ण *resolved = canonicalize_file_name (point);
  यदि (resolved && resolved[0] == '/')
    {
      माप_प्रकार resolved_len = म_माप (resolved);
      माप_प्रकार best_match_len = 0;

      लागि (संरचना mount_entry *me = mount_list; me; me = me->me_next)
        {
          यदि (!streq (me->me_type, "lofs")
              && (!best_match || best_match->me_dummy || !me->me_dummy))
            {
              माप_प्रकार len = म_माप (me->me_mountdir);
              यदि (best_match_len <= len && len <= resolved_len
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
  यदि (best_match
      && (stat (best_match->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_match = रिक्त;

  यदि (! best_match)
    लागि (संरचना mount_entry *me = mount_list; me; me = me->me_next)
      {
        यदि (me->me_dev == (dev_t) -1)
          {
            यदि (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            अन्यथा
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                यदि (त्रुटि_सं == EIO)
                  {
                    error (0, त्रुटि_सं, "%s", quotef (me->me_mountdir));
                    exit_status = निकास_त्रुटि;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        यदि (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_match || best_match->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            यदि (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            अन्यथा
              best_match = me;
          }
      }

  यदि (best_match)
    get_dev (best_match->me_devname, best_match->me_mountdir, point, point,
             best_match->me_type, best_match->me_dummy, best_match->me_remote,
             रिक्त, false);
  अन्यथा
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      वर्ण *mp = find_mount_point (point, statp);
      यदि (mp)
        {
          get_dev (रिक्त, mp, point, रिक्त, रिक्त,
                   false, false, रिक्त, false);
          मुक्त (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

स्थिर शून्य
get_entry (वर्ण अचर *name, संरचना stat अचर *statp)
{
  यदि ((S_ISBLK (statp->st_mode) || S_ISCHR (statp->st_mode))
      && get_device (name))
    फर्काऊ;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

स्थिर शून्य
get_all_entries (शून्य)
{
  filter_mount_list (show_all_fs);

  लागि (संरचना mount_entry *me = mount_list; me; me = me->me_next)
    get_dev (me->me_devname, me->me_mountdir, रिक्त, रिक्त, me->me_type,
             me->me_dummy, me->me_remote, रिक्त, true);
}

/* Add FSTYPE to the list of file system types to display.  */

स्थिर शून्य
add_fs_type (वर्ण अचर *fstype)
{
  संरचना fs_type_list *fsp;

  fsp = xmalloc (परिमाण *fsp);
  fsp->fs_name = (वर्ण *) fstype;
  fsp->fs_next = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

स्थिर शून्य
add_excluded_fs_type (वर्ण अचर *fstype)
{
  संरचना fs_type_list *fsp;

  fsp = xmalloc (परिमाण *fsp);
  fsp->fs_name = (वर्ण *) fstype;
  fsp->fs_next = fs_exclude_list;
  fs_exclude_list = fsp;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]...\n"), program_name);
      ख_माला_दो (_("\
Show information about the file प्रणाली on which each खाता resides,\n\
or all file systems by स्वभाव.\n\
"), मानक_निकास);

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
         गर not invoke sync before getting usage info (स्वभाव)\n\
"));
      oputs (_("\
      --output[=FIELD_LIST]\n\
         use the output format defined by FIELD_LIST,\n\
         or print all fields यदि FIELD_LIST is omitted\n\
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
      emit_size_note ();
      ख_माला_दो (_("\n\
FIELD_LIST is a comma-separated list of columns to be included.  Valid\n\
field names are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info page).\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  संरचना stat *stats = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  वर्ण अचर *सन्देश_mut_excl = _("options %s and %s are mutually exclusive");

  जबसम्म (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      यदि (c == -1)
        रोक;

      विकल्प (c)
        {
        पक्ष 'a':
          show_all_fs = true;
          रोक;
        पक्ष 'B':
          {
            गणना strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            यदि (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          रोक;
        पक्ष 'i':
          यदि (header_mode == OUTPUT_MODE)
            {
              error (0, 0, सन्देश_mut_excl, "-i", "--output");
              usage (निकास_त्रुटि);
            }
          header_mode = INODES_MODE;
          रोक;
        पक्ष 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          रोक;
        पक्ष 'H':
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          रोक;
        पक्ष 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          रोक;
        पक्ष 'l':
          show_local_fs = true;
          रोक;
        पक्ष 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          रोक;
        पक्ष 'T':
          यदि (header_mode == OUTPUT_MODE)
            {
              error (0, 0, सन्देश_mut_excl, "-T", "--output");
              usage (निकास_त्रुटि);
            }
          print_type = true;
          रोक;
        पक्ष 'P':
          यदि (header_mode == OUTPUT_MODE)
            {
              error (0, 0, सन्देश_mut_excl, "-P", "--output");
              usage (निकास_त्रुटि);
            }
          posix_format = true;
          रोक;
        पक्ष SYNC_OPTION:
          require_sync = true;
          रोक;
        पक्ष NO_SYNC_OPTION:
          require_sync = false;
          रोक;

        पक्ष 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        पक्ष 't':
          add_fs_type (optarg);
          रोक;

        पक्ष 'v':		/* For SysV compatibility.  */
          /* ignore */
          रोक;
        पक्ष 'x':
          add_excluded_fs_type (optarg);
          रोक;

        पक्ष OUTPUT_OPTION:
          यदि (header_mode == INODES_MODE)
            {
              error (0, 0, सन्देश_mut_excl, "-i", "--output");
              usage (निकास_त्रुटि);
            }
          यदि (posix_format && header_mode == DEFAULT_MODE)
            {
              error (0, 0, सन्देश_mut_excl, "-P", "--output");
              usage (निकास_त्रुटि);
            }
          यदि (print_type)
            {
              error (0, 0, सन्देश_mut_excl, "-T", "--output");
              usage (निकास_त्रुटि);
            }
          header_mode = OUTPUT_MODE;
          यदि (optarg)
            decode_output_arg (optarg);
          रोक;

        पक्ष TOTAL_OPTION:
          print_grand_total = true;
          रोक;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (human_output_opts == -1)
    {
      यदि (posix_format)
        {
          human_output_opts = 0;
          output_block_size = (दो_पर्या ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      अन्यथा
        human_options (दो_पर्या ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_block_size);
    }

  यदि (header_mode == INODES_MODE || header_mode == OUTPUT_MODE)
    ;
  अन्यथा यदि (human_output_opts & human_autoscale)
    header_mode = HUMAN_MODE;
  अन्यथा यदि (posix_format)
    header_mode = POSIX_MODE;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool match = false;
    लागि (संरचना fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_next)
      {
        लागि (संरचना fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_next)
          {
            यदि (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                match = true;
                रोक;
              }
          }
      }
    यदि (match)
      फर्काऊ निकास_त्रुटि;
  }

  यदि (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, परिमाण *stats);
      लागि (पूर्णाङ्क i = optind; i < argc; ++i)
        {
          पूर्णाङ्क err = automount_stat_err (argv[i], &stats[i - optind]);
          यदि (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_status = निकास_त्रुटि;
              argv[i] = रिक्त;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != रिक्त
                            || fs_exclude_list != रिक्त
                            || print_type
                            || field_data[FSTYPE_FIELD].used
                            || show_local_fs));

  यदि (mount_list == रिक्त)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      पूर्णाङ्क status = 0;
      यदि ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != रिक्त
               || fs_exclude_list != रिक्त))
        {
          status = निकास_त्रुटि;
        }
      वर्ण अचर *warning = (status == 0 ? _("Warning: ") : "");
      error (status, त्रुटि_सं, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  यदि (require_sync)
    sync ();

  get_field_list ();
  get_header ();

  यदि (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      लागि (पूर्णाङ्क i = optind; i < argc; ++i)
        यदि (argv[i])
          get_entry (argv[i], &stats[i - optind]);
    }
  अन्यथा
    get_all_entries ();

  यदि (file_systems_processed)
    {
      यदि (print_grand_total)
        get_dev ("total",
                 (field_data[SOURCE_FIELD].used ? "-" : "total"),
                 रिक्त, रिक्त, रिक्त, false, false, &grand_fsu, false);

      print_table ();
    }
  अन्यथा
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      यदि (exit_status == निकास_सफल)
        error (निकास_त्रुटि, 0, _("no file systems processed"));
    }

  main_exit (exit_status);
}
