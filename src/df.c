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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "df"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ರಚನೆ devlist
{
  dev_t dev_ಅಂಕೆ;
  ರಚನೆ mount_ನಮೂದು *me;
  ರಚನೆ devlist *ಮುಂದಿನ;
  ರಚನೆ devlist *ಮುಂದಿನ_same_dev;
  ರಚನೆ devlist *ನೋಡಿದ_ಕೊನೆಯ; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *devlist_ಕೋಷ್ಟಕ;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ಸ್ಥಿರ bool show_all_fs;

/* If true, show only local file systems.  */
ಸ್ಥಿರ bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ಸ್ಥಿರ bool show_listed_fs;

/* Human-readable options for output.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ human_output_opts = -1;

/* The units to use when printing sizes.  */
ಸ್ಥಿರ uintmax_t output_ಖಂಡ_ಗಾತ್ರ;

/* True if a file system has been processed for output.  */
ಸ್ಥಿರ bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ಸ್ಥಿರ bool require_sync;

/* Desired exit status.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ;

/* A file system type to display.  */

ರಚನೆ fs_type_list
{
  ಅಕ್ಷರ *fs_name;
  ರಚನೆ fs_type_list *fs_ಮುಂದಿನ;
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

ಸ್ಥಿರ ರಚನೆ fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ಸ್ಥಿರ ರಚನೆ fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ಸ್ಥಿರ ರಚನೆ mount_ನಮೂದು *mount_list;

/* If true, print file system type as well.  */
ಸ್ಥಿರ bool print_type;

/* If true, print a grand total at the end.  */
ಸ್ಥಿರ bool print_grand_ಒಟ್ಟು;

/* Grand total data.  */
ಸ್ಥಿರ ರಚನೆ fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ಸ್ಥಿರ ಎಣಿಕೆ
{
  DEFAULT_ಬಹುಲಕ,
  INODES_ಬಹುಲಕ,
  HUMAN_ಬಹುಲಕ,
  POSIX_ಬಹುಲಕ,
  OUTPUT_ಬಹುಲಕ
} ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = DEFAULT_ಬಹುಲಕ;

/* Displayable fields.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{
  ಮೂಲ_ಕ್ಷೇತ್ರ, /* file system */
  FSTYPE_ಕ್ಷೇತ್ರ, /* FS type */
  ಗಾತ್ರ_ಕ್ಷೇತ್ರ,   /* FS size */
  ಬಳಸಿದ_ಕ್ಷೇತ್ರ,   /* FS size used  */
  AVAIL_ಕ್ಷೇತ್ರ,  /* FS size available */
  PCENT_ಕ್ಷೇತ್ರ,  /* percent used */
  ITOTAL_ಕ್ಷೇತ್ರ, /* inode total */
  IUSED_ಕ್ಷೇತ್ರ,  /* inodes used */
  IAVAIL_ಕ್ಷೇತ್ರ, /* inodes available */
  IPCENT_ಕ್ಷೇತ್ರ, /* inodes used in percent */
  ಗುರಿ_ಕ್ಷೇತ್ರ, /* mount point */
  FILE_ಕ್ಷೇತ್ರ,   /* specified file name */
  INVALID_ಕ್ಷೇತ್ರ /* validation marker */
} display_ಕ್ಷೇತ್ರ_t;

/* Flag if a field contains a block, an inode or another value.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{
  ಖಂಡ_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} ಕ್ಷೇತ್ರ_type_t;

/* Attributes of a display field.  */
ರಚನೆ ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_t
{
  display_ಕ್ಷೇತ್ರ_t ಕ್ಷೇತ್ರ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg;
  ಕ್ಷೇತ್ರ_type_t ಕ್ಷೇತ್ರ_type;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *caption;/* NULL means use default header of this field.  */
  ಪೂರ್ಣಾಂಕ ಅಗಲ;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ಬಲ;   /* Whether to right-align columns, not left-align.  */
  bool ಬಳಸಿದ;
};

/* Header strings, minimum width and alignment for the above fields.  */
ಸ್ಥಿರ ರಚನೆ ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_t ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[] = {
  [ಮೂಲ_ಕ್ಷೇತ್ರ] = { ಮೂಲ_ಕ್ಷೇತ್ರ,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_ಕ್ಷೇತ್ರ] = { FSTYPE_ಕ್ಷೇತ್ರ,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [ಗಾತ್ರ_ಕ್ಷೇತ್ರ] = { ಗಾತ್ರ_ಕ್ಷೇತ್ರ,
    "size",   ಖಂಡ_FLD, N_("blocks"),      5, true, false },

  [ಬಳಸಿದ_ಕ್ಷೇತ್ರ] = { ಬಳಸಿದ_ಕ್ಷೇತ್ರ,
    "used",   ಖಂಡ_FLD, N_("Used"),        5, true, false },

  [AVAIL_ಕ್ಷೇತ್ರ] = { AVAIL_ಕ್ಷೇತ್ರ,
    "avail",  ಖಂಡ_FLD, N_("Available"),   5, true, false },

  [PCENT_ಕ್ಷೇತ್ರ] = { PCENT_ಕ್ಷೇತ್ರ,
    "pcent",  ಖಂಡ_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_ಕ್ಷೇತ್ರ] = { ITOTAL_ಕ್ಷೇತ್ರ,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_ಕ್ಷೇತ್ರ] = { IUSED_ಕ್ಷೇತ್ರ,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_ಕ್ಷೇತ್ರ] = { IAVAIL_ಕ್ಷೇತ್ರ,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_ಕ್ಷೇತ್ರ] = { IPCENT_ಕ್ಷೇತ್ರ,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [ಗುರಿ_ಕ್ಷೇತ್ರ] = { ಗುರಿ_ಕ್ಷೇತ್ರ,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_ಕ್ಷೇತ್ರ] = { FILE_ಕ್ಷೇತ್ರ,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *all_ವಾದಗಳು_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ಸ್ಥಿರ ರಚನೆ
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  ಅಕ್ಷರ ***ಅಡ್ಡಸಾಲುಗಳು COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ರಚನೆ ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_t **columns COUNTED_BY (ncolumns);
} ಕೋಷ್ಟಕ;

/* Field values.  */
ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t
{
  uintmax_t ಒಳಹರಿವು_units;
  uintmax_t output_units;
  uintmax_t ಒಟ್ಟು;
  uintmax_t ಲಭ್ಯ;
  bool negate_ಲಭ್ಯ;
  uintmax_t ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು;
  uintmax_t ಬಳಸಿದ;
  bool negate_ಬಳಸಿದ;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  ಒಟ್ಟು_OPTION,
  OUTPUT_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"block-size", required_ವಾದ, ಖಾಲಿ, 'B'},
  {"inodes", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"human-readable", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"si", no_ವಾದ, ಖಾಲಿ, 'H'},
  {"local", no_ವಾದ, ಖಾಲಿ, 'l'},
  {"output", optional_ವಾದ, ಖಾಲಿ, OUTPUT_OPTION},
  {"portability", no_ವಾದ, ಖಾಲಿ, 'P'},
  {"print-type", no_ವಾದ, ಖಾಲಿ, 'T'},
  {"sync", no_ವಾದ, ಖಾಲಿ, SYNC_OPTION},
  {"no-sync", no_ವಾದ, ಖಾಲಿ, NO_SYNC_OPTION},
  {"total", no_ವಾದ, ಖಾಲಿ, ಒಟ್ಟು_OPTION},
  {"type", required_ವಾದ, ಖಾಲಿ, 't'},
  {"exclude-type", required_ವಾದ, ಖಾಲಿ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
automount_stat_err (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ stat *st)
{
  ಪೂರ್ಣಾಂಕ fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ಆದರೆ (fd < 0)
    {
      ಆದರೆ (errno == ENOENT || errno == ENOTDIR)
        ಹಿಂತಿರುಗಿ errno;
      ಹಿಂತಿರುಗಿ stat (file, st) == 0 ? 0 : errno;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      ಹಿಂತಿರುಗಿ err;
    }
}

ಎಣಿಕೆ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ಸ್ಥಿರ ಶೂನ್ಯ
replace_control_chars (ಅಕ್ಷರ *cell)
{
  ಅಕ್ಷರ *p = cell;
  ಆಗ (*p)
    {
      ಆದರೆ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ಸ್ಥಿರ ಶೂನ್ಯ
replace_invalid_chars (ಅಕ್ಷರ *cell)
{
  ಅಕ್ಷರ *srcend = cell + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cell);
  ಅಕ್ಷರ *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  ಗಾಗಿ (ಅಕ್ಷರ *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ಆದರೆ (ok)
        ok = !c32iscntrl (wc);
      ಇಲ್ಲದಿದ್ದರೆ
        n = 1;

      ಆದರೆ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ಸ್ಥಿರ ಶೂನ್ಯ
replace_problematic_chars (ಅಕ್ಷರ *cell)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tty_ಹೊರಹೋಗು = -1;
  ಆದರೆ (tty_ಹೊರಹೋಗು < 0)
    tty_ಹೊರಹೋಗು = isatty (STDOUT_FILENO);

  (tty_ಹೊರಹೋಗು ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು (ಶೂನ್ಯ)
{
  ಆದರೆ (ಕೋಷ್ಟಕ.nrows == ಕೋಷ್ಟಕ.nrows_alloc)
    ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು = xpalloc (ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು, &ಕೋಷ್ಟಕ.nrows_alloc, 1, -1,
                          ಗಾತ್ರ *ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು);
  idx_t ಸ್ಥಿರಾಂಕ ಅಡ್ಡಸಾಲು_idx = ಕೋಷ್ಟಕ.nrows++;
  ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು[ಅಡ್ಡಸಾಲು_idx] = xinmalloc (ಕೋಷ್ಟಕ.ncolumns, ಗಾತ್ರ *ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಕೋಷ್ಟಕ (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t ಅಡ್ಡಸಾಲು = 0; ಅಡ್ಡಸಾಲು < ಕೋಷ್ಟಕ.nrows; ಅಡ್ಡಸಾಲು++)
    {
      ಗಾಗಿ (idx_t ಲಂಬಸಾಲು = 0; ಲಂಬಸಾಲು < ಕೋಷ್ಟಕ.ncolumns; ಲಂಬಸಾಲು++)
        {
          ಅಕ್ಷರ *cell = ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು[ಅಡ್ಡಸಾಲು][ಲಂಬಸಾಲು];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ಆದರೆ (ಲಂಬಸಾಲು != 0)
            ಅಕ್ಷರ_ಬರೆ (' ');

          ಪೂರ್ಣಾಂಕ ಅಗಲ = mbswidth (cell, MBSWIDTH_FLAGS);
          ಪೂರ್ಣಾಂಕ fill = ಅಗಲ < 0 ? 0 : ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಅಗಲ - ಅಗಲ;
          ಆದರೆ (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->align_ಬಲ)
            ಗಾಗಿ (; 0 < fill; fill--)
              ಅಕ್ಷರ_ಬರೆ (' ');
          fputs (cell, stdout);
          ಆದರೆ (ಲಂಬಸಾಲು + 1 < ಕೋಷ್ಟಕ.ncolumns)
            ಗಾಗಿ (; 0 < fill; fill--)
              ಅಕ್ಷರ_ಬರೆ (' ');
        }
      ಅಕ್ಷರ_ಬರೆ ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ಸ್ಥಿರ ಶೂನ್ಯ
alloc_ಕ್ಷೇತ್ರ (ಪೂರ್ಣಾಂಕ f, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *c)
{
  ಆದರೆ (ಕೋಷ್ಟಕ.ncolumns == ಕೋಷ್ಟಕ.ncolumns_alloc)
    ಕೋಷ್ಟಕ.columns = xpalloc (ಕೋಷ್ಟಕ.columns, &ಕೋಷ್ಟಕ.ncolumns_alloc, 1, -1,
                             ಗಾತ್ರ *ಕೋಷ್ಟಕ.columns);
  idx_t ಸ್ಥಿರಾಂಕ ಸ್ತಂಭ_idx = ಕೋಷ್ಟಕ.ncolumns++;
  ಕೋಷ್ಟಕ.columns[ಸ್ತಂಭ_idx] = &ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[f];
  ಆದರೆ (c != ಖಾಲಿ)
    ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[f].caption = c;

  affirm (!ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[f].ಬಳಸಿದ);

  /* Mark field as used.  */
  ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[f].ಬಳಸಿದ = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ಸ್ಥಿರ ಶೂನ್ಯ
decode_output_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಅಕ್ಷರ *arg_writable = xstrdup (arg);
  ಅಕ್ಷರ *s = arg_writable;
  ಮಾಡು
    {
      /* find next comma */
      ಅಕ್ಷರ *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ಆದರೆ (comma)
        *comma++ = 0;

      /* process S.  */
      display_ಕ್ಷೇತ್ರ_t ಕ್ಷೇತ್ರ = INVALID_ಕ್ಷೇತ್ರ;
      ಗಾಗಿ (idx_t i = 0; i < countof (ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2); i++)
        {
          ಆದರೆ (streq (ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[i].arg, s))
            {
              ಕ್ಷೇತ್ರ = i;
              ನಿಲ್ಲಿಸು;
            }
        }
      ಆದರೆ (ಕ್ಷೇತ್ರ == INVALID_ಕ್ಷೇತ್ರ)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ಆದರೆ (ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[ಕ್ಷೇತ್ರ].ಬಳಸಿದ)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[ಕ್ಷೇತ್ರ].arg));
          usage (EXIT_FAILURE);
        }

      ಆಯ್ಕೆ (ಕ್ಷೇತ್ರ)
        {
        ಸ್ಥಿತಿ ಮೂಲ_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ FSTYPE_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ ಬಳಸಿದ_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ PCENT_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ ITOTAL_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IUSED_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IAVAIL_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IPCENT_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ ಗುರಿ_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ FILE_ಕ್ಷೇತ್ರ:
          alloc_ಕ್ಷೇತ್ರ (ಕ್ಷೇತ್ರ, ಖಾಲಿ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗಾತ್ರ_ಕ್ಷೇತ್ರ:
          alloc_ಕ್ಷೇತ್ರ (ಕ್ಷೇತ್ರ, N_("Size"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ AVAIL_ಕ್ಷೇತ್ರ:
          alloc_ಕ್ಷೇತ್ರ (ಕ್ಷೇತ್ರ, N_("Avail"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ INVALID_ಕ್ಷೇತ್ರ:
        ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ಆಗ (s);

  ಬಿಡುಗಡೆ (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_ಕ್ಷೇತ್ರ_list (ಶೂನ್ಯ)
{
  ಆಯ್ಕೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ DEFAULT_ಬಹುಲಕ:
      alloc_ಕ್ಷೇತ್ರ (ಮೂಲ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ಆದರೆ (print_type)
        alloc_ಕ್ಷೇತ್ರ (FSTYPE_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಗಾತ್ರ_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಬಳಸಿದ_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (AVAIL_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (PCENT_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಗುರಿ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ HUMAN_ಬಹುಲಕ:
      alloc_ಕ್ಷೇತ್ರ (ಮೂಲ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ಆದರೆ (print_type)
        alloc_ಕ್ಷೇತ್ರ (FSTYPE_ಕ್ಷೇತ್ರ, ಖಾಲಿ);

      alloc_ಕ್ಷೇತ್ರ (ಗಾತ್ರ_ಕ್ಷೇತ್ರ,   N_("Size"));
      alloc_ಕ್ಷೇತ್ರ (ಬಳಸಿದ_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (AVAIL_ಕ್ಷೇತ್ರ,  N_("Avail"));
      alloc_ಕ್ಷೇತ್ರ (PCENT_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಗುರಿ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ INODES_ಬಹುಲಕ:
      alloc_ಕ್ಷೇತ್ರ (ಮೂಲ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ಆದರೆ (print_type)
        alloc_ಕ್ಷೇತ್ರ (FSTYPE_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ITOTAL_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (IUSED_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (IAVAIL_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (IPCENT_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಗುರಿ_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ POSIX_ಬಹುಲಕ:
      alloc_ಕ್ಷೇತ್ರ (ಮೂಲ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ಆದರೆ (print_type)
        alloc_ಕ್ಷೇತ್ರ (FSTYPE_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಗಾತ್ರ_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (ಬಳಸಿದ_ಕ್ಷೇತ್ರ,   ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (AVAIL_ಕ್ಷೇತ್ರ,  ಖಾಲಿ);
      alloc_ಕ್ಷೇತ್ರ (PCENT_ಕ್ಷೇತ್ರ,  N_("Capacity"));
      alloc_ಕ್ಷೇತ್ರ (ಗುರಿ_ಕ್ಷೇತ್ರ, ಖಾಲಿ);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OUTPUT_ಬಹುಲಕ:
      ಆದರೆ (!ಕೋಷ್ಟಕ.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_ವಾದಗಳು_string);
        }
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_ಶೀರ್ಷಿಕೆ (ಶೂನ್ಯ)
{
  alloc_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು ();

  ಗಾಗಿ (idx_t ಲಂಬಸಾಲು = 0; ಲಂಬಸಾಲು < ಕೋಷ್ಟಕ.ncolumns; ಲಂಬಸಾಲು++)
    {
      ಅಕ್ಷರ *cell;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಶೀರ್ಷಿಕೆ = _(ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->caption);

      ಆದರೆ (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಕ್ಷೇತ್ರ == ಗಾತ್ರ_ಕ್ಷೇತ್ರ
          && (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == DEFAULT_ಬಹುಲಕ
              || (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == OUTPUT_ಬಹುಲಕ
                  && !(human_output_opts & human_autoscale))))
        {
          ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 1];

          ಪೂರ್ಣಾಂಕ opts = (human_suppress_ಬಿಂದು_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_ಗುಂಪು_digits | human_ತಳ_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_ಖಂಡ_ಗಾತ್ರ;
          uintmax_t q1024 = output_ಖಂಡ_ಗಾತ್ರ;
          bool divisible_by_1000;
          bool divisible_by_1024;

          ಮಾಡು
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ಆಗ (divisible_by_1000 & divisible_by_1024);

          ಆದರೆ (divisible_by_1000 < divisible_by_1024)
            opts |= human_ತಳ_1024;
          ಆದರೆ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_ತಳ_1024;
          ಆದರೆ (! (opts & human_ತಳ_1024))
            opts |= human_B;

          ಅಕ್ಷರ *ಅಂಕೆ = human_readable (output_ಖಂಡ_ಗಾತ್ರ, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          ಶೀರ್ಷಿಕೆ = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), ಅಂಕೆ, ಶೀರ್ಷಿಕೆ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == POSIX_ಬಹುಲಕ
               && ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಕ್ಷೇತ್ರ == ಗಾತ್ರ_ಕ್ಷೇತ್ರ)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_ಖಂಡ_ಗಾತ್ರ, ಶೀರ್ಷಿಕೆ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        cell = xstrdup (ಶೀರ್ಷಿಕೆ);

      replace_problematic_chars (cell);

      ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು[ಕೋಷ್ಟಕ.nrows - 1][ಲಂಬಸಾಲು] = cell;

      ಪೂರ್ಣಾಂಕ cell_ಅಗಲ = mbswidth (cell, MBSWIDTH_FLAGS);
      ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಅಗಲ = MAX (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಅಗಲ, cell_ಅಗಲ);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
selected_fstype (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fstype)
{
  ಆದರೆ (fs_select_list == ಖಾಲಿ || fstype == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ true;
  ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ರಚನೆ fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_ಮುಂದಿನ)
    ಆದರೆ (streq (fstype, fsp->fs_name))
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
excluded_fstype (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fstype)
{
  ಆದರೆ (fs_exclude_list == ಖಾಲಿ || fstype == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ರಚನೆ fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_ಮುಂದಿನ)
    ಆದರೆ (streq (fstype, fsp->fs_name))
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ size_t
devlist_hash (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, size_t ಕೋಷ್ಟಕ_ಗಾತ್ರ)
{
  ರಚನೆ devlist ಸ್ಥಿರಾಂಕ *p = x;
  ಹಿಂತಿರುಗಿ (uintmax_t) {p->dev_ಅಂಕೆ} % ಕೋಷ್ಟಕ_ಗಾತ್ರ;
}

ಸ್ಥಿರ bool
devlist_compare (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *x, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *y)
{
  ರಚನೆ devlist ಸ್ಥಿರಾಂಕ *a = x;
  ರಚನೆ devlist ಸ್ಥಿರಾಂಕ *b = y;
  ಹಿಂತಿರುಗಿ a->dev_ಅಂಕೆ == b->dev_ಅಂಕೆ;
}

ಸ್ಥಿರ ರಚನೆ devlist *
devlist_for_dev (dev_t dev)
{
  ಆದರೆ (devlist_ಕೋಷ್ಟಕ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ರಚನೆ devlist dev_ನಮೂದು;
  dev_ನಮೂದು.dev_ಅಂಕೆ = dev;

  ರಚನೆ devlist *ಸಿಕ್ಕಿತು = hash_lookup (devlist_ಕೋಷ್ಟಕ, &dev_ನಮೂದು);
  ಆದರೆ (ಸಿಕ್ಕಿತು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  /* Return the last devlist entry we have seen with this dev_num */
  ಹಿಂತಿರುಗಿ ಸಿಕ್ಕಿತು->ನೋಡಿದ_ಕೊನೆಯ;
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
  ರಚನೆ devlist *device_list = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ mount_list_ಗಾತ್ರ = 0;

  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me; me = me->me_ಮುಂದಿನ)
    mount_list_ಗಾತ್ರ++;

  devlist_ಕೋಷ್ಟಕ = hash_initialize (mount_list_ಗಾತ್ರ, ಖಾಲಿ,
                                   devlist_hash, devlist_compare, ಖಾಲಿ);
  ಆದರೆ (devlist_ಕೋಷ್ಟಕ == ಖಾಲಿ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me;)
    {
      ರಚನೆ stat buf;
      ರಚನೆ mount_ನಮೂದು *discard_me = ಖಾಲಿ;
      ರಚನೆ devlist *ಕೊನೆಯ_ನೋಡಿದ_dev = ಖಾಲಿ, *ನೋಡಿದ_dev = ಖಾಲಿ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ಆದರೆ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If we've already seen this device...  */
          ಕೊನೆಯ_ನೋಡಿದ_dev = ನೋಡಿದ_dev = devlist_for_dev (buf.st_dev);

          ಗಾಗಿ (; ನೋಡಿದ_dev && ! discard_me; ನೋಡಿದ_dev = ನೋಡಿದ_dev->ಮುಂದಿನ_same_dev)
            {
              bool ಗುರಿ_nearer_ಬೇರು = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ನೋಡಿದ_dev->me->me_mountdir)
                                        > ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool ಮೂಲ_ಕೆಳಗೆ_ಬೇರು = ನೋಡಿದ_dev->me->me_mntroot != ಖಾಲಿ
                                       && me->me_mntroot != ಖಾಲಿ
                                       && (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ನೋಡಿದ_dev->me->me_mntroot)
                                           < ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (me->me_mntroot));
              ಆದರೆ (! print_grand_ಒಟ್ಟು
                  && me->me_remote && ನೋಡಿದ_dev->me->me_remote
                  && ! streq (ನೋಡಿದ_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (ನೋಡಿದ_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (ಗುರಿ_nearer_ಬೇರು && ! ಮೂಲ_ಕೆಳಗೆ_ಬೇರು)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (ನೋಡಿದ_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     ನೋಡಿದ_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = ನೋಡಿದ_dev->me;
                  ನೋಡಿದ_dev->me = me;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ಆದರೆ (discard_me)
        {
          me = me->me_ಮುಂದಿನ;
          ಆದರೆ (! devices_only)
            free_mount_ನಮೂದು (discard_me);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Add the device number to the device_table.  */
          ರಚನೆ devlist *devlist = xmalloc (ಗಾತ್ರ *devlist);
          devlist->me = me;
          devlist->dev_ಅಂಕೆ = buf.st_dev;
          devlist->ಮುಂದಿನ_same_dev = ಕೊನೆಯ_ನೋಡಿದ_dev;
          devlist->ಮುಂದಿನ = device_list;
          device_list = devlist;

          ರಚನೆ devlist *hash_ನಮೂದು = hash_ಸೇರಿಸು (devlist_ಕೋಷ್ಟಕ, devlist);
          ಆದರೆ (hash_ನಮೂದು == ಖಾಲಿ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_ನಮೂದು->ನೋಡಿದ_ಕೊನೆಯ = devlist;

          me = me->me_ಮುಂದಿನ;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ಆದರೆ (! devices_only)
    {
      mount_list = ಖಾಲಿ;
      ಆಗ (device_list)
        {
          /* Add the mount entry.  */
          ರಚನೆ mount_ನಮೂದು *me = device_list->me;
          me->me_ಮುಂದಿನ = mount_list;
          mount_list = me;
          ರಚನೆ devlist *ಮುಂದಿನ = device_list->ಮುಂದಿನ;
          ಬಿಡುಗಡೆ (device_list);
          device_list = ಮುಂದಿನ;
        }
      hash_free (devlist_ಕೋಷ್ಟಕ);
      devlist_ಕೋಷ್ಟಕ = ಖಾಲಿ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *
me_for_dev (dev_t dev)
{
  ರಚನೆ devlist *dl = devlist_for_dev (dev);
  ಆದರೆ (dl)
        ಹಿಂತಿರುಗಿ dl->me;

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ಸ್ಥಿರ bool
ತಿಳಿದ_ಮೌಲ್ಯ (uintmax_t n)
{
  ಹಿಂತಿರುಗಿ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
df_readable (bool negative, uintmax_t n, ಅಕ್ಷರ *buf,
             uintmax_t ಒಳಹರಿವು_units, uintmax_t output_units)
{
  ಆದರೆ (! ತಿಳಿದ_ಮೌಲ್ಯ (n) && !negative)
    ಹಿಂತಿರುಗಿ "-";
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ಒಳಹರಿವು_units, output_units);
      ಆದರೆ (negative)
        *--p = '-';
      ಹಿಂತಿರುಗಿ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ಆದರೆ (*dest_neg == src_neg)
    {
      *dest += src;
      ಹಿಂತಿರುಗಿ;
    }

  ಆದರೆ (*dest_neg)
    *dest = -*dest;

  ಆದರೆ (src_neg)
    src = -src;

  ಆದರೆ (src < *dest)
    *dest -= src;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ಆದರೆ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
has_uuid_suffix (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);
  ಹಿಂತಿರುಗಿ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು (ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t *bv,
                  ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t *iv,
                  ಸ್ಥಿರಾಂಕ ರಚನೆ fs_usage *fsu)
{
  /* Inode values.  */
  iv->ಒಳಹರಿವು_units = iv->output_units = 1;
  iv->ಒಟ್ಟು = fsu->fsu_files;
  iv->ಲಭ್ಯ = iv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು = fsu->fsu_ffree;
  iv->negate_ಲಭ್ಯ = false;

  iv->ಬಳಸಿದ = UINTMAX_MAX;
  iv->negate_ಬಳಸಿದ = false;
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (iv->ಒಟ್ಟು) && ತಿಳಿದ_ಮೌಲ್ಯ (iv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು))
    {
      iv->ಬಳಸಿದ = iv->ಒಟ್ಟು - iv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು;
      iv->negate_ಬಳಸಿದ = (iv->ಒಟ್ಟು < iv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು);
    }

  /* Block values.  */
  bv->ಒಳಹರಿವು_units = fsu->fsu_blocksize;
  bv->output_units = output_ಖಂಡ_ಗಾತ್ರ;
  bv->ಒಟ್ಟು = fsu->fsu_ಖಂಡಗಳು;
  bv->ಲಭ್ಯ = fsu->fsu_bavail;
  bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು = fsu->fsu_bfree;
  bv->negate_ಲಭ್ಯ = (fsu->fsu_bavail_ತುದಿ_ಬಿಟ್_ಗಣ
                         && ತಿಳಿದ_ಮೌಲ್ಯ (fsu->fsu_bavail));

  bv->ಬಳಸಿದ = UINTMAX_MAX;
  bv->negate_ಬಳಸಿದ = false;
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (bv->ಒಟ್ಟು) && ತಿಳಿದ_ಮೌಲ್ಯ (bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು))
    {
      bv->ಬಳಸಿದ = bv->ಒಟ್ಟು - bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು;
      bv->negate_ಬಳಸಿದ = (bv->ಒಟ್ಟು < bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು);
    }
}

/* Add block and inode values to grand total.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ಗಮ್ಯ_grand_ಒಟ್ಟು (ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t *bv, ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t *iv)
{
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (iv->ಒಟ್ಟು))
    grand_fsu.fsu_files += iv->ಒಟ್ಟು;
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (iv->ಲಭ್ಯ))
    grand_fsu.fsu_ffree += iv->ಲಭ್ಯ;

  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (bv->ಒಟ್ಟು))
    grand_fsu.fsu_ಖಂಡಗಳು += bv->ಒಳಹರಿವು_units * bv->ಒಟ್ಟು;
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು))
    grand_fsu.fsu_bfree += bv->ಒಳಹರಿವು_units * bv->ಲಭ್ಯ_ಗಮ್ಯ_ಬೇರು;
  ಆದರೆ (ತಿಳಿದ_ಮೌಲ್ಯ (bv->ಲಭ್ಯ))
    ಕೂಡಿಸು_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_ತುದಿ_ಬಿಟ್_ಗಣ,
                            bv->ಒಳಹರಿವು_units * bv->ಲಭ್ಯ,
                            bv->negate_ಲಭ್ಯ);
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
get_dev (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *mount_ಬಿಂದು, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file,
         ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *stat_file, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fstype,
         bool me_dummy, bool me_remote,
         ಸ್ಥಿರಾಂಕ ರಚನೆ fs_usage *force_fsu,
         bool process_all)
{
  ಆದರೆ (me_remote && show_local_fs)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (me_dummy && !show_all_fs && !show_listed_fs)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (!selected_fstype (fstype) || excluded_fstype (fstype))
    ಹಿಂತಿರುಗಿ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ಆದರೆ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_ಬಿಂದು))
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (!stat_file)
    stat_file = mount_ಬಿಂದು;

  ರಚನೆ fs_usage fsu;
  ಆದರೆ (force_fsu)
    fsu = *force_fsu;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ಆದರೆ (process_all && (errno == EACCES || errno == ENOENT))
        {
          ಆದರೆ (! show_all_fs)
            ಹಿಂತಿರುಗಿ;

          fstype = "-";
          fsu.fsu_bavail_ತುದಿ_ಬಿಟ್_ಗಣ = false;
          fsu.fsu_blocksize = fsu.fsu_ಖಂಡಗಳು = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
          ಹಿಂತಿರುಗಿ;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ರಚನೆ stat sb;
      ಆದರೆ (stat (stat_file, &sb) == 0)
        {
          ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ * dev_me = me_for_dev (sb.st_dev);
          ಆದರೆ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_ತುದಿ_ಬಿಟ್_ಗಣ = false;
              fsu.fsu_blocksize = fsu.fsu_ಖಂಡಗಳು = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ಆದರೆ (fsu.fsu_ಖಂಡಗಳು == 0 && !show_all_fs && !show_listed_fs)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (! force_fsu)
    file_systems_processed = true;

  alloc_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು ();

  ಆದರೆ (! device)
    device = "-";		/* unknown */

  ಆದರೆ (! file)
    file = "-";			/* unspecified */

  ಅಕ್ಷರ *dev_name = xstrdup (device);
  ಅಕ್ಷರ *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ಆದರೆ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_ಬಹುಲಕ (dev_name, CAN_EXISTING)))
    {
      ಬಿಡುಗಡೆ (dev_name);
      dev_name = resolved_dev;
    }

  ಆದರೆ (! fstype)
    fstype = "-";		/* unknown */

  ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t ಖಂಡ_ಮೌಲ್ಯಗಳು;
  ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t inode_ಮೌಲ್ಯಗಳು;
  get_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು (&ಖಂಡ_ಮೌಲ್ಯಗಳು, &inode_ಮೌಲ್ಯಗಳು, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ಆದರೆ (print_grand_ಒಟ್ಟು && ! force_fsu)
    ಕೂಡಿಸು_ಗಮ್ಯ_grand_ಒಟ್ಟು (&ಖಂಡ_ಮೌಲ್ಯಗಳು, &inode_ಮೌಲ್ಯಗಳು);

  ಗಾಗಿ (idx_t ಲಂಬಸಾಲು = 0; ಲಂಬಸಾಲು < ಕೋಷ್ಟಕ.ncolumns; ಲಂಬಸಾಲು++)
    {
      ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 2];
      ಅಕ್ಷರ *cell;

      ರಚನೆ ಕ್ಷೇತ್ರ_ಮೌಲ್ಯಗಳು_t *v;
      ಆಯ್ಕೆ (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಕ್ಷೇತ್ರ_type)
        {
        ಸ್ಥಿತಿ ಖಂಡ_FLD:
          v = &ಖಂಡ_ಮೌಲ್ಯಗಳು;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ INODE_FLD:
          v = &inode_ಮೌಲ್ಯಗಳು;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ OTHER_FLD:
          v = ಖಾಲಿ;
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (!"bad field_type");
        }

      ಆಯ್ಕೆ (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಕ್ಷೇತ್ರ)
        {
        ಸ್ಥಿತಿ ಮೂಲ_ಕ್ಷೇತ್ರ:
          cell = xstrdup (dev_name);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FSTYPE_ಕ್ಷೇತ್ರ:
          cell = xstrdup (fstype);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗಾತ್ರ_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ ITOTAL_ಕ್ಷೇತ್ರ:
          cell = xstrdup (df_readable (false, v->ಒಟ್ಟು, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಬಳಸಿದ_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IUSED_ಕ್ಷೇತ್ರ:
          cell = xstrdup (df_readable (v->negate_ಬಳಸಿದ, v->ಬಳಸಿದ, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ AVAIL_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IAVAIL_ಕ್ಷೇತ್ರ:
          cell = xstrdup (df_readable (v->negate_ಲಭ್ಯ, v->ಲಭ್ಯ, buf,
                                       v->ಒಳಹರಿವು_units, v->output_units));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PCENT_ಕ್ಷೇತ್ರ:
        ಸ್ಥಿತಿ IPCENT_ಕ್ಷೇತ್ರ:
          {
            ದ್ವಿಗುಣ pct = -1;
            ಆದರೆ (! ತಿಳಿದ_ಮೌಲ್ಯ (v->ಬಳಸಿದ) || ! ತಿಳಿದ_ಮೌಲ್ಯ (v->ಲಭ್ಯ))
              ;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!v->negate_ಬಳಸಿದ
                     && v->ಬಳಸಿದ <= UINTMAX_MAX / 100
                     && v->ಬಳಸಿದ + v->ಲಭ್ಯ != 0
                     && (v->ಬಳಸಿದ + v->ಲಭ್ಯ < v->ಬಳಸಿದ)
                     == v->negate_ಲಭ್ಯ)
              {
                uintmax_t u100 = v->ಬಳಸಿದ * 100;
                uintmax_t nonroot_ಒಟ್ಟು = v->ಬಳಸಿದ + v->ಲಭ್ಯ;
                pct = u100 / nonroot_ಒಟ್ಟು + (u100 % nonroot_ಒಟ್ಟು != 0);
              }
            ಇಲ್ಲದಿದ್ದರೆ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ದ್ವಿಗುಣ u = v->negate_ಬಳಸಿದ ? - (ದ್ವಿಗುಣ) - v->ಬಳಸಿದ : v->ಬಳಸಿದ;
                ದ್ವಿಗುಣ a = v->negate_ಲಭ್ಯ
                           ? - (ದ್ವಿಗುಣ) - v->ಲಭ್ಯ : v->ಲಭ್ಯ;
                ದ್ವಿಗುಣ nonroot_ಒಟ್ಟು = u + a;
                ಆದರೆ (nonroot_ಒಟ್ಟು)
                  {
                    ದೀರ್ಘ ಪೂರ್ಣಾಂಕ lipct = pct = u * 100 / nonroot_ಒಟ್ಟು;
                    ದ್ವಿಗುಣ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ಆದರೆ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ FILE_ಕ್ಷೇತ್ರ:
          cell = xstrdup (file);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಗುರಿ_ಕ್ಷೇತ್ರ:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HIDE_AUTOMOUNT_ಪೂರ್ವಪ್ರತ್ಯಯ
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ಆದರೆ (STRNCMP_LIT (mount_ಬಿಂದು, "/auto/") == 0)
            mount_ಬಿಂದು += 5;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STRNCMP_LIT (mount_ಬಿಂದು, "/tmp_mnt/") == 0)
            mount_ಬಿಂದು += 8;
#ಆದರೆ_ಅಂತ್ಯ
          cell = xstrdup (mount_ಬಿಂದು);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ INVALID_ಕ್ಷೇತ್ರ:
        ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      ಪೂರ್ಣಾಂಕ cell_ಅಗಲ = mbswidth (cell, MBSWIDTH_FLAGS);
      ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಅಗಲ = MAX (ಕೋಷ್ಟಕ.columns[ಲಂಬಸಾಲು]->ಅಗಲ, cell_ಅಗಲ);
      ಕೋಷ್ಟಕ.ಅಡ್ಡಸಾಲುಗಳು[ಕೋಷ್ಟಕ.nrows - 1][ಲಂಬಸಾಲು] = cell;
    }
  ಬಿಡುಗಡೆ (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
ಕೊನೆಯ_device_for_mount (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *mount)
{
  ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *le = ಖಾಲಿ;

  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *me = mount_list; me; me = me->me_ಮುಂದಿನ)
    {
      ಆದರೆ (streq (me->me_mountdir, mount))
        le = me;
    }

  ಆದರೆ (le)
    {
      ಅಕ್ಷರ *devname = le->me_devname;
      ಅಕ್ಷರ *canon_dev = canonicalize_file_name (devname);
      ಆದರೆ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        ಹಿಂತಿರುಗಿ canon_dev;
      ಬಿಡುಗಡೆ (canon_dev);
      ಹಿಂತಿರುಗಿ xstrdup (le->me_devname);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ಸ್ಥಿರ bool
get_device (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *device)
{
  ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *ಉತ್ತಮ_ಹೊಂದಿಕೆ = ಖಾಲಿ;
  bool ಉತ್ತಮ_ಹೊಂದಿಕೆ_accessible = false;
  bool eclipsed_device = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = device;

  ಅಕ್ಷರ *resolved = canonicalize_file_name (device);
  ಆದರೆ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t ಉತ್ತಮ_ಹೊಂದಿಕೆ_len = SIZE_MAX;
  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *me = mount_list; me; me = me->me_ಮುಂದಿನ)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      ಅಕ್ಷರ *devname = me->me_devname;
      ಅಕ್ಷರ *canon_dev = canonicalize_file_name (me->me_devname);
      ಆದರೆ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ಆದರೆ (streq (device, devname))
        {
          ಅಕ್ಷರ *ಕೊನೆಯ_device = ಕೊನೆಯ_device_for_mount (me->me_mountdir);
          eclipsed_device = ಕೊನೆಯ_device && ! streq (ಕೊನೆಯ_device, devname);
          size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (me->me_mountdir);

          ಆದರೆ (! eclipsed_device
              && (! ಉತ್ತಮ_ಹೊಂದಿಕೆ_accessible || len < ಉತ್ತಮ_ಹೊಂದಿಕೆ_len))
            {
              ರಚನೆ stat device_stats;
              bool this_ಹೊಂದಿಕೆ_accessible = false;

              ಆದರೆ (stat (me->me_mountdir, &device_stats) == 0)
                ಉತ್ತಮ_ಹೊಂದಿಕೆ_accessible = this_ಹೊಂದಿಕೆ_accessible = true;

              ಆದರೆ (this_ಹೊಂದಿಕೆ_accessible
                  || (! ಉತ್ತಮ_ಹೊಂದಿಕೆ_accessible && len < ಉತ್ತಮ_ಹೊಂದಿಕೆ_len))
                {
                  ಉತ್ತಮ_ಹೊಂದಿಕೆ = me;
                  ಆದರೆ (len == 1) /* Traditional root.  */
                    {
                      ಬಿಡುಗಡೆ (ಕೊನೆಯ_device);
                      ಬಿಡುಗಡೆ (canon_dev);
                      ನಿಲ್ಲಿಸು;
                    }
                  ಇಲ್ಲದಿದ್ದರೆ
                    ಉತ್ತಮ_ಹೊಂದಿಕೆ_len = len;
                }
            }

          ಬಿಡುಗಡೆ (ಕೊನೆಯ_device);
        }

      ಬಿಡುಗಡೆ (canon_dev);
    }

  ಬಿಡುಗಡೆ (resolved);

  ಆದರೆ (ಉತ್ತಮ_ಹೊಂದಿಕೆ)
    {
      get_dev (ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_devname, ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_mountdir, file, ಖಾಲಿ,
               ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_type, ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_dummy,
               ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_remote, ಖಾಲಿ, false);
      ಹಿಂತಿರುಗಿ true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
      ಹಿಂತಿರುಗಿ true;
    }

  ಹಿಂತಿರುಗಿ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ಸ್ಥಿರ ಶೂನ್ಯ
get_ಬಿಂದು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಬಿಂದು, ಸ್ಥಿರಾಂಕ ರಚನೆ stat *statp)
{
  ರಚನೆ stat device_stats;
  ರಚನೆ mount_ನಮೂದು ಸ್ಥಿರಾಂಕ *ಉತ್ತಮ_ಹೊಂದಿಕೆ = ಖಾಲಿ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  ಅಕ್ಷರ *resolved = canonicalize_file_name (ಬಿಂದು);
  ಆದರೆ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (resolved);
      size_t ಉತ್ತಮ_ಹೊಂದಿಕೆ_len = 0;

      ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me; me = me->me_ಮುಂದಿನ)
        {
          ಆದರೆ (!streq (me->me_type, "lofs")
              && (!ಉತ್ತಮ_ಹೊಂದಿಕೆ || ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_dummy || !me->me_dummy))
            {
              size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (me->me_mountdir);
              ಆದರೆ (ಉತ್ತಮ_ಹೊಂದಿಕೆ_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  ಉತ್ತಮ_ಹೊಂದಿಕೆ = me;
                  ಉತ್ತಮ_ಹೊಂದಿಕೆ_len = len;
                }
            }
        }
    }
  ಬಿಡುಗಡೆ (resolved);
  ಆದರೆ (ಉತ್ತಮ_ಹೊಂದಿಕೆ
      && (stat (ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    ಉತ್ತಮ_ಹೊಂದಿಕೆ = ಖಾಲಿ;

  ಆದರೆ (! ಉತ್ತಮ_ಹೊಂದಿಕೆ)
    ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me; me = me->me_ಮುಂದಿನ)
      {
        ಆದರೆ (me->me_dev == (dev_t) -1)
          {
            ಆದರೆ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ಇಲ್ಲದಿದ್ದರೆ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ಆದರೆ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ಆದರೆ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!ಉತ್ತಮ_ಹೊಂದಿಕೆ || ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ಆದರೆ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ಇಲ್ಲದಿದ್ದರೆ
              ಉತ್ತಮ_ಹೊಂದಿಕೆ = me;
          }
      }

  ಆದರೆ (ಉತ್ತಮ_ಹೊಂದಿಕೆ)
    get_dev (ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_devname, ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_mountdir, ಬಿಂದು, ಬಿಂದು,
             ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_type, ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_dummy, ಉತ್ತಮ_ಹೊಂದಿಕೆ->me_remote,
             ಖಾಲಿ, false);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      ಅಕ್ಷರ *mp = ಕಂಡುಹಿಡಿ_mount_ಬಿಂದು (ಬಿಂದು, statp);
      ಆದರೆ (mp)
        {
          get_dev (ಖಾಲಿ, mp, ಬಿಂದು, ಖಾಲಿ, ಖಾಲಿ,
                   false, false, ಖಾಲಿ, false);
          ಬಿಡುಗಡೆ (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_ನಮೂದು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *statp)
{
  ಆದರೆ ((S_ISBLK (statp->st_ಬಹುಲಕ) || S_ISCHR (statp->st_ಬಹುಲಕ))
      && get_device (name))
    ಹಿಂತಿರುಗಿ;

  get_ಬಿಂದು (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ಸ್ಥಿರ ಶೂನ್ಯ
get_all_entries (ಶೂನ್ಯ)
{
  filter_mount_list (show_all_fs);

  ಗಾಗಿ (ರಚನೆ mount_ನಮೂದು *me = mount_list; me; me = me->me_ಮುಂದಿನ)
    get_dev (me->me_devname, me->me_mountdir, ಖಾಲಿ, ಖಾಲಿ, me->me_type,
             me->me_dummy, me->me_remote, ಖಾಲಿ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_fs_type (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fstype)
{
  ರಚನೆ fs_type_list *fsp;

  fsp = xmalloc (ಗಾತ್ರ *fsp);
  fsp->fs_name = (ಅಕ್ಷರ *) fstype;
  fsp->fs_ಮುಂದಿನ = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_excluded_fs_type (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fstype)
{
  ರಚನೆ fs_type_list *fsp;

  fsp = xmalloc (ಗಾತ್ರ *fsp);
  fsp->fs_name = (ಅಕ್ಷರ *) fstype;
  fsp->fs_ಮುಂದಿನ = fs_exclude_list;
  fs_exclude_list = fsp;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by ಪೂರ್ವನಿಯೋಜಿತ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --ಖಂಡ-ಗಾತ್ರ_2=ಗಾತ್ರ_3\n\
         scale sizes by ಗಾತ್ರ_3 before printing them; see ಗಾತ್ರ_3 format ಕೆಳಗೆ;\n\
         E.g., '-BM' prints sizes ಒಳ units ದ 1,048,576 ಬೈಟುಗಳು\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes ಒಳ powers ದ 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes ಒಳ powers ದ 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead ದ ಖಂಡ usage\n\
"));
      oputs (_("\
  -k\n\
         like --ಖಂಡ-ಗಾತ್ರ_2=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         ಎಲ್ಲೆ listing ಗಮ್ಯ local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         ಮಾಡು not invoke sync before getting usage info (ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
      --output[=ಕ್ಷೇತ್ರ_LIST]\n\
         use the output format defined by ಕ್ಷೇತ್ರ_LIST,\n\
         or print all fields ಆದರೆ ಕ್ಷೇತ್ರ_LIST is omitted\n\
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
      --ಒಟ್ಟು\n\
         elide all entries insignificant ಗಮ್ಯ ಲಭ್ಯ space,\n\
         and produce a grand ಒಟ್ಟು\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         ಎಲ್ಲೆ listing ಗಮ್ಯ file systems ದ type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         ಎಲ್ಲೆ listing ಗಮ್ಯ file systems not ದ type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_ಗಾತ್ರ_note ();
      fputs (_("\n\
ಕ್ಷೇತ್ರ_LIST is a comma-separated list ದ columns ಗಮ್ಯ be included.  Valid\n\
ಕ್ಷೇತ್ರ ಹೆಸರುಗಳು are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info ಪುಟ).\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ರಚನೆ stat *stats = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_mut_excl = _("options %s and %s are mutually exclusive");

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'a':
          show_all_fs = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'B':
          {
            ಎಣಿಕೆ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ಖಂಡ_ಗಾತ್ರ);
            ಆದರೆ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == OUTPUT_ಬಹುಲಕ)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = INODES_ಬಹುಲಕ;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'h':
          human_output_opts = human_autoscale | human_SI | human_ತಳ_1024;
          output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'k':
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1024;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'l':
          show_local_fs = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1024 * 1024;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'T':
          ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == OUTPUT_ಬಹುಲಕ)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'P':
          ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == OUTPUT_ಬಹುಲಕ)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ SYNC_OPTION:
          require_sync = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ NO_SYNC_OPTION:
          require_sync = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        ಸ್ಥಿತಿ 't':
          ಕೂಡಿಸು_fs_type (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'v':		/* For SysV compatibility.  */
          /* ignore */
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'x':
          ಕೂಡಿಸು_excluded_fs_type (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ OUTPUT_OPTION:
          ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == INODES_ಬಹುಲಕ)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ಆದರೆ (posix_format && ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == DEFAULT_ಬಹುಲಕ)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ಆದರೆ (print_type)
            {
              error (0, 0, ಸಂದೇಶ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = OUTPUT_ಬಹುಲಕ;
          ಆದರೆ (optarg)
            decode_output_arg (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಒಟ್ಟು_OPTION:
          print_grand_ಒಟ್ಟು = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (human_output_opts == -1)
    {
      ಆದರೆ (posix_format)
        {
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_ಖಂಡ_ಗಾತ್ರ);
    }

  ಆದರೆ (ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == INODES_ಬಹುಲಕ || ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ == OUTPUT_ಬಹುಲಕ)
    ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (human_output_opts & human_autoscale)
    ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = HUMAN_ಬಹುಲಕ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (posix_format)
    ಶೀರ್ಷಿಕೆ_ಬಹುಲಕ = POSIX_ಬಹುಲಕ;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool ಹೊಂದಿಕೆ = false;
    ಗಾಗಿ (ರಚನೆ fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_ಮುಂದಿನ)
      {
        ಗಾಗಿ (ರಚನೆ fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_ಮುಂದಿನ)
          {
            ಆದರೆ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                ಹೊಂದಿಕೆ = true;
                ನಿಲ್ಲಿಸು;
              }
          }
      }
    ಆದರೆ (ಹೊಂದಿಕೆ)
      ಹಿಂತಿರುಗಿ EXIT_FAILURE;
  }

  ಆದರೆ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, ಗಾತ್ರ *stats);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; ++i)
        {
          ಪೂರ್ಣಾಂಕ err = automount_stat_err (argv[i], &stats[i - optind]);
          ಆದರೆ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
              argv[i] = ಖಾಲಿ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ಖಾಲಿ
                            || fs_exclude_list != ಖಾಲಿ
                            || print_type
                            || ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[FSTYPE_ಕ್ಷೇತ್ರ].ಬಳಸಿದ
                            || show_local_fs));

  ಆದರೆ (mount_list == ಖಾಲಿ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = 0;
      ಆದರೆ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ಖಾಲಿ
               || fs_exclude_list != ಖಾಲಿ))
        {
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *warning = (ಸ್ಥಿತಿ_2 == 0 ? _("Warning: ") : "");
      error (ಸ್ಥಿತಿ_2, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ಆದರೆ (require_sync)
    sync ();

  get_ಕ್ಷೇತ್ರ_list ();
  get_ಶೀರ್ಷಿಕೆ ();

  ಆದರೆ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; ++i)
        ಆದರೆ (argv[i])
          get_ನಮೂದು (argv[i], &stats[i - optind]);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    get_all_entries ();

  ಆದರೆ (file_systems_processed)
    {
      ಆದರೆ (print_grand_ಒಟ್ಟು)
        get_dev ("total",
                 (ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ_2[ಮೂಲ_ಕ್ಷೇತ್ರ].ಬಳಸಿದ ? "-" : "total"),
                 ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, false, false, &grand_fsu, false);

      print_ಕೋಷ್ಟಕ ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ಆದರೆ (exit_ಸ್ಥಿತಿ == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_ಸ್ಥಿತಿ);
}
