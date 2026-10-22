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
#परिभाषय क्रमादेशः_NAME "df"

#परिभाषय AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

संरचना devlist
{
  dev_t dev_अङ्कः;
  संरचना mount_प्रविष्टिः *me;
  संरचना devlist *अग्र;
  संरचना devlist *अग्र_same_dev;
  संरचना devlist *आलोकितम्_चरमः; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
स्थिर Hash_सारणी *devlist_सारणी;

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
स्थिर uintmax_t output_खण्डम्_आकार;

/* True if a file system has been processed for output.  */
स्थिर bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
स्थिर bool require_sync;

/* Desired exit status.  */
स्थिर पूर्णाङ्क exit_स्थितिः;

/* A file system type to display.  */

संरचना fs_type_list
{
  वर्ण *fs_name;
  संरचना fs_type_list *fs_अग्र;
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
स्थिर संरचना mount_प्रविष्टिः *mount_list;

/* If true, print file system type as well.  */
स्थिर bool print_type;

/* If true, print a grand total at the end.  */
स्थिर bool print_grand_योग;

/* Grand total data.  */
स्थिर संरचना fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
स्थिर गणना
{
  DEFAULT_बहुलकम्,
  INODES_बहुलकम्,
  HUMAN_बहुलकम्,
  POSIX_बहुलकम्,
  OUTPUT_बहुलकम्
} शीर्षकम्_बहुलकम् = DEFAULT_बहुलकम्;

/* Displayable fields.  */
प्रकारनिर्देश गणना
{
  SOURCE_क्षेत्रम्, /* file system */
  FSTYPE_क्षेत्रम्, /* FS type */
  आकार_क्षेत्रम्,   /* FS size */
  प्रयुक्तम्_क्षेत्रम्,   /* FS size used  */
  AVAIL_क्षेत्रम्,  /* FS size available */
  PCENT_क्षेत्रम्,  /* percent used */
  ITOTAL_क्षेत्रम्, /* inode total */
  IUSED_क्षेत्रम्,  /* inodes used */
  IAVAIL_क्षेत्रम्, /* inodes available */
  IPCENT_क्षेत्रम्, /* inodes used in percent */
  TARGET_क्षेत्रम्, /* mount point */
  FILE_क्षेत्रम्,   /* specified file name */
  INVALID_क्षेत्रम् /* validation marker */
} display_क्षेत्रम्_t;

/* Flag if a field contains a block, an inode or another value.  */
प्रकारनिर्देश गणना
{
  खण्डम्_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} क्षेत्रम्_type_t;

/* Attributes of a display field.  */
संरचना क्षेत्रम्_दत्त_t
{
  display_क्षेत्रम्_t क्षेत्रम्;
  वर्ण ध्रुव *arg;
  क्षेत्रम्_type_t क्षेत्रम्_type;
  वर्ण ध्रुव *caption;/* NULL means use default header of this field.  */
  पूर्णाङ्क विस्तारः;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_दक्षिणः;   /* Whether to right-align columns, not left-align.  */
  bool प्रयुक्तम्;
};

/* Header strings, minimum width and alignment for the above fields.  */
स्थिर संरचना क्षेत्रम्_दत्त_t क्षेत्रम्_दत्त_2[] = {
  [SOURCE_क्षेत्रम्] = { SOURCE_क्षेत्रम्,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_क्षेत्रम्] = { FSTYPE_क्षेत्रम्,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [आकार_क्षेत्रम्] = { आकार_क्षेत्रम्,
    "size",   खण्डम्_FLD, N_("blocks"),      5, true, false },

  [प्रयुक्तम्_क्षेत्रम्] = { प्रयुक्तम्_क्षेत्रम्,
    "used",   खण्डम्_FLD, N_("Used"),        5, true, false },

  [AVAIL_क्षेत्रम्] = { AVAIL_क्षेत्रम्,
    "avail",  खण्डम्_FLD, N_("Available"),   5, true, false },

  [PCENT_क्षेत्रम्] = { PCENT_क्षेत्रम्,
    "pcent",  खण्डम्_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_क्षेत्रम्] = { ITOTAL_क्षेत्रम्,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_क्षेत्रम्] = { IUSED_क्षेत्रम्,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_क्षेत्रम्] = { IAVAIL_क्षेत्रम्,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_क्षेत्रम्] = { IPCENT_क्षेत्रम्,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [TARGET_क्षेत्रम्] = { TARGET_क्षेत्रम्,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_क्षेत्रम्] = { FILE_क्षेत्रम्,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

स्थिर वर्ण ध्रुव *all_तर्काः_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

स्थिर संरचना
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  वर्ण ***पङ्क्तिकाः COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  संरचना क्षेत्रम्_दत्त_t **columns COUNTED_BY (ncolumns);
} सारणी;

/* Field values.  */
संरचना क्षेत्रम्_मानानि_t
{
  uintmax_t निवेशः_units;
  uintmax_t output_units;
  uintmax_t योग_2;
  uintmax_t उपलब्धम्;
  bool negate_उपलब्धम्;
  uintmax_t उपलब्धम्_गन्तव्यम्_मूलम्;
  uintmax_t प्रयुक्तम्;
  bool negate_प्रयुक्तम्;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  योग_OPTION,
  OUTPUT_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"all", no_तर्कः, रिक्त, 'a'},
  {"block-size", required_तर्कः, रिक्त, 'B'},
  {"inodes", no_तर्कः, रिक्त, 'i'},
  {"human-readable", no_तर्कः, रिक्त, 'h'},
  {"si", no_तर्कः, रिक्त, 'H'},
  {"local", no_तर्कः, रिक्त, 'l'},
  {"output", optional_तर्कः, रिक्त, OUTPUT_OPTION},
  {"portability", no_तर्कः, रिक्त, 'P'},
  {"print-type", no_तर्कः, रिक्त, 'T'},
  {"sync", no_तर्कः, रिक्त, SYNC_OPTION},
  {"no-sync", no_तर्कः, रिक्त, NO_SYNC_OPTION},
  {"total", no_तर्कः, रिक्त, योग_OPTION},
  {"type", required_तर्कः, रिक्त, 't'},
  {"exclude-type", required_तर्कः, रिक्त, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

स्थिर पूर्णाङ्क
automount_stat_err (वर्ण ध्रुव *file, संरचना stat *st)
{
  पूर्णाङ्क fd = उद्घाटय (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  यदि (fd < 0)
    {
      यदि (errno == ENOENT || errno == ENOTDIR)
        प्रत्यागम errno;
      प्रत्यागम stat (file, st) == 0 ? 0 : errno;
    }
  अन्यथा
    {
      पूर्णाङ्क err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      प्रत्यागम err;
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
  यावत् (*p)
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
  वर्ण *srcend = cell + वाक्यदैर्घ्यम् (cell);
  वर्ण *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  प्रति (वर्ण *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      यदि (ok)
        ok = !c32iscntrl (wc);
      अन्यथा
        n = 1;

      यदि (ok)
        {
          memmove (dst, src, n);
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
  स्थिर पूर्णाङ्क tty_बहिर्गमनम् = -1;
  यदि (tty_बहिर्गमनम् < 0)
    tty_बहिर्गमनम् = isatty (STDOUT_FILENO);

  (tty_बहिर्गमनम् ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

स्थिर शून्य
alloc_सारणी_row (शून्य)
{
  यदि (सारणी.nrows == सारणी.nrows_alloc)
    सारणी.पङ्क्तिकाः = xpalloc (सारणी.पङ्क्तिकाः, &सारणी.nrows_alloc, 1, -1,
                          परिमाण *सारणी.पङ्क्तिकाः);
  idx_t ध्रुव row_idx = सारणी.nrows++;
  सारणी.पङ्क्तिकाः[row_idx] = xinmalloc (सारणी.ncolumns, परिमाण *सारणी.पङ्क्तिकाः[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

स्थिर शून्य
print_सारणी (शून्य)
{
  प्रति (idx_t row = 0; row < सारणी.nrows; row++)
    {
      प्रति (idx_t col = 0; col < सारणी.ncolumns; col++)
        {
          वर्ण *cell = सारणी.पङ्क्तिकाः[row][col];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          यदि (col != 0)
            अक्षरं_लिख (' ');

          पूर्णाङ्क विस्तारः = mbswidth (cell, MBSWIDTH_FLAGS);
          पूर्णाङ्क fill = विस्तारः < 0 ? 0 : सारणी.columns[col]->विस्तारः - विस्तारः;
          यदि (सारणी.columns[col]->align_दक्षिणः)
            प्रति (; 0 < fill; fill--)
              अक्षरं_लिख (' ');
          fputs (cell, stdout);
          यदि (col + 1 < सारणी.ncolumns)
            प्रति (; 0 < fill; fill--)
              अक्षरं_लिख (' ');
        }
      अक्षरं_लिख ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

स्थिर शून्य
alloc_क्षेत्रम् (पूर्णाङ्क f, वर्ण ध्रुव *c)
{
  यदि (सारणी.ncolumns == सारणी.ncolumns_alloc)
    सारणी.columns = xpalloc (सारणी.columns, &सारणी.ncolumns_alloc, 1, -1,
                             परिमाण *सारणी.columns);
  idx_t ध्रुव column_idx = सारणी.ncolumns++;
  सारणी.columns[column_idx] = &क्षेत्रम्_दत्त_2[f];
  यदि (c != रिक्त)
    क्षेत्रम्_दत्त_2[f].caption = c;

  affirm (!क्षेत्रम्_दत्त_2[f].प्रयुक्तम्);

  /* Mark field as used.  */
  क्षेत्रम्_दत्त_2[f].प्रयुक्तम् = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
स्थिर शून्य
decode_output_arg (वर्ण ध्रुव *arg)
{
  वर्ण *arg_writable = xstrdup (arg);
  वर्ण *s = arg_writable;
  कुरु
    {
      /* find next comma */
      वर्ण *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      यदि (comma)
        *comma++ = 0;

      /* process S.  */
      display_क्षेत्रम्_t क्षेत्रम् = INVALID_क्षेत्रम्;
      प्रति (idx_t i = 0; i < countof (क्षेत्रम्_दत्त_2); i++)
        {
          यदि (streq (क्षेत्रम्_दत्त_2[i].arg, s))
            {
              क्षेत्रम् = i;
              भङ्ग;
            }
        }
      यदि (क्षेत्रम् == INVALID_क्षेत्रम्)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      यदि (क्षेत्रम्_दत्त_2[क्षेत्रम्].प्रयुक्तम्)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (क्षेत्रम्_दत्त_2[क्षेत्रम्].arg));
          usage (EXIT_FAILURE);
        }

      विकल्प (क्षेत्रम्)
        {
        पक्ष SOURCE_क्षेत्रम्:
        पक्ष FSTYPE_क्षेत्रम्:
        पक्ष प्रयुक्तम्_क्षेत्रम्:
        पक्ष PCENT_क्षेत्रम्:
        पक्ष ITOTAL_क्षेत्रम्:
        पक्ष IUSED_क्षेत्रम्:
        पक्ष IAVAIL_क्षेत्रम्:
        पक्ष IPCENT_क्षेत्रम्:
        पक्ष TARGET_क्षेत्रम्:
        पक्ष FILE_क्षेत्रम्:
          alloc_क्षेत्रम् (क्षेत्रम्, रिक्त);
          भङ्ग;

        पक्ष आकार_क्षेत्रम्:
          alloc_क्षेत्रम् (क्षेत्रम्, N_("Size"));
          भङ्ग;

        पक्ष AVAIL_क्षेत्रम्:
          alloc_क्षेत्रम् (क्षेत्रम्, N_("Avail"));
          भङ्ग;

        पक्ष INVALID_क्षेत्रम्:
        स्वभाव:
          affirm (!"invalid field");
        }
      s = comma;
    }
  यावत् (s);

  मुक्त (arg_writable);
}

/* Get the appropriate columns for the mode.  */
स्थिर शून्य
get_क्षेत्रम्_list (शून्य)
{
  विकल्प (शीर्षकम्_बहुलकम्)
    {
    पक्ष DEFAULT_बहुलकम्:
      alloc_क्षेत्रम् (SOURCE_क्षेत्रम्, रिक्त);
      यदि (print_type)
        alloc_क्षेत्रम् (FSTYPE_क्षेत्रम्, रिक्त);
      alloc_क्षेत्रम् (आकार_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (प्रयुक्तम्_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (AVAIL_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (PCENT_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (TARGET_क्षेत्रम्, रिक्त);
      भङ्ग;

    पक्ष HUMAN_बहुलकम्:
      alloc_क्षेत्रम् (SOURCE_क्षेत्रम्, रिक्त);
      यदि (print_type)
        alloc_क्षेत्रम् (FSTYPE_क्षेत्रम्, रिक्त);

      alloc_क्षेत्रम् (आकार_क्षेत्रम्,   N_("Size"));
      alloc_क्षेत्रम् (प्रयुक्तम्_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (AVAIL_क्षेत्रम्,  N_("Avail"));
      alloc_क्षेत्रम् (PCENT_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (TARGET_क्षेत्रम्, रिक्त);
      भङ्ग;

    पक्ष INODES_बहुलकम्:
      alloc_क्षेत्रम् (SOURCE_क्षेत्रम्, रिक्त);
      यदि (print_type)
        alloc_क्षेत्रम् (FSTYPE_क्षेत्रम्, रिक्त);
      alloc_क्षेत्रम् (ITOTAL_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (IUSED_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (IAVAIL_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (IPCENT_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (TARGET_क्षेत्रम्,  रिक्त);
      भङ्ग;

    पक्ष POSIX_बहुलकम्:
      alloc_क्षेत्रम् (SOURCE_क्षेत्रम्, रिक्त);
      यदि (print_type)
        alloc_क्षेत्रम् (FSTYPE_क्षेत्रम्, रिक्त);
      alloc_क्षेत्रम् (आकार_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (प्रयुक्तम्_क्षेत्रम्,   रिक्त);
      alloc_क्षेत्रम् (AVAIL_क्षेत्रम्,  रिक्त);
      alloc_क्षेत्रम् (PCENT_क्षेत्रम्,  N_("Capacity"));
      alloc_क्षेत्रम् (TARGET_क्षेत्रम्, रिक्त);
      भङ्ग;

    पक्ष OUTPUT_बहुलकम्:
      यदि (!सारणी.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_तर्काः_string);
        }
      भङ्ग;

    स्वभाव:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

स्थिर शून्य
get_शीर्षकम् (शून्य)
{
  alloc_सारणी_row ();

  प्रति (idx_t col = 0; col < सारणी.ncolumns; col++)
    {
      वर्ण *cell;
      वर्ण ध्रुव *शीर्षकम् = _(सारणी.columns[col]->caption);

      यदि (सारणी.columns[col]->क्षेत्रम् == आकार_क्षेत्रम्
          && (शीर्षकम्_बहुलकम् == DEFAULT_बहुलकम्
              || (शीर्षकम्_बहुलकम् == OUTPUT_बहुलकम्
                  && !(human_output_opts & human_autoscale))))
        {
          वर्ण buf[LONGEST_HUMAN_READABLE + 1];

          पूर्णाङ्क opts = (human_suppress_point_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_गणः_digits | human_आधारः_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_खण्डम्_आकार;
          uintmax_t q1024 = output_खण्डम्_आकार;
          bool divisible_by_1000;
          bool divisible_by_1024;

          कुरु
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          यावत् (divisible_by_1000 & divisible_by_1024);

          यदि (divisible_by_1000 < divisible_by_1024)
            opts |= human_आधारः_1024;
          यदि (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_आधारः_1024;
          यदि (! (opts & human_आधारः_1024))
            opts |= human_B;

          वर्ण *अङ्कः = human_readable (output_खण्डम्_आकार, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          शीर्षकम् = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), अङ्कः, शीर्षकम्);
        }
      अन्यथा यदि (शीर्षकम्_बहुलकम् == POSIX_बहुलकम्
               && सारणी.columns[col]->क्षेत्रम् == आकार_क्षेत्रम्)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_खण्डम्_आकार, शीर्षकम्);
        }
      अन्यथा
        cell = xstrdup (शीर्षकम्);

      replace_problematic_chars (cell);

      सारणी.पङ्क्तिकाः[सारणी.nrows - 1][col] = cell;

      पूर्णाङ्क cell_विस्तारः = mbswidth (cell, MBSWIDTH_FLAGS);
      सारणी.columns[col]->विस्तारः = MAX (सारणी.columns[col]->विस्तारः, cell_विस्तारः);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
selected_fstype (वर्ण ध्रुव *fstype)
{
  यदि (fs_select_list == रिक्त || fstype == रिक्त)
    प्रत्यागम true;
  प्रति (ध्रुव संरचना fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_अग्र)
    यदि (streq (fstype, fsp->fs_name))
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_विशुद्धम्
स्थिर bool
excluded_fstype (वर्ण ध्रुव *fstype)
{
  यदि (fs_exclude_list == रिक्त || fstype == रिक्त)
    प्रत्यागम false;
  प्रति (ध्रुव संरचना fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_अग्र)
    यदि (streq (fstype, fsp->fs_name))
      प्रत्यागम true;
  प्रत्यागम false;
}

स्थिर size_t
devlist_hash (शून्य ध्रुव *x, size_t सारणी_आकार)
{
  संरचना devlist ध्रुव *p = x;
  प्रत्यागम (uintmax_t) {p->dev_अङ्कः} % सारणी_आकार;
}

स्थिर bool
devlist_compare (शून्य ध्रुव *x, शून्य ध्रुव *y)
{
  संरचना devlist ध्रुव *a = x;
  संरचना devlist ध्रुव *b = y;
  प्रत्यागम a->dev_अङ्कः == b->dev_अङ्कः;
}

स्थिर संरचना devlist *
devlist_for_dev (dev_t dev)
{
  यदि (devlist_सारणी == रिक्त)
    प्रत्यागम रिक्त;
  संरचना devlist dev_प्रविष्टिः;
  dev_प्रविष्टिः.dev_अङ्कः = dev;

  संरचना devlist *प्राप्तम् = hash_lookup (devlist_सारणी, &dev_प्रविष्टिः);
  यदि (प्राप्तम् == रिक्त)
    प्रत्यागम रिक्त;

  /* Return the last devlist entry we have seen with this dev_num */
  प्रत्यागम प्राप्तम्->आलोकितम्_चरमः;
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
  पूर्णाङ्क mount_list_आकार = 0;

  प्रति (संरचना mount_प्रविष्टिः *me = mount_list; me; me = me->me_अग्र)
    mount_list_आकार++;

  devlist_सारणी = hash_initialize (mount_list_आकार, रिक्त,
                                   devlist_hash, devlist_compare, रिक्त);
  यदि (devlist_सारणी == रिक्त)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  प्रति (संरचना mount_प्रविष्टिः *me = mount_list; me;)
    {
      संरचना stat buf;
      संरचना mount_प्रविष्टिः *discard_me = रिक्त;
      संरचना devlist *चरमः_आलोकितम्_dev = रिक्त, *आलोकितम्_dev = रिक्त;

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
          चरमः_आलोकितम्_dev = आलोकितम्_dev = devlist_for_dev (buf.st_dev);

          प्रति (; आलोकितम्_dev && ! discard_me; आलोकितम्_dev = आलोकितम्_dev->अग्र_same_dev)
            {
              bool target_nearer_मूलम् = वाक्यदैर्घ्यम् (आलोकितम्_dev->me->me_mountdir)
                                        > वाक्यदैर्घ्यम् (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool source_अधः_मूलम् = आलोकितम्_dev->me->me_mntroot != रिक्त
                                       && me->me_mntroot != रिक्त
                                       && (वाक्यदैर्घ्यम् (आलोकितम्_dev->me->me_mntroot)
                                           < वाक्यदैर्घ्यम् (me->me_mntroot));
              यदि (! print_grand_योग
                  && me->me_remote && आलोकितम्_dev->me->me_remote
                  && ! streq (आलोकितम्_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              अन्यथा यदि ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (आलोकितम्_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (target_nearer_मूलम् && ! source_अधः_मूलम्)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (आलोकितम्_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     आलोकितम्_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = आलोकितम्_dev->me;
                  आलोकितम्_dev->me = me;
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
          me = me->me_अग्र;
          यदि (! devices_only)
            free_mount_प्रविष्टिः (discard_me);
        }
      अन्यथा
        {
          /* Add the device number to the device_table.  */
          संरचना devlist *devlist = xmalloc (परिमाण *devlist);
          devlist->me = me;
          devlist->dev_अङ्कः = buf.st_dev;
          devlist->अग्र_same_dev = चरमः_आलोकितम्_dev;
          devlist->अग्र = device_list;
          device_list = devlist;

          संरचना devlist *hash_प्रविष्टिः = hash_निवेशय (devlist_सारणी, devlist);
          यदि (hash_प्रविष्टिः == रिक्त)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_प्रविष्टिः->आलोकितम्_चरमः = devlist;

          me = me->me_अग्र;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  यदि (! devices_only)
    {
      mount_list = रिक्त;
      यावत् (device_list)
        {
          /* Add the mount entry.  */
          संरचना mount_प्रविष्टिः *me = device_list->me;
          me->me_अग्र = mount_list;
          mount_list = me;
          संरचना devlist *अग्र = device_list->अग्र;
          मुक्त (device_list);
          device_list = अग्र;
        }
      hash_free (devlist_सारणी);
      devlist_सारणी = रिक्त;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_विशुद्धम्
स्थिर संरचना mount_प्रविष्टिः ध्रुव *
me_for_dev (dev_t dev)
{
  संरचना devlist *dl = devlist_for_dev (dev);
  यदि (dl)
        प्रत्यागम dl->me;

  प्रत्यागम रिक्त;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
स्थिर bool
ज्ञातम्_मानम् (uintmax_t n)
{
  प्रत्यागम n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

स्थिर वर्ण ध्रुव *
df_readable (bool negative, uintmax_t n, वर्ण *buf,
             uintmax_t निवेशः_units, uintmax_t output_units)
{
  यदि (! ज्ञातम्_मानम् (n) && !negative)
    प्रत्यागम "-";
  अन्यथा
    {
      वर्ण *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, निवेशः_units, output_units);
      यदि (negative)
        *--p = '-';
      प्रत्यागम p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
स्थिर शून्य
सङ्कलय_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  यदि (*dest_neg == src_neg)
    {
      *dest += src;
      प्रत्यागम;
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
ATTRIBUTE_विशुद्धम्
स्थिर bool
has_uuid_suffix (वर्ण ध्रुव *s)
{
  size_t len = वाक्यदैर्घ्यम् (s);
  प्रत्यागम (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
स्थिर शून्य
get_क्षेत्रम्_मानानि (संरचना क्षेत्रम्_मानानि_t *bv,
                  संरचना क्षेत्रम्_मानानि_t *iv,
                  ध्रुव संरचना fs_usage *fsu)
{
  /* Inode values.  */
  iv->निवेशः_units = iv->output_units = 1;
  iv->योग_2 = fsu->fsu_files;
  iv->उपलब्धम् = iv->उपलब्धम्_गन्तव्यम्_मूलम् = fsu->fsu_ffree;
  iv->negate_उपलब्धम् = false;

  iv->प्रयुक्तम् = UINTMAX_MAX;
  iv->negate_प्रयुक्तम् = false;
  यदि (ज्ञातम्_मानम् (iv->योग_2) && ज्ञातम्_मानम् (iv->उपलब्धम्_गन्तव्यम्_मूलम्))
    {
      iv->प्रयुक्तम् = iv->योग_2 - iv->उपलब्धम्_गन्तव्यम्_मूलम्;
      iv->negate_प्रयुक्तम् = (iv->योग_2 < iv->उपलब्धम्_गन्तव्यम्_मूलम्);
    }

  /* Block values.  */
  bv->निवेशः_units = fsu->fsu_blocksize;
  bv->output_units = output_खण्डम्_आकार;
  bv->योग_2 = fsu->fsu_खण्डानि;
  bv->उपलब्धम् = fsu->fsu_bavail;
  bv->उपलब्धम्_गन्तव्यम्_मूलम् = fsu->fsu_bfree;
  bv->negate_उपलब्धम् = (fsu->fsu_bavail_शिखरम्_bit_समुच्चयः
                         && ज्ञातम्_मानम् (fsu->fsu_bavail));

  bv->प्रयुक्तम् = UINTMAX_MAX;
  bv->negate_प्रयुक्तम् = false;
  यदि (ज्ञातम्_मानम् (bv->योग_2) && ज्ञातम्_मानम् (bv->उपलब्धम्_गन्तव्यम्_मूलम्))
    {
      bv->प्रयुक्तम् = bv->योग_2 - bv->उपलब्धम्_गन्तव्यम्_मूलम्;
      bv->negate_प्रयुक्तम् = (bv->योग_2 < bv->उपलब्धम्_गन्तव्यम्_मूलम्);
    }
}

/* Add block and inode values to grand total.  */
स्थिर शून्य
सङ्कलय_गन्तव्यम्_grand_योग (संरचना क्षेत्रम्_मानानि_t *bv, संरचना क्षेत्रम्_मानानि_t *iv)
{
  यदि (ज्ञातम्_मानम् (iv->योग_2))
    grand_fsu.fsu_files += iv->योग_2;
  यदि (ज्ञातम्_मानम् (iv->उपलब्धम्))
    grand_fsu.fsu_ffree += iv->उपलब्धम्;

  यदि (ज्ञातम्_मानम् (bv->योग_2))
    grand_fsu.fsu_खण्डानि += bv->निवेशः_units * bv->योग_2;
  यदि (ज्ञातम्_मानम् (bv->उपलब्धम्_गन्तव्यम्_मूलम्))
    grand_fsu.fsu_bfree += bv->निवेशः_units * bv->उपलब्धम्_गन्तव्यम्_मूलम्;
  यदि (ज्ञातम्_मानम् (bv->उपलब्धम्))
    सङ्कलय_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_शिखरम्_bit_समुच्चयः,
                            bv->निवेशः_units * bv->उपलब्धम्,
                            bv->negate_उपलब्धम्);
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
get_dev (वर्ण ध्रुव *device, वर्ण ध्रुव *mount_point, वर्ण ध्रुव *file,
         वर्ण ध्रुव *stat_file, वर्ण ध्रुव *fstype,
         bool me_dummy, bool me_remote,
         ध्रुव संरचना fs_usage *force_fsu,
         bool process_all)
{
  यदि (me_remote && show_local_fs)
    प्रत्यागम;

  यदि (me_dummy && !show_all_fs && !show_listed_fs)
    प्रत्यागम;

  यदि (!selected_fstype (fstype) || excluded_fstype (fstype))
    प्रत्यागम;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  यदि (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_point))
    प्रत्यागम;

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
      यदि (process_all && (errno == EACCES || errno == ENOENT))
        {
          यदि (! show_all_fs)
            प्रत्यागम;

          fstype = "-";
          fsu.fsu_bavail_शिखरम्_bit_समुच्चयः = false;
          fsu.fsu_blocksize = fsu.fsu_खण्डानि = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      अन्यथा
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_स्थितिः = EXIT_FAILURE;
          प्रत्यागम;
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
          संरचना mount_प्रविष्टिः ध्रुव * dev_me = me_for_dev (sb.st_dev);
          यदि (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_शिखरम्_bit_समुच्चयः = false;
              fsu.fsu_blocksize = fsu.fsu_खण्डानि = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  यदि (fsu.fsu_खण्डानि == 0 && !show_all_fs && !show_listed_fs)
    प्रत्यागम;

  यदि (! force_fsu)
    file_systems_processed = true;

  alloc_सारणी_row ();

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
      && (resolved_dev = canonicalize_filename_बहुलकम् (dev_name, CAN_EXISTING)))
    {
      मुक्त (dev_name);
      dev_name = resolved_dev;
    }

  यदि (! fstype)
    fstype = "-";		/* unknown */

  संरचना क्षेत्रम्_मानानि_t खण्डम्_मानानि;
  संरचना क्षेत्रम्_मानानि_t inode_मानानि;
  get_क्षेत्रम्_मानानि (&खण्डम्_मानानि, &inode_मानानि, &fsu);

  /* Add to grand total unless processing grand total line.  */
  यदि (print_grand_योग && ! force_fsu)
    सङ्कलय_गन्तव्यम्_grand_योग (&खण्डम्_मानानि, &inode_मानानि);

  प्रति (idx_t col = 0; col < सारणी.ncolumns; col++)
    {
      वर्ण buf[LONGEST_HUMAN_READABLE + 2];
      वर्ण *cell;

      संरचना क्षेत्रम्_मानानि_t *v;
      विकल्प (सारणी.columns[col]->क्षेत्रम्_type)
        {
        पक्ष खण्डम्_FLD:
          v = &खण्डम्_मानानि;
          भङ्ग;
        पक्ष INODE_FLD:
          v = &inode_मानानि;
          भङ्ग;
        पक्ष OTHER_FLD:
          v = रिक्त;
          भङ्ग;
        स्वभाव:
          affirm (!"bad field_type");
        }

      विकल्प (सारणी.columns[col]->क्षेत्रम्)
        {
        पक्ष SOURCE_क्षेत्रम्:
          cell = xstrdup (dev_name);
          भङ्ग;

        पक्ष FSTYPE_क्षेत्रम्:
          cell = xstrdup (fstype);
          भङ्ग;

        पक्ष आकार_क्षेत्रम्:
        पक्ष ITOTAL_क्षेत्रम्:
          cell = xstrdup (df_readable (false, v->योग_2, buf,
                                       v->निवेशः_units, v->output_units));
          भङ्ग;

        पक्ष प्रयुक्तम्_क्षेत्रम्:
        पक्ष IUSED_क्षेत्रम्:
          cell = xstrdup (df_readable (v->negate_प्रयुक्तम्, v->प्रयुक्तम्, buf,
                                       v->निवेशः_units, v->output_units));
          भङ्ग;

        पक्ष AVAIL_क्षेत्रम्:
        पक्ष IAVAIL_क्षेत्रम्:
          cell = xstrdup (df_readable (v->negate_उपलब्धम्, v->उपलब्धम्, buf,
                                       v->निवेशः_units, v->output_units));
          भङ्ग;

        पक्ष PCENT_क्षेत्रम्:
        पक्ष IPCENT_क्षेत्रम्:
          {
            द्विगुण pct = -1;
            यदि (! ज्ञातम्_मानम् (v->प्रयुक्तम्) || ! ज्ञातम्_मानम् (v->उपलब्धम्))
              ;
            अन्यथा यदि (!v->negate_प्रयुक्तम्
                     && v->प्रयुक्तम् <= UINTMAX_MAX / 100
                     && v->प्रयुक्तम् + v->उपलब्धम् != 0
                     && (v->प्रयुक्तम् + v->उपलब्धम् < v->प्रयुक्तम्)
                     == v->negate_उपलब्धम्)
              {
                uintmax_t u100 = v->प्रयुक्तम् * 100;
                uintmax_t nonroot_योग = v->प्रयुक्तम् + v->उपलब्धम्;
                pct = u100 / nonroot_योग + (u100 % nonroot_योग != 0);
              }
            अन्यथा
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                द्विगुण u = v->negate_प्रयुक्तम् ? - (द्विगुण) - v->प्रयुक्तम् : v->प्रयुक्तम्;
                द्विगुण a = v->negate_उपलब्धम्
                           ? - (द्विगुण) - v->उपलब्धम् : v->उपलब्धम्;
                द्विगुण nonroot_योग = u + a;
                यदि (nonroot_योग)
                  {
                    दीर्घ पूर्णाङ्क lipct = pct = u * 100 / nonroot_योग;
                    द्विगुण ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    यदि (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            भङ्ग;
          }

        पक्ष FILE_क्षेत्रम्:
          cell = xstrdup (file);
          भङ्ग;

        पक्ष TARGET_क्षेत्रम्:
#यदि_परिभाषितम् HIDE_AUTOMOUNT_उपसर्गः
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          यदि (STRNCMP_LIT (mount_point, "/auto/") == 0)
            mount_point += 5;
          अन्यथा यदि (STRNCMP_LIT (mount_point, "/tmp_mnt/") == 0)
            mount_point += 8;
#यदि_अन्तः
          cell = xstrdup (mount_point);
          भङ्ग;

        पक्ष INVALID_क्षेत्रम्:
        स्वभाव:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      पूर्णाङ्क cell_विस्तारः = mbswidth (cell, MBSWIDTH_FLAGS);
      सारणी.columns[col]->विस्तारः = MAX (सारणी.columns[col]->विस्तारः, cell_विस्तारः);
      सारणी.पङ्क्तिकाः[सारणी.nrows - 1][col] = cell;
    }
  मुक्त (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
स्थिर वर्ण *
चरमः_device_for_mount (वर्ण ध्रुव *mount)
{
  संरचना mount_प्रविष्टिः ध्रुव *le = रिक्त;

  प्रति (संरचना mount_प्रविष्टिः ध्रुव *me = mount_list; me; me = me->me_अग्र)
    {
      यदि (streq (me->me_mountdir, mount))
        le = me;
    }

  यदि (le)
    {
      वर्ण *devname = le->me_devname;
      वर्ण *canon_dev = canonicalize_file_name (devname);
      यदि (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        प्रत्यागम canon_dev;
      मुक्त (canon_dev);
      प्रत्यागम xstrdup (le->me_devname);
    }
  अन्यथा
    प्रत्यागम रिक्त;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
स्थिर bool
get_device (वर्ण ध्रुव *device)
{
  संरचना mount_प्रविष्टिः ध्रुव *best_मेलनम् = रिक्त;
  bool best_मेलनम्_accessible = false;
  bool eclipsed_device = false;
  वर्ण ध्रुव *file = device;

  वर्ण *resolved = canonicalize_file_name (device);
  यदि (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t best_मेलनम्_len = SIZE_MAX;
  प्रति (संरचना mount_प्रविष्टिः ध्रुव *me = mount_list; me; me = me->me_अग्र)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      वर्ण *devname = me->me_devname;
      वर्ण *canon_dev = canonicalize_file_name (me->me_devname);
      यदि (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      यदि (streq (device, devname))
        {
          वर्ण *चरमः_device = चरमः_device_for_mount (me->me_mountdir);
          eclipsed_device = चरमः_device && ! streq (चरमः_device, devname);
          size_t len = वाक्यदैर्घ्यम् (me->me_mountdir);

          यदि (! eclipsed_device
              && (! best_मेलनम्_accessible || len < best_मेलनम्_len))
            {
              संरचना stat device_stats;
              bool this_मेलनम्_accessible = false;

              यदि (stat (me->me_mountdir, &device_stats) == 0)
                best_मेलनम्_accessible = this_मेलनम्_accessible = true;

              यदि (this_मेलनम्_accessible
                  || (! best_मेलनम्_accessible && len < best_मेलनम्_len))
                {
                  best_मेलनम् = me;
                  यदि (len == 1) /* Traditional root.  */
                    {
                      मुक्त (चरमः_device);
                      मुक्त (canon_dev);
                      भङ्ग;
                    }
                  अन्यथा
                    best_मेलनम्_len = len;
                }
            }

          मुक्त (चरमः_device);
        }

      मुक्त (canon_dev);
    }

  मुक्त (resolved);

  यदि (best_मेलनम्)
    {
      get_dev (best_मेलनम्->me_devname, best_मेलनम्->me_mountdir, file, रिक्त,
               best_मेलनम्->me_type, best_मेलनम्->me_dummy,
               best_मेलनम्->me_remote, रिक्त, false);
      प्रत्यागम true;
    }
  अन्यथा यदि (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_स्थितिः = EXIT_FAILURE;
      प्रत्यागम true;
    }

  प्रत्यागम false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
स्थिर शून्य
get_point (वर्ण ध्रुव *point, ध्रुव संरचना stat *statp)
{
  संरचना stat device_stats;
  संरचना mount_प्रविष्टिः ध्रुव *best_मेलनम् = रिक्त;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  वर्ण *resolved = canonicalize_file_name (point);
  यदि (resolved && resolved[0] == '/')
    {
      size_t resolved_len = वाक्यदैर्घ्यम् (resolved);
      size_t best_मेलनम्_len = 0;

      प्रति (संरचना mount_प्रविष्टिः *me = mount_list; me; me = me->me_अग्र)
        {
          यदि (!streq (me->me_type, "lofs")
              && (!best_मेलनम् || best_मेलनम्->me_dummy || !me->me_dummy))
            {
              size_t len = वाक्यदैर्घ्यम् (me->me_mountdir);
              यदि (best_मेलनम्_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  best_मेलनम् = me;
                  best_मेलनम्_len = len;
                }
            }
        }
    }
  मुक्त (resolved);
  यदि (best_मेलनम्
      && (stat (best_मेलनम्->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    best_मेलनम् = रिक्त;

  यदि (! best_मेलनम्)
    प्रति (संरचना mount_प्रविष्टिः *me = mount_list; me; me = me->me_अग्र)
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
                यदि (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_स्थितिः = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        यदि (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!best_मेलनम् || best_मेलनम्->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            यदि (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            अन्यथा
              best_मेलनम् = me;
          }
      }

  यदि (best_मेलनम्)
    get_dev (best_मेलनम्->me_devname, best_मेलनम्->me_mountdir, point, point,
             best_मेलनम्->me_type, best_मेलनम्->me_dummy, best_मेलनम्->me_remote,
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
get_प्रविष्टिः (वर्ण ध्रुव *name, संरचना stat ध्रुव *statp)
{
  यदि ((S_ISBLK (statp->st_बहुलकम्) || S_ISCHR (statp->st_बहुलकम्))
      && get_device (name))
    प्रत्यागम;

  get_point (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

स्थिर शून्य
get_all_entries (शून्य)
{
  filter_mount_list (show_all_fs);

  प्रति (संरचना mount_प्रविष्टिः *me = mount_list; me; me = me->me_अग्र)
    get_dev (me->me_devname, me->me_mountdir, रिक्त, रिक्त, me->me_type,
             me->me_dummy, me->me_remote, रिक्त, true);
}

/* Add FSTYPE to the list of file system types to display.  */

स्थिर शून्य
सङ्कलय_fs_type (वर्ण ध्रुव *fstype)
{
  संरचना fs_type_list *fsp;

  fsp = xmalloc (परिमाण *fsp);
  fsp->fs_name = (वर्ण *) fstype;
  fsp->fs_अग्र = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

स्थिर शून्य
सङ्कलय_excluded_fs_type (वर्ण ध्रुव *fstype)
{
  संरचना fs_type_list *fsp;

  fsp = xmalloc (परिमाण *fsp);
  fsp->fs_name = (वर्ण *) fstype;
  fsp->fs_अग्र = fs_exclude_list;
  fs_exclude_list = fsp;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [FILE]...\n"), क्रमादेशः_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by स्वभाव.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --खण्डम्-आकार=आकार_2\n\
         scale sizes by आकार_2 before printing them; see आकार_2 format अधः;\n\
         E.g., '-BM' prints sizes अन्तर् units सम्बन्धि 1,048,576 अष्टकानि\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes अन्तर् powers सम्बन्धि 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes अन्तर् powers सम्बन्धि 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead सम्बन्धि खण्डम् usage\n\
"));
      oputs (_("\
  -k\n\
         like --खण्डम्-आकार=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         सीमा listing गन्तव्यम् local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         कुरु not invoke sync before getting usage info (स्वभाव)\n\
"));
      oputs (_("\
      --output[=क्षेत्रम्_LIST]\n\
         use the output format defined by क्षेत्रम्_LIST,\n\
         or print all fields यदि क्षेत्रम्_LIST is omitted\n\
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
      --योग_2\n\
         elide all entries insignificant गन्तव्यम् उपलब्धम् space,\n\
         and produce a grand योग_2\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         सीमा listing गन्तव्यम् file systems सम्बन्धि type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         सीमा listing गन्तव्यम् file systems not सम्बन्धि type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_आकार_note ();
      fputs (_("\n\
क्षेत्रम्_LIST is a comma-separated list सम्बन्धि columns गन्तव्यम् be included.  Valid\n\
क्षेत्रम् नामानि are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info page).\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  संरचना stat *stats = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  वर्ण ध्रुव *सन्देशः_mut_excl = _("options %s and %s are mutually exclusive");

  यावत् (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      यदि (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'a':
          show_all_fs = true;
          भङ्ग;
        पक्ष 'B':
          {
            गणना strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_खण्डम्_आकार);
            यदि (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          भङ्ग;
        पक्ष 'i':
          यदि (शीर्षकम्_बहुलकम् == OUTPUT_बहुलकम्)
            {
              error (0, 0, सन्देशः_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          शीर्षकम्_बहुलकम् = INODES_बहुलकम्;
          भङ्ग;
        पक्ष 'h':
          human_output_opts = human_autoscale | human_SI | human_आधारः_1024;
          output_खण्डम्_आकार = 1;
          भङ्ग;
        पक्ष 'H':
          human_output_opts = human_autoscale | human_SI;
          output_खण्डम्_आकार = 1;
          भङ्ग;
        पक्ष 'k':
          human_output_opts = 0;
          output_खण्डम्_आकार = 1024;
          भङ्ग;
        पक्ष 'l':
          show_local_fs = true;
          भङ्ग;
        पक्ष 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_खण्डम्_आकार = 1024 * 1024;
          भङ्ग;
        पक्ष 'T':
          यदि (शीर्षकम्_बहुलकम् == OUTPUT_बहुलकम्)
            {
              error (0, 0, सन्देशः_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          भङ्ग;
        पक्ष 'P':
          यदि (शीर्षकम्_बहुलकम् == OUTPUT_बहुलकम्)
            {
              error (0, 0, सन्देशः_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          भङ्ग;
        पक्ष SYNC_OPTION:
          require_sync = true;
          भङ्ग;
        पक्ष NO_SYNC_OPTION:
          require_sync = false;
          भङ्ग;

        पक्ष 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        पक्ष 't':
          सङ्कलय_fs_type (optarg);
          भङ्ग;

        पक्ष 'v':		/* For SysV compatibility.  */
          /* ignore */
          भङ्ग;
        पक्ष 'x':
          सङ्कलय_excluded_fs_type (optarg);
          भङ्ग;

        पक्ष OUTPUT_OPTION:
          यदि (शीर्षकम्_बहुलकम् == INODES_बहुलकम्)
            {
              error (0, 0, सन्देशः_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          यदि (posix_format && शीर्षकम्_बहुलकम् == DEFAULT_बहुलकम्)
            {
              error (0, 0, सन्देशः_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          यदि (print_type)
            {
              error (0, 0, सन्देशः_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          शीर्षकम्_बहुलकम् = OUTPUT_बहुलकम्;
          यदि (optarg)
            decode_output_arg (optarg);
          भङ्ग;

        पक्ष योग_OPTION:
          print_grand_योग = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (human_output_opts == -1)
    {
      यदि (posix_format)
        {
          human_output_opts = 0;
          output_खण्डम्_आकार = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      अन्यथा
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_खण्डम्_आकार);
    }

  यदि (शीर्षकम्_बहुलकम् == INODES_बहुलकम् || शीर्षकम्_बहुलकम् == OUTPUT_बहुलकम्)
    ;
  अन्यथा यदि (human_output_opts & human_autoscale)
    शीर्षकम्_बहुलकम् = HUMAN_बहुलकम्;
  अन्यथा यदि (posix_format)
    शीर्षकम्_बहुलकम् = POSIX_बहुलकम्;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool मेलनम् = false;
    प्रति (संरचना fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_अग्र)
      {
        प्रति (संरचना fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_अग्र)
          {
            यदि (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                मेलनम् = true;
                भङ्ग;
              }
          }
      }
    यदि (मेलनम्)
      प्रत्यागम EXIT_FAILURE;
  }

  यदि (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, परिमाण *stats);
      प्रति (पूर्णाङ्क i = optind; i < argc; ++i)
        {
          पूर्णाङ्क err = automount_stat_err (argv[i], &stats[i - optind]);
          यदि (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_स्थितिः = EXIT_FAILURE;
              argv[i] = रिक्त;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != रिक्त
                            || fs_exclude_list != रिक्त
                            || print_type
                            || क्षेत्रम्_दत्त_2[FSTYPE_क्षेत्रम्].प्रयुक्तम्
                            || show_local_fs));

  यदि (mount_list == रिक्त)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      पूर्णाङ्क स्थितिः = 0;
      यदि ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != रिक्त
               || fs_exclude_list != रिक्त))
        {
          स्थितिः = EXIT_FAILURE;
        }
      वर्ण ध्रुव *warning = (स्थितिः == 0 ? _("Warning: ") : "");
      error (स्थितिः, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  यदि (require_sync)
    sync ();

  get_क्षेत्रम्_list ();
  get_शीर्षकम् ();

  यदि (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      प्रति (पूर्णाङ्क i = optind; i < argc; ++i)
        यदि (argv[i])
          get_प्रविष्टिः (argv[i], &stats[i - optind]);
    }
  अन्यथा
    get_all_entries ();

  यदि (file_systems_processed)
    {
      यदि (print_grand_योग)
        get_dev ("total",
                 (क्षेत्रम्_दत्त_2[SOURCE_क्षेत्रम्].प्रयुक्तम् ? "-" : "total"),
                 रिक्त, रिक्त, रिक्त, false, false, &grand_fsu, false);

      print_सारणी ();
    }
  अन्यथा
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      यदि (exit_स्थितिः == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_स्थितिः);
}
