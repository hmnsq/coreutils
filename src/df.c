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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <c-ctype.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fsusage.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbswidth.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mountlist.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "find-mount-point.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "df"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ਬਣਤਰ devlist
{
  dev_t dev_ਅੰਕ;
  ਬਣਤਰ mount_ਇੰਦਰਾਜ *me;
  ਬਣਤਰ devlist *ਅਗਲਾ;
  ਬਣਤਰ devlist *ਅਗਲਾ_same_dev;
  ਬਣਤਰ devlist *ਵੇਖਿਆ_ਆਖਰੀ; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ਸਥਿਰ Hash_ਸਾਰਣੀ *devlist_ਸਾਰਣੀ;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ਸਥਿਰ bool show_all_fs;

/* If true, show only local file systems.  */
ਸਥਿਰ bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ਸਥਿਰ bool show_listed_fs;

/* Human-readable options for output.  */
ਸਥਿਰ ਪੂਰਨ human_output_opts = -1;

/* The units to use when printing sizes.  */
ਸਥਿਰ uintmax_t output_ਖੰਡ_ਮਾਪ;

/* True if a file system has been processed for output.  */
ਸਥਿਰ bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ਸਥਿਰ bool require_sync;

/* Desired exit status.  */
ਸਥਿਰ ਪੂਰਨ exit_ਸਥਿਤੀ;

/* A file system type to display.  */

ਬਣਤਰ fs_type_list
{
  ਅੱਖਰ *fs_name;
  ਬਣਤਰ fs_type_list *fs_ਅਗਲਾ;
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

ਸਥਿਰ ਬਣਤਰ fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ਸਥਿਰ ਬਣਤਰ fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ਸਥਿਰ ਬਣਤਰ mount_ਇੰਦਰਾਜ *mount_list;

/* If true, print file system type as well.  */
ਸਥਿਰ bool print_type;

/* If true, print a grand total at the end.  */
ਸਥਿਰ bool print_grand_ਕੁੱਲ;

/* Grand total data.  */
ਸਥਿਰ ਬਣਤਰ fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ਸਥਿਰ ਗਿਣਤੀ
{
  DEFAULT_ਬਹੁਲਕ,
  INODES_ਬਹੁਲਕ,
  HUMAN_ਬਹੁਲਕ,
  POSIX_ਬਹੁਲਕ,
  OUTPUT_ਬਹੁਲਕ
} ਸਿਰਲੇਖ_ਬਹੁਲਕ = DEFAULT_ਬਹੁਲਕ;

/* Displayable fields.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  ਸਰੋਤ_ਖੇਤਰ, /* file system */
  FSTYPE_ਖੇਤਰ, /* FS type */
  ਮਾਪ_ਖੇਤਰ,   /* FS size */
  ਵਰਤਿਆ_ਖੇਤਰ,   /* FS size used  */
  AVAIL_ਖੇਤਰ,  /* FS size available */
  PCENT_ਖੇਤਰ,  /* percent used */
  ITOTAL_ਖੇਤਰ, /* inode total */
  IUSED_ਖੇਤਰ,  /* inodes used */
  IAVAIL_ਖੇਤਰ, /* inodes available */
  IPCENT_ਖੇਤਰ, /* inodes used in percent */
  ਨਿਸ਼ਾਨਾ_ਖੇਤਰ, /* mount point */
  FILE_ਖੇਤਰ,   /* specified file name */
  INVALID_ਖੇਤਰ /* validation marker */
} display_ਖੇਤਰ_t;

/* Flag if a field contains a block, an inode or another value.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  ਖੰਡ_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} ਖੇਤਰ_type_t;

/* Attributes of a display field.  */
ਬਣਤਰ ਖੇਤਰ_ਅੰਕੜੇ_t
{
  display_ਖੇਤਰ_t ਖੇਤਰ;
  ਅੱਖਰ ਅਚਲ *arg;
  ਖੇਤਰ_type_t ਖੇਤਰ_type;
  ਅੱਖਰ ਅਚਲ *caption;/* NULL means use default header of this field.  */
  ਪੂਰਨ ਚੌੜਾਈ;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ਸੱਜਾ;   /* Whether to right-align columns, not left-align.  */
  bool ਵਰਤਿਆ;
};

/* Header strings, minimum width and alignment for the above fields.  */
ਸਥਿਰ ਬਣਤਰ ਖੇਤਰ_ਅੰਕੜੇ_t ਖੇਤਰ_ਅੰਕੜੇ_2[] = {
  [ਸਰੋਤ_ਖੇਤਰ] = { ਸਰੋਤ_ਖੇਤਰ,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_ਖੇਤਰ] = { FSTYPE_ਖੇਤਰ,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [ਮਾਪ_ਖੇਤਰ] = { ਮਾਪ_ਖੇਤਰ,
    "size",   ਖੰਡ_FLD, N_("blocks"),      5, true, false },

  [ਵਰਤਿਆ_ਖੇਤਰ] = { ਵਰਤਿਆ_ਖੇਤਰ,
    "used",   ਖੰਡ_FLD, N_("Used"),        5, true, false },

  [AVAIL_ਖੇਤਰ] = { AVAIL_ਖੇਤਰ,
    "avail",  ਖੰਡ_FLD, N_("Available"),   5, true, false },

  [PCENT_ਖੇਤਰ] = { PCENT_ਖੇਤਰ,
    "pcent",  ਖੰਡ_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_ਖੇਤਰ] = { ITOTAL_ਖੇਤਰ,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_ਖੇਤਰ] = { IUSED_ਖੇਤਰ,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_ਖੇਤਰ] = { IAVAIL_ਖੇਤਰ,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_ਖੇਤਰ] = { IPCENT_ਖੇਤਰ,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [ਨਿਸ਼ਾਨਾ_ਖੇਤਰ] = { ਨਿਸ਼ਾਨਾ_ਖੇਤਰ,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_ਖੇਤਰ] = { FILE_ਖੇਤਰ,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *all_ਦਲੀਲਾਂ_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ਸਥਿਰ ਬਣਤਰ
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  ਅੱਖਰ ***ਪੰਕਤੀਆਂ COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ਬਣਤਰ ਖੇਤਰ_ਅੰਕੜੇ_t **columns COUNTED_BY (ncolumns);
} ਸਾਰਣੀ;

/* Field values.  */
ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t
{
  uintmax_t ਨਿਵੇਸ਼_units;
  uintmax_t output_units;
  uintmax_t ਕੁੱਲ;
  uintmax_t ਉਪਲਬਧ;
  bool negate_ਉਪਲਬਧ;
  uintmax_t ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ;
  uintmax_t ਵਰਤਿਆ;
  bool negate_ਵਰਤਿਆ;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  ਕੁੱਲ_OPTION,
  OUTPUT_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"block-size", required_ਦਲੀਲ, ਨਿਰਾ, 'B'},
  {"inodes", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"human-readable", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"si", no_ਦਲੀਲ, ਨਿਰਾ, 'H'},
  {"local", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"output", optional_ਦਲੀਲ, ਨਿਰਾ, OUTPUT_OPTION},
  {"portability", no_ਦਲੀਲ, ਨਿਰਾ, 'P'},
  {"print-type", no_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"sync", no_ਦਲੀਲ, ਨਿਰਾ, SYNC_OPTION},
  {"no-sync", no_ਦਲੀਲ, ਨਿਰਾ, NO_SYNC_OPTION},
  {"total", no_ਦਲੀਲ, ਨਿਰਾ, ਕੁੱਲ_OPTION},
  {"type", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"exclude-type", required_ਦਲੀਲ, ਨਿਰਾ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ਸਥਿਰ ਪੂਰਨ
automount_stat_err (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ stat *st)
{
  ਪੂਰਨ fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ਜੇ (fd < 0)
    {
      ਜੇ (errno == ENOENT || errno == ENOTDIR)
        ਵਾਪਸ errno;
      ਵਾਪਸ stat (file, st) == 0 ? 0 : errno;
    }
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      ਵਾਪਸ err;
    }
}

ਗਿਣਤੀ { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ਸਥਿਰ ਖਾਲੀ
replace_control_chars (ਅੱਖਰ *cell)
{
  ਅੱਖਰ *p = cell;
  ਜਦੋਂ (*p)
    {
      ਜੇ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ਸਥਿਰ ਖਾਲੀ
replace_invalid_chars (ਅੱਖਰ *cell)
{
  ਅੱਖਰ *srcend = cell + ਲੜੀ_ਲੰਬਾਈ (cell);
  ਅੱਖਰ *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  ਲਈ (ਅੱਖਰ *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ਜੇ (ok)
        ok = !c32iscntrl (wc);
      ਨਹੀਂਤਾਂ
        n = 1;

      ਜੇ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ਨਹੀਂਤਾਂ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ਸਥਿਰ ਖਾਲੀ
replace_problematic_chars (ਅੱਖਰ *cell)
{
  ਸਥਿਰ ਪੂਰਨ tty_ਬਾਹਰਲਾ = -1;
  ਜੇ (tty_ਬਾਹਰਲਾ < 0)
    tty_ਬਾਹਰਲਾ = isatty (STDOUT_FILENO);

  (tty_ਬਾਹਰਲਾ ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ਸਥਿਰ ਖਾਲੀ
alloc_ਸਾਰਣੀ_ਪੰਕਤੀ (ਖਾਲੀ)
{
  ਜੇ (ਸਾਰਣੀ.nrows == ਸਾਰਣੀ.nrows_alloc)
    ਸਾਰਣੀ.ਪੰਕਤੀਆਂ = xpalloc (ਸਾਰਣੀ.ਪੰਕਤੀਆਂ, &ਸਾਰਣੀ.nrows_alloc, 1, -1,
                          ਆਕਾਰ *ਸਾਰਣੀ.ਪੰਕਤੀਆਂ);
  idx_t ਅਚਲ ਪੰਕਤੀ_idx = ਸਾਰਣੀ.nrows++;
  ਸਾਰਣੀ.ਪੰਕਤੀਆਂ[ਪੰਕਤੀ_idx] = xinmalloc (ਸਾਰਣੀ.ncolumns, ਆਕਾਰ *ਸਾਰਣੀ.ਪੰਕਤੀਆਂ[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ਸਥਿਰ ਖਾਲੀ
print_ਸਾਰਣੀ (ਖਾਲੀ)
{
  ਲਈ (idx_t ਪੰਕਤੀ = 0; ਪੰਕਤੀ < ਸਾਰਣੀ.nrows; ਪੰਕਤੀ++)
    {
      ਲਈ (idx_t ਖਾਨਾ = 0; ਖਾਨਾ < ਸਾਰਣੀ.ncolumns; ਖਾਨਾ++)
        {
          ਅੱਖਰ *cell = ਸਾਰਣੀ.ਪੰਕਤੀਆਂ[ਪੰਕਤੀ][ਖਾਨਾ];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ਜੇ (ਖਾਨਾ != 0)
            ਅੱਖਰ_ਲਿਖੋ (' ');

          ਪੂਰਨ ਚੌੜਾਈ = mbswidth (cell, MBSWIDTH_FLAGS);
          ਪੂਰਨ fill = ਚੌੜਾਈ < 0 ? 0 : ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਚੌੜਾਈ - ਚੌੜਾਈ;
          ਜੇ (ਸਾਰਣੀ.columns[ਖਾਨਾ]->align_ਸੱਜਾ)
            ਲਈ (; 0 < fill; fill--)
              ਅੱਖਰ_ਲਿਖੋ (' ');
          fputs (cell, stdout);
          ਜੇ (ਖਾਨਾ + 1 < ਸਾਰਣੀ.ncolumns)
            ਲਈ (; 0 < fill; fill--)
              ਅੱਖਰ_ਲਿਖੋ (' ');
        }
      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ਸਥਿਰ ਖਾਲੀ
alloc_ਖੇਤਰ (ਪੂਰਨ f, ਅੱਖਰ ਅਚਲ *c)
{
  ਜੇ (ਸਾਰਣੀ.ncolumns == ਸਾਰਣੀ.ncolumns_alloc)
    ਸਾਰਣੀ.columns = xpalloc (ਸਾਰਣੀ.columns, &ਸਾਰਣੀ.ncolumns_alloc, 1, -1,
                             ਆਕਾਰ *ਸਾਰਣੀ.columns);
  idx_t ਅਚਲ ਕਾਲਮ_idx = ਸਾਰਣੀ.ncolumns++;
  ਸਾਰਣੀ.columns[ਕਾਲਮ_idx] = &ਖੇਤਰ_ਅੰਕੜੇ_2[f];
  ਜੇ (c != ਨਿਰਾ)
    ਖੇਤਰ_ਅੰਕੜੇ_2[f].caption = c;

  affirm (!ਖੇਤਰ_ਅੰਕੜੇ_2[f].ਵਰਤਿਆ);

  /* Mark field as used.  */
  ਖੇਤਰ_ਅੰਕੜੇ_2[f].ਵਰਤਿਆ = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ਸਥਿਰ ਖਾਲੀ
decode_output_arg (ਅੱਖਰ ਅਚਲ *arg)
{
  ਅੱਖਰ *arg_writable = xstrdup (arg);
  ਅੱਖਰ *s = arg_writable;
  ਕਰੋ
    {
      /* find next comma */
      ਅੱਖਰ *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ਜੇ (comma)
        *comma++ = 0;

      /* process S.  */
      display_ਖੇਤਰ_t ਖੇਤਰ = INVALID_ਖੇਤਰ;
      ਲਈ (idx_t i = 0; i < countof (ਖੇਤਰ_ਅੰਕੜੇ_2); i++)
        {
          ਜੇ (streq (ਖੇਤਰ_ਅੰਕੜੇ_2[i].arg, s))
            {
              ਖੇਤਰ = i;
              ਰੋਕੋ;
            }
        }
      ਜੇ (ਖੇਤਰ == INVALID_ਖੇਤਰ)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ਜੇ (ਖੇਤਰ_ਅੰਕੜੇ_2[ਖੇਤਰ].ਵਰਤਿਆ)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (ਖੇਤਰ_ਅੰਕੜੇ_2[ਖੇਤਰ].arg));
          usage (EXIT_FAILURE);
        }

      ਚੋਣ (ਖੇਤਰ)
        {
        ਹਾਲਤ ਸਰੋਤ_ਖੇਤਰ:
        ਹਾਲਤ FSTYPE_ਖੇਤਰ:
        ਹਾਲਤ ਵਰਤਿਆ_ਖੇਤਰ:
        ਹਾਲਤ PCENT_ਖੇਤਰ:
        ਹਾਲਤ ITOTAL_ਖੇਤਰ:
        ਹਾਲਤ IUSED_ਖੇਤਰ:
        ਹਾਲਤ IAVAIL_ਖੇਤਰ:
        ਹਾਲਤ IPCENT_ਖੇਤਰ:
        ਹਾਲਤ ਨਿਸ਼ਾਨਾ_ਖੇਤਰ:
        ਹਾਲਤ FILE_ਖੇਤਰ:
          alloc_ਖੇਤਰ (ਖੇਤਰ, ਨਿਰਾ);
          ਰੋਕੋ;

        ਹਾਲਤ ਮਾਪ_ਖੇਤਰ:
          alloc_ਖੇਤਰ (ਖੇਤਰ, N_("Size"));
          ਰੋਕੋ;

        ਹਾਲਤ AVAIL_ਖੇਤਰ:
          alloc_ਖੇਤਰ (ਖੇਤਰ, N_("Avail"));
          ਰੋਕੋ;

        ਹਾਲਤ INVALID_ਖੇਤਰ:
        ਮੂਲ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  ਜਦੋਂ (s);

  ਮੁਕਤ (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ਸਥਿਰ ਖਾਲੀ
get_ਖੇਤਰ_list (ਖਾਲੀ)
{
  ਚੋਣ (ਸਿਰਲੇਖ_ਬਹੁਲਕ)
    {
    ਹਾਲਤ DEFAULT_ਬਹੁਲਕ:
      alloc_ਖੇਤਰ (ਸਰੋਤ_ਖੇਤਰ, ਨਿਰਾ);
      ਜੇ (print_type)
        alloc_ਖੇਤਰ (FSTYPE_ਖੇਤਰ, ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਮਾਪ_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਵਰਤਿਆ_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (AVAIL_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (PCENT_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਨਿਸ਼ਾਨਾ_ਖੇਤਰ, ਨਿਰਾ);
      ਰੋਕੋ;

    ਹਾਲਤ HUMAN_ਬਹੁਲਕ:
      alloc_ਖੇਤਰ (ਸਰੋਤ_ਖੇਤਰ, ਨਿਰਾ);
      ਜੇ (print_type)
        alloc_ਖੇਤਰ (FSTYPE_ਖੇਤਰ, ਨਿਰਾ);

      alloc_ਖੇਤਰ (ਮਾਪ_ਖੇਤਰ,   N_("Size"));
      alloc_ਖੇਤਰ (ਵਰਤਿਆ_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (AVAIL_ਖੇਤਰ,  N_("Avail"));
      alloc_ਖੇਤਰ (PCENT_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਨਿਸ਼ਾਨਾ_ਖੇਤਰ, ਨਿਰਾ);
      ਰੋਕੋ;

    ਹਾਲਤ INODES_ਬਹੁਲਕ:
      alloc_ਖੇਤਰ (ਸਰੋਤ_ਖੇਤਰ, ਨਿਰਾ);
      ਜੇ (print_type)
        alloc_ਖੇਤਰ (FSTYPE_ਖੇਤਰ, ਨਿਰਾ);
      alloc_ਖੇਤਰ (ITOTAL_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (IUSED_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (IAVAIL_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (IPCENT_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਨਿਸ਼ਾਨਾ_ਖੇਤਰ,  ਨਿਰਾ);
      ਰੋਕੋ;

    ਹਾਲਤ POSIX_ਬਹੁਲਕ:
      alloc_ਖੇਤਰ (ਸਰੋਤ_ਖੇਤਰ, ਨਿਰਾ);
      ਜੇ (print_type)
        alloc_ਖੇਤਰ (FSTYPE_ਖੇਤਰ, ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਮਾਪ_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (ਵਰਤਿਆ_ਖੇਤਰ,   ਨਿਰਾ);
      alloc_ਖੇਤਰ (AVAIL_ਖੇਤਰ,  ਨਿਰਾ);
      alloc_ਖੇਤਰ (PCENT_ਖੇਤਰ,  N_("Capacity"));
      alloc_ਖੇਤਰ (ਨਿਸ਼ਾਨਾ_ਖੇਤਰ, ਨਿਰਾ);
      ਰੋਕੋ;

    ਹਾਲਤ OUTPUT_ਬਹੁਲਕ:
      ਜੇ (!ਸਾਰਣੀ.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_ਦਲੀਲਾਂ_string);
        }
      ਰੋਕੋ;

    ਮੂਲ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ਸਥਿਰ ਖਾਲੀ
get_ਸਿਰਲੇਖ (ਖਾਲੀ)
{
  alloc_ਸਾਰਣੀ_ਪੰਕਤੀ ();

  ਲਈ (idx_t ਖਾਨਾ = 0; ਖਾਨਾ < ਸਾਰਣੀ.ncolumns; ਖਾਨਾ++)
    {
      ਅੱਖਰ *cell;
      ਅੱਖਰ ਅਚਲ *ਸਿਰਲੇਖ = _(ਸਾਰਣੀ.columns[ਖਾਨਾ]->caption);

      ਜੇ (ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਖੇਤਰ == ਮਾਪ_ਖੇਤਰ
          && (ਸਿਰਲੇਖ_ਬਹੁਲਕ == DEFAULT_ਬਹੁਲਕ
              || (ਸਿਰਲੇਖ_ਬਹੁਲਕ == OUTPUT_ਬਹੁਲਕ
                  && !(human_output_opts & human_autoscale))))
        {
          ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 1];

          ਪੂਰਨ opts = (human_suppress_ਬਿੰਦੂ_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_ਸਮੂਹ_digits | human_ਅਧਾਰ_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_ਖੰਡ_ਮਾਪ;
          uintmax_t q1024 = output_ਖੰਡ_ਮਾਪ;
          bool divisible_by_1000;
          bool divisible_by_1024;

          ਕਰੋ
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          ਜਦੋਂ (divisible_by_1000 & divisible_by_1024);

          ਜੇ (divisible_by_1000 < divisible_by_1024)
            opts |= human_ਅਧਾਰ_1024;
          ਜੇ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_ਅਧਾਰ_1024;
          ਜੇ (! (opts & human_ਅਧਾਰ_1024))
            opts |= human_B;

          ਅੱਖਰ *ਅੰਕ = human_readable (output_ਖੰਡ_ਮਾਪ, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          ਸਿਰਲੇਖ = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), ਅੰਕ, ਸਿਰਲੇਖ);
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == POSIX_ਬਹੁਲਕ
               && ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਖੇਤਰ == ਮਾਪ_ਖੇਤਰ)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_ਖੰਡ_ਮਾਪ, ਸਿਰਲੇਖ);
        }
      ਨਹੀਂਤਾਂ
        cell = xstrdup (ਸਿਰਲੇਖ);

      replace_problematic_chars (cell);

      ਸਾਰਣੀ.ਪੰਕਤੀਆਂ[ਸਾਰਣੀ.nrows - 1][ਖਾਨਾ] = cell;

      ਪੂਰਨ cell_ਚੌੜਾਈ = mbswidth (cell, MBSWIDTH_FLAGS);
      ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਚੌੜਾਈ = MAX (ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਚੌੜਾਈ, cell_ਚੌੜਾਈ);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
selected_fstype (ਅੱਖਰ ਅਚਲ *fstype)
{
  ਜੇ (fs_select_list == ਨਿਰਾ || fstype == ਨਿਰਾ)
    ਵਾਪਸ true;
  ਲਈ (ਅਚਲ ਬਣਤਰ fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_ਅਗਲਾ)
    ਜੇ (streq (fstype, fsp->fs_name))
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
excluded_fstype (ਅੱਖਰ ਅਚਲ *fstype)
{
  ਜੇ (fs_exclude_list == ਨਿਰਾ || fstype == ਨਿਰਾ)
    ਵਾਪਸ false;
  ਲਈ (ਅਚਲ ਬਣਤਰ fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_ਅਗਲਾ)
    ਜੇ (streq (fstype, fsp->fs_name))
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

ਸਥਿਰ size_t
devlist_hash (ਖਾਲੀ ਅਚਲ *x, size_t ਸਾਰਣੀ_ਮਾਪ)
{
  ਬਣਤਰ devlist ਅਚਲ *p = x;
  ਵਾਪਸ (uintmax_t) {p->dev_ਅੰਕ} % ਸਾਰਣੀ_ਮਾਪ;
}

ਸਥਿਰ bool
devlist_compare (ਖਾਲੀ ਅਚਲ *x, ਖਾਲੀ ਅਚਲ *y)
{
  ਬਣਤਰ devlist ਅਚਲ *a = x;
  ਬਣਤਰ devlist ਅਚਲ *b = y;
  ਵਾਪਸ a->dev_ਅੰਕ == b->dev_ਅੰਕ;
}

ਸਥਿਰ ਬਣਤਰ devlist *
devlist_for_dev (dev_t dev)
{
  ਜੇ (devlist_ਸਾਰਣੀ == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;
  ਬਣਤਰ devlist dev_ਇੰਦਰਾਜ;
  dev_ਇੰਦਰਾਜ.dev_ਅੰਕ = dev;

  ਬਣਤਰ devlist *ਮਿਲਿਆ = hash_lookup (devlist_ਸਾਰਣੀ, &dev_ਇੰਦਰਾਜ);
  ਜੇ (ਮਿਲਿਆ == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  /* Return the last devlist entry we have seen with this dev_num */
  ਵਾਪਸ ਮਿਲਿਆ->ਵੇਖਿਆ_ਆਖਰੀ;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ਸਥਿਰ ਖਾਲੀ
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ਬਣਤਰ devlist *device_list = ਨਿਰਾ;
  ਪੂਰਨ mount_list_ਮਾਪ = 0;

  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me; me = me->me_ਅਗਲਾ)
    mount_list_ਮਾਪ++;

  devlist_ਸਾਰਣੀ = hash_initialize (mount_list_ਮਾਪ, ਨਿਰਾ,
                                   devlist_hash, devlist_compare, ਨਿਰਾ);
  ਜੇ (devlist_ਸਾਰਣੀ == ਨਿਰਾ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me;)
    {
      ਬਣਤਰ stat buf;
      ਬਣਤਰ mount_ਇੰਦਰਾਜ *discard_me = ਨਿਰਾ;
      ਬਣਤਰ devlist *ਆਖਰੀ_ਵੇਖਿਆ_dev = ਨਿਰਾ, *ਵੇਖਿਆ_dev = ਨਿਰਾ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ਜੇ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ਨਹੀਂਤਾਂ
        {
          /* If we've already seen this device...  */
          ਆਖਰੀ_ਵੇਖਿਆ_dev = ਵੇਖਿਆ_dev = devlist_for_dev (buf.st_dev);

          ਲਈ (; ਵੇਖਿਆ_dev && ! discard_me; ਵੇਖਿਆ_dev = ਵੇਖਿਆ_dev->ਅਗਲਾ_same_dev)
            {
              bool ਨਿਸ਼ਾਨਾ_nearer_ਜੜ੍ਹ = ਲੜੀ_ਲੰਬਾਈ (ਵੇਖਿਆ_dev->me->me_mountdir)
                                        > ਲੜੀ_ਲੰਬਾਈ (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool ਸਰੋਤ_ਹੇਠਾਂ_ਜੜ੍ਹ = ਵੇਖਿਆ_dev->me->me_mntroot != ਨਿਰਾ
                                       && me->me_mntroot != ਨਿਰਾ
                                       && (ਲੜੀ_ਲੰਬਾਈ (ਵੇਖਿਆ_dev->me->me_mntroot)
                                           < ਲੜੀ_ਲੰਬਾਈ (me->me_mntroot));
              ਜੇ (! print_grand_ਕੁੱਲ
                  && me->me_remote && ਵੇਖਿਆ_dev->me->me_remote
                  && ! streq (ਵੇਖਿਆ_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ਨਹੀਂਤਾਂ ਜੇ ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (ਵੇਖਿਆ_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (ਨਿਸ਼ਾਨਾ_nearer_ਜੜ੍ਹ && ! ਸਰੋਤ_ਹੇਠਾਂ_ਜੜ੍ਹ)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (ਵੇਖਿਆ_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     ਵੇਖਿਆ_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = ਵੇਖਿਆ_dev->me;
                  ਵੇਖਿਆ_dev->me = me;
                }
              ਨਹੀਂਤਾਂ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ਜੇ (discard_me)
        {
          me = me->me_ਅਗਲਾ;
          ਜੇ (! devices_only)
            free_mount_ਇੰਦਰਾਜ (discard_me);
        }
      ਨਹੀਂਤਾਂ
        {
          /* Add the device number to the device_table.  */
          ਬਣਤਰ devlist *devlist = xmalloc (ਆਕਾਰ *devlist);
          devlist->me = me;
          devlist->dev_ਅੰਕ = buf.st_dev;
          devlist->ਅਗਲਾ_same_dev = ਆਖਰੀ_ਵੇਖਿਆ_dev;
          devlist->ਅਗਲਾ = device_list;
          device_list = devlist;

          ਬਣਤਰ devlist *hash_ਇੰਦਰਾਜ = hash_ਪਾਓ (devlist_ਸਾਰਣੀ, devlist);
          ਜੇ (hash_ਇੰਦਰਾਜ == ਨਿਰਾ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_ਇੰਦਰਾਜ->ਵੇਖਿਆ_ਆਖਰੀ = devlist;

          me = me->me_ਅਗਲਾ;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ਜੇ (! devices_only)
    {
      mount_list = ਨਿਰਾ;
      ਜਦੋਂ (device_list)
        {
          /* Add the mount entry.  */
          ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = device_list->me;
          me->me_ਅਗਲਾ = mount_list;
          mount_list = me;
          ਬਣਤਰ devlist *ਅਗਲਾ = device_list->ਅਗਲਾ;
          ਮੁਕਤ (device_list);
          device_list = ਅਗਲਾ;
        }
      hash_free (devlist_ਸਾਰਣੀ);
      devlist_ਸਾਰਣੀ = ਨਿਰਾ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *
me_for_dev (dev_t dev)
{
  ਬਣਤਰ devlist *dl = devlist_for_dev (dev);
  ਜੇ (dl)
        ਵਾਪਸ dl->me;

  ਵਾਪਸ ਨਿਰਾ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ਸਥਿਰ bool
ਗਿਆਤ_ਮੁੱਲ (uintmax_t n)
{
  ਵਾਪਸ n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
df_readable (bool negative, uintmax_t n, ਅੱਖਰ *buf,
             uintmax_t ਨਿਵੇਸ਼_units, uintmax_t output_units)
{
  ਜੇ (! ਗਿਆਤ_ਮੁੱਲ (n) && !negative)
    ਵਾਪਸ "-";
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ਨਿਵੇਸ਼_units, output_units);
      ਜੇ (negative)
        *--p = '-';
      ਵਾਪਸ p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ਜੇ (*dest_neg == src_neg)
    {
      *dest += src;
      ਵਾਪਸ;
    }

  ਜੇ (*dest_neg)
    *dest = -*dest;

  ਜੇ (src_neg)
    src = -src;

  ਜੇ (src < *dest)
    *dest -= src;
  ਨਹੀਂਤਾਂ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ਜੇ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
has_uuid_suffix (ਅੱਖਰ ਅਚਲ *s)
{
  size_t len = ਲੜੀ_ਲੰਬਾਈ (s);
  ਵਾਪਸ (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ਸਥਿਰ ਖਾਲੀ
get_ਖੇਤਰ_ਮੁੱਲਾਂ (ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t *bv,
                  ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t *iv,
                  ਅਚਲ ਬਣਤਰ fs_usage *fsu)
{
  /* Inode values.  */
  iv->ਨਿਵੇਸ਼_units = iv->output_units = 1;
  iv->ਕੁੱਲ = fsu->fsu_files;
  iv->ਉਪਲਬਧ = iv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ = fsu->fsu_ffree;
  iv->negate_ਉਪਲਬਧ = false;

  iv->ਵਰਤਿਆ = UINTMAX_MAX;
  iv->negate_ਵਰਤਿਆ = false;
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (iv->ਕੁੱਲ) && ਗਿਆਤ_ਮੁੱਲ (iv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ))
    {
      iv->ਵਰਤਿਆ = iv->ਕੁੱਲ - iv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ;
      iv->negate_ਵਰਤਿਆ = (iv->ਕੁੱਲ < iv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ);
    }

  /* Block values.  */
  bv->ਨਿਵੇਸ਼_units = fsu->fsu_blocksize;
  bv->output_units = output_ਖੰਡ_ਮਾਪ;
  bv->ਕੁੱਲ = fsu->fsu_ਖੰਡਾਂ;
  bv->ਉਪਲਬਧ = fsu->fsu_bavail;
  bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ = fsu->fsu_bfree;
  bv->negate_ਉਪਲਬਧ = (fsu->fsu_bavail_ਸਿਖਰ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ
                         && ਗਿਆਤ_ਮੁੱਲ (fsu->fsu_bavail));

  bv->ਵਰਤਿਆ = UINTMAX_MAX;
  bv->negate_ਵਰਤਿਆ = false;
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (bv->ਕੁੱਲ) && ਗਿਆਤ_ਮੁੱਲ (bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ))
    {
      bv->ਵਰਤਿਆ = bv->ਕੁੱਲ - bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ;
      bv->negate_ਵਰਤਿਆ = (bv->ਕੁੱਲ < bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ);
    }
}

/* Add block and inode values to grand total.  */
ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਮੰਜ਼ਿਲ_grand_ਕੁੱਲ (ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t *bv, ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t *iv)
{
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (iv->ਕੁੱਲ))
    grand_fsu.fsu_files += iv->ਕੁੱਲ;
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (iv->ਉਪਲਬਧ))
    grand_fsu.fsu_ffree += iv->ਉਪਲਬਧ;

  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (bv->ਕੁੱਲ))
    grand_fsu.fsu_ਖੰਡਾਂ += bv->ਨਿਵੇਸ਼_units * bv->ਕੁੱਲ;
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ))
    grand_fsu.fsu_bfree += bv->ਨਿਵੇਸ਼_units * bv->ਉਪਲਬਧ_ਮੰਜ਼ਿਲ_ਜੜ੍ਹ;
  ਜੇ (ਗਿਆਤ_ਮੁੱਲ (bv->ਉਪਲਬਧ))
    ਜੋੜੋ_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_ਸਿਖਰ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ,
                            bv->ਨਿਵੇਸ਼_units * bv->ਉਪਲਬਧ,
                            bv->negate_ਉਪਲਬਧ);
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

ਸਥਿਰ ਖਾਲੀ
get_dev (ਅੱਖਰ ਅਚਲ *device, ਅੱਖਰ ਅਚਲ *mount_ਬਿੰਦੂ, ਅੱਖਰ ਅਚਲ *file,
         ਅੱਖਰ ਅਚਲ *stat_file, ਅੱਖਰ ਅਚਲ *fstype,
         bool me_dummy, bool me_remote,
         ਅਚਲ ਬਣਤਰ fs_usage *force_fsu,
         bool process_all)
{
  ਜੇ (me_remote && show_local_fs)
    ਵਾਪਸ;

  ਜੇ (me_dummy && !show_all_fs && !show_listed_fs)
    ਵਾਪਸ;

  ਜੇ (!selected_fstype (fstype) || excluded_fstype (fstype))
    ਵਾਪਸ;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ਜੇ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_ਬਿੰਦੂ))
    ਵਾਪਸ;

  ਜੇ (!stat_file)
    stat_file = mount_ਬਿੰਦੂ;

  ਬਣਤਰ fs_usage fsu;
  ਜੇ (force_fsu)
    fsu = *force_fsu;
  ਨਹੀਂਤਾਂ ਜੇ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ਜੇ (process_all && (errno == EACCES || errno == ENOENT))
        {
          ਜੇ (! show_all_fs)
            ਵਾਪਸ;

          fstype = "-";
          fsu.fsu_bavail_ਸਿਖਰ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ = false;
          fsu.fsu_blocksize = fsu.fsu_ਖੰਡਾਂ = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ਨਹੀਂਤਾਂ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
          ਵਾਪਸ;
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ਬਣਤਰ stat sb;
      ਜੇ (stat (stat_file, &sb) == 0)
        {
          ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ * dev_me = me_for_dev (sb.st_dev);
          ਜੇ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_ਸਿਖਰ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ = false;
              fsu.fsu_blocksize = fsu.fsu_ਖੰਡਾਂ = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ਜੇ (fsu.fsu_ਖੰਡਾਂ == 0 && !show_all_fs && !show_listed_fs)
    ਵਾਪਸ;

  ਜੇ (! force_fsu)
    file_systems_processed = true;

  alloc_ਸਾਰਣੀ_ਪੰਕਤੀ ();

  ਜੇ (! device)
    device = "-";		/* unknown */

  ਜੇ (! file)
    file = "-";			/* unspecified */

  ਅੱਖਰ *dev_name = xstrdup (device);
  ਅੱਖਰ *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ਜੇ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_ਬਹੁਲਕ (dev_name, CAN_EXISTING)))
    {
      ਮੁਕਤ (dev_name);
      dev_name = resolved_dev;
    }

  ਜੇ (! fstype)
    fstype = "-";		/* unknown */

  ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t ਖੰਡ_ਮੁੱਲਾਂ;
  ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t inode_ਮੁੱਲਾਂ;
  get_ਖੇਤਰ_ਮੁੱਲਾਂ (&ਖੰਡ_ਮੁੱਲਾਂ, &inode_ਮੁੱਲਾਂ, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ਜੇ (print_grand_ਕੁੱਲ && ! force_fsu)
    ਜੋੜੋ_ਮੰਜ਼ਿਲ_grand_ਕੁੱਲ (&ਖੰਡ_ਮੁੱਲਾਂ, &inode_ਮੁੱਲਾਂ);

  ਲਈ (idx_t ਖਾਨਾ = 0; ਖਾਨਾ < ਸਾਰਣੀ.ncolumns; ਖਾਨਾ++)
    {
      ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 2];
      ਅੱਖਰ *cell;

      ਬਣਤਰ ਖੇਤਰ_ਮੁੱਲਾਂ_t *v;
      ਚੋਣ (ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਖੇਤਰ_type)
        {
        ਹਾਲਤ ਖੰਡ_FLD:
          v = &ਖੰਡ_ਮੁੱਲਾਂ;
          ਰੋਕੋ;
        ਹਾਲਤ INODE_FLD:
          v = &inode_ਮੁੱਲਾਂ;
          ਰੋਕੋ;
        ਹਾਲਤ OTHER_FLD:
          v = ਨਿਰਾ;
          ਰੋਕੋ;
        ਮੂਲ:
          affirm (!"bad field_type");
        }

      ਚੋਣ (ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਖੇਤਰ)
        {
        ਹਾਲਤ ਸਰੋਤ_ਖੇਤਰ:
          cell = xstrdup (dev_name);
          ਰੋਕੋ;

        ਹਾਲਤ FSTYPE_ਖੇਤਰ:
          cell = xstrdup (fstype);
          ਰੋਕੋ;

        ਹਾਲਤ ਮਾਪ_ਖੇਤਰ:
        ਹਾਲਤ ITOTAL_ਖੇਤਰ:
          cell = xstrdup (df_readable (false, v->ਕੁੱਲ, buf,
                                       v->ਨਿਵੇਸ਼_units, v->output_units));
          ਰੋਕੋ;

        ਹਾਲਤ ਵਰਤਿਆ_ਖੇਤਰ:
        ਹਾਲਤ IUSED_ਖੇਤਰ:
          cell = xstrdup (df_readable (v->negate_ਵਰਤਿਆ, v->ਵਰਤਿਆ, buf,
                                       v->ਨਿਵੇਸ਼_units, v->output_units));
          ਰੋਕੋ;

        ਹਾਲਤ AVAIL_ਖੇਤਰ:
        ਹਾਲਤ IAVAIL_ਖੇਤਰ:
          cell = xstrdup (df_readable (v->negate_ਉਪਲਬਧ, v->ਉਪਲਬਧ, buf,
                                       v->ਨਿਵੇਸ਼_units, v->output_units));
          ਰੋਕੋ;

        ਹਾਲਤ PCENT_ਖੇਤਰ:
        ਹਾਲਤ IPCENT_ਖੇਤਰ:
          {
            ਦੁੱਗਣਾ pct = -1;
            ਜੇ (! ਗਿਆਤ_ਮੁੱਲ (v->ਵਰਤਿਆ) || ! ਗਿਆਤ_ਮੁੱਲ (v->ਉਪਲਬਧ))
              ;
            ਨਹੀਂਤਾਂ ਜੇ (!v->negate_ਵਰਤਿਆ
                     && v->ਵਰਤਿਆ <= UINTMAX_MAX / 100
                     && v->ਵਰਤਿਆ + v->ਉਪਲਬਧ != 0
                     && (v->ਵਰਤਿਆ + v->ਉਪਲਬਧ < v->ਵਰਤਿਆ)
                     == v->negate_ਉਪਲਬਧ)
              {
                uintmax_t u100 = v->ਵਰਤਿਆ * 100;
                uintmax_t nonroot_ਕੁੱਲ = v->ਵਰਤਿਆ + v->ਉਪਲਬਧ;
                pct = u100 / nonroot_ਕੁੱਲ + (u100 % nonroot_ਕੁੱਲ != 0);
              }
            ਨਹੀਂਤਾਂ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ਦੁੱਗਣਾ u = v->negate_ਵਰਤਿਆ ? - (ਦੁੱਗਣਾ) - v->ਵਰਤਿਆ : v->ਵਰਤਿਆ;
                ਦੁੱਗਣਾ a = v->negate_ਉਪਲਬਧ
                           ? - (ਦੁੱਗਣਾ) - v->ਉਪਲਬਧ : v->ਉਪਲਬਧ;
                ਦੁੱਗਣਾ nonroot_ਕੁੱਲ = u + a;
                ਜੇ (nonroot_ਕੁੱਲ)
                  {
                    ਲੰਮਾ ਪੂਰਨ lipct = pct = u * 100 / nonroot_ਕੁੱਲ;
                    ਦੁੱਗਣਾ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ਜੇ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ਰੋਕੋ;
          }

        ਹਾਲਤ FILE_ਖੇਤਰ:
          cell = xstrdup (file);
          ਰੋਕੋ;

        ਹਾਲਤ ਨਿਸ਼ਾਨਾ_ਖੇਤਰ:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HIDE_AUTOMOUNT_ਅਗੇਤਰ
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ਜੇ (STRNCMP_LIT (mount_ਬਿੰਦੂ, "/auto/") == 0)
            mount_ਬਿੰਦੂ += 5;
          ਨਹੀਂਤਾਂ ਜੇ (STRNCMP_LIT (mount_ਬਿੰਦੂ, "/tmp_mnt/") == 0)
            mount_ਬਿੰਦੂ += 8;
#ਜੇ_ਅੰਤ
          cell = xstrdup (mount_ਬਿੰਦੂ);
          ਰੋਕੋ;

        ਹਾਲਤ INVALID_ਖੇਤਰ:
        ਮੂਲ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      ਪੂਰਨ cell_ਚੌੜਾਈ = mbswidth (cell, MBSWIDTH_FLAGS);
      ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਚੌੜਾਈ = MAX (ਸਾਰਣੀ.columns[ਖਾਨਾ]->ਚੌੜਾਈ, cell_ਚੌੜਾਈ);
      ਸਾਰਣੀ.ਪੰਕਤੀਆਂ[ਸਾਰਣੀ.nrows - 1][ਖਾਨਾ] = cell;
    }
  ਮੁਕਤ (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ਸਥਿਰ ਅੱਖਰ *
ਆਖਰੀ_device_for_mount (ਅੱਖਰ ਅਚਲ *mount)
{
  ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *le = ਨਿਰਾ;

  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *me = mount_list; me; me = me->me_ਅਗਲਾ)
    {
      ਜੇ (streq (me->me_mountdir, mount))
        le = me;
    }

  ਜੇ (le)
    {
      ਅੱਖਰ *devname = le->me_devname;
      ਅੱਖਰ *canon_dev = canonicalize_file_name (devname);
      ਜੇ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        ਵਾਪਸ canon_dev;
      ਮੁਕਤ (canon_dev);
      ਵਾਪਸ xstrdup (le->me_devname);
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਨਿਰਾ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ਸਥਿਰ bool
get_device (ਅੱਖਰ ਅਚਲ *device)
{
  ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *ਉੱਤਮ_ਮੇਲ = ਨਿਰਾ;
  bool ਉੱਤਮ_ਮੇਲ_accessible = false;
  bool eclipsed_device = false;
  ਅੱਖਰ ਅਚਲ *file = device;

  ਅੱਖਰ *resolved = canonicalize_file_name (device);
  ਜੇ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t ਉੱਤਮ_ਮੇਲ_len = SIZE_MAX;
  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *me = mount_list; me; me = me->me_ਅਗਲਾ)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      ਅੱਖਰ *devname = me->me_devname;
      ਅੱਖਰ *canon_dev = canonicalize_file_name (me->me_devname);
      ਜੇ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ਜੇ (streq (device, devname))
        {
          ਅੱਖਰ *ਆਖਰੀ_device = ਆਖਰੀ_device_for_mount (me->me_mountdir);
          eclipsed_device = ਆਖਰੀ_device && ! streq (ਆਖਰੀ_device, devname);
          size_t len = ਲੜੀ_ਲੰਬਾਈ (me->me_mountdir);

          ਜੇ (! eclipsed_device
              && (! ਉੱਤਮ_ਮੇਲ_accessible || len < ਉੱਤਮ_ਮੇਲ_len))
            {
              ਬਣਤਰ stat device_stats;
              bool this_ਮੇਲ_accessible = false;

              ਜੇ (stat (me->me_mountdir, &device_stats) == 0)
                ਉੱਤਮ_ਮੇਲ_accessible = this_ਮੇਲ_accessible = true;

              ਜੇ (this_ਮੇਲ_accessible
                  || (! ਉੱਤਮ_ਮੇਲ_accessible && len < ਉੱਤਮ_ਮੇਲ_len))
                {
                  ਉੱਤਮ_ਮੇਲ = me;
                  ਜੇ (len == 1) /* Traditional root.  */
                    {
                      ਮੁਕਤ (ਆਖਰੀ_device);
                      ਮੁਕਤ (canon_dev);
                      ਰੋਕੋ;
                    }
                  ਨਹੀਂਤਾਂ
                    ਉੱਤਮ_ਮੇਲ_len = len;
                }
            }

          ਮੁਕਤ (ਆਖਰੀ_device);
        }

      ਮੁਕਤ (canon_dev);
    }

  ਮੁਕਤ (resolved);

  ਜੇ (ਉੱਤਮ_ਮੇਲ)
    {
      get_dev (ਉੱਤਮ_ਮੇਲ->me_devname, ਉੱਤਮ_ਮੇਲ->me_mountdir, file, ਨਿਰਾ,
               ਉੱਤਮ_ਮੇਲ->me_type, ਉੱਤਮ_ਮੇਲ->me_dummy,
               ਉੱਤਮ_ਮੇਲ->me_remote, ਨਿਰਾ, false);
      ਵਾਪਸ true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_ਸਥਿਤੀ = EXIT_FAILURE;
      ਵਾਪਸ true;
    }

  ਵਾਪਸ false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ਸਥਿਰ ਖਾਲੀ
get_ਬਿੰਦੂ (ਅੱਖਰ ਅਚਲ *ਬਿੰਦੂ, ਅਚਲ ਬਣਤਰ stat *statp)
{
  ਬਣਤਰ stat device_stats;
  ਬਣਤਰ mount_ਇੰਦਰਾਜ ਅਚਲ *ਉੱਤਮ_ਮੇਲ = ਨਿਰਾ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  ਅੱਖਰ *resolved = canonicalize_file_name (ਬਿੰਦੂ);
  ਜੇ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = ਲੜੀ_ਲੰਬਾਈ (resolved);
      size_t ਉੱਤਮ_ਮੇਲ_len = 0;

      ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me; me = me->me_ਅਗਲਾ)
        {
          ਜੇ (!streq (me->me_type, "lofs")
              && (!ਉੱਤਮ_ਮੇਲ || ਉੱਤਮ_ਮੇਲ->me_dummy || !me->me_dummy))
            {
              size_t len = ਲੜੀ_ਲੰਬਾਈ (me->me_mountdir);
              ਜੇ (ਉੱਤਮ_ਮੇਲ_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  ਉੱਤਮ_ਮੇਲ = me;
                  ਉੱਤਮ_ਮੇਲ_len = len;
                }
            }
        }
    }
  ਮੁਕਤ (resolved);
  ਜੇ (ਉੱਤਮ_ਮੇਲ
      && (stat (ਉੱਤਮ_ਮੇਲ->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    ਉੱਤਮ_ਮੇਲ = ਨਿਰਾ;

  ਜੇ (! ਉੱਤਮ_ਮੇਲ)
    ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me; me = me->me_ਅਗਲਾ)
      {
        ਜੇ (me->me_dev == (dev_t) -1)
          {
            ਜੇ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ਨਹੀਂਤਾਂ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ਜੇ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_ਸਥਿਤੀ = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ਜੇ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!ਉੱਤਮ_ਮੇਲ || ਉੱਤਮ_ਮੇਲ->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ਜੇ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ਨਹੀਂਤਾਂ
              ਉੱਤਮ_ਮੇਲ = me;
          }
      }

  ਜੇ (ਉੱਤਮ_ਮੇਲ)
    get_dev (ਉੱਤਮ_ਮੇਲ->me_devname, ਉੱਤਮ_ਮੇਲ->me_mountdir, ਬਿੰਦੂ, ਬਿੰਦੂ,
             ਉੱਤਮ_ਮੇਲ->me_type, ਉੱਤਮ_ਮੇਲ->me_dummy, ਉੱਤਮ_ਮੇਲ->me_remote,
             ਨਿਰਾ, false);
  ਨਹੀਂਤਾਂ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      ਅੱਖਰ *mp = ਲੱਭੋ_mount_ਬਿੰਦੂ (ਬਿੰਦੂ, statp);
      ਜੇ (mp)
        {
          get_dev (ਨਿਰਾ, mp, ਬਿੰਦੂ, ਨਿਰਾ, ਨਿਰਾ,
                   false, false, ਨਿਰਾ, false);
          ਮੁਕਤ (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ਸਥਿਰ ਖਾਲੀ
get_ਇੰਦਰਾਜ (ਅੱਖਰ ਅਚਲ *name, ਬਣਤਰ stat ਅਚਲ *statp)
{
  ਜੇ ((S_ISBLK (statp->st_ਬਹੁਲਕ) || S_ISCHR (statp->st_ਬਹੁਲਕ))
      && get_device (name))
    ਵਾਪਸ;

  get_ਬਿੰਦੂ (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ਸਥਿਰ ਖਾਲੀ
get_all_entries (ਖਾਲੀ)
{
  filter_mount_list (show_all_fs);

  ਲਈ (ਬਣਤਰ mount_ਇੰਦਰਾਜ *me = mount_list; me; me = me->me_ਅਗਲਾ)
    get_dev (me->me_devname, me->me_mountdir, ਨਿਰਾ, ਨਿਰਾ, me->me_type,
             me->me_dummy, me->me_remote, ਨਿਰਾ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_fs_type (ਅੱਖਰ ਅਚਲ *fstype)
{
  ਬਣਤਰ fs_type_list *fsp;

  fsp = xmalloc (ਆਕਾਰ *fsp);
  fsp->fs_name = (ਅੱਖਰ *) fstype;
  fsp->fs_ਅਗਲਾ = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_excluded_fs_type (ਅੱਖਰ ਅਚਲ *fstype)
{
  ਬਣਤਰ fs_type_list *fsp;

  fsp = xmalloc (ਆਕਾਰ *fsp);
  fsp->fs_name = (ਅੱਖਰ *) fstype;
  fsp->fs_ਅਗਲਾ = fs_exclude_list;
  fs_exclude_list = fsp;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by ਮੂਲ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --ਖੰਡ-ਮਾਪ=ਮਾਪ_2\n\
         scale sizes by ਮਾਪ_2 before printing them; see ਮਾਪ_2 format ਹੇਠਾਂ;\n\
         E.g., '-BM' prints sizes ਵਿੱਚ units ਦਾ 1,048,576 ਬਾਈਟਾਂ\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes ਵਿੱਚ powers ਦਾ 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes ਵਿੱਚ powers ਦਾ 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead ਦਾ ਖੰਡ usage\n\
"));
      oputs (_("\
  -k\n\
         like --ਖੰਡ-ਮਾਪ=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         ਹੱਦਬੰਦੀ listing ਮੰਜ਼ਿਲ local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         ਕਰੋ not invoke sync before getting usage info (ਮੂਲ)\n\
"));
      oputs (_("\
      --output[=ਖੇਤਰ_LIST]\n\
         use the output format defined by ਖੇਤਰ_LIST,\n\
         or print all fields ਜੇ ਖੇਤਰ_LIST is omitted\n\
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
      --ਕੁੱਲ\n\
         elide all entries insignificant ਮੰਜ਼ਿਲ ਉਪਲਬਧ space,\n\
         and produce a grand ਕੁੱਲ\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         ਹੱਦਬੰਦੀ listing ਮੰਜ਼ਿਲ file systems ਦਾ type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         ਹੱਦਬੰਦੀ listing ਮੰਜ਼ਿਲ file systems not ਦਾ type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_ਮਾਪ_note ();
      fputs (_("\n\
ਖੇਤਰ_LIST is a comma-separated list ਦਾ columns ਮੰਜ਼ਿਲ be included.  Valid\n\
ਖੇਤਰ ਨਾਂ are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info ਪੰਨਾ).\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਣਤਰ stat *stats = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_mut_excl = _("options %s and %s are mutually exclusive");

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ oi = -1;
      ਪੂਰਨ c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 'a':
          show_all_fs = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'B':
          {
            ਗਿਣਤੀ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ਖੰਡ_ਮਾਪ);
            ਜੇ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == OUTPUT_ਬਹੁਲਕ)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ਸਿਰਲੇਖ_ਬਹੁਲਕ = INODES_ਬਹੁਲਕ;
          ਰੋਕੋ;
        ਹਾਲਤ 'h':
          human_output_opts = human_autoscale | human_SI | human_ਅਧਾਰ_1024;
          output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;
        ਹਾਲਤ 'H':
          human_output_opts = human_autoscale | human_SI;
          output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;
        ਹਾਲਤ 'k':
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1024;
          ਰੋਕੋ;
        ਹਾਲਤ 'l':
          show_local_fs = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1024 * 1024;
          ਰੋਕੋ;
        ਹਾਲਤ 'T':
          ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == OUTPUT_ਬਹੁਲਕ)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'P':
          ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == OUTPUT_ਬਹੁਲਕ)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ਰੋਕੋ;
        ਹਾਲਤ SYNC_OPTION:
          require_sync = true;
          ਰੋਕੋ;
        ਹਾਲਤ NO_SYNC_OPTION:
          require_sync = false;
          ਰੋਕੋ;

        ਹਾਲਤ 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        ਹਾਲਤ 't':
          ਜੋੜੋ_fs_type (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'v':		/* For SysV compatibility.  */
          /* ignore */
          ਰੋਕੋ;
        ਹਾਲਤ 'x':
          ਜੋੜੋ_excluded_fs_type (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ OUTPUT_OPTION:
          ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == INODES_ਬਹੁਲਕ)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ਜੇ (posix_format && ਸਿਰਲੇਖ_ਬਹੁਲਕ == DEFAULT_ਬਹੁਲਕ)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ਜੇ (print_type)
            {
              error (0, 0, ਸੁਨੇਹਾ_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          ਸਿਰਲੇਖ_ਬਹੁਲਕ = OUTPUT_ਬਹੁਲਕ;
          ਜੇ (optarg)
            decode_output_arg (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ ਕੁੱਲ_OPTION:
          print_grand_ਕੁੱਲ = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (human_output_opts == -1)
    {
      ਜੇ (posix_format)
        {
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ਨਹੀਂਤਾਂ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_ਖੰਡ_ਮਾਪ);
    }

  ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == INODES_ਬਹੁਲਕ || ਸਿਰਲੇਖ_ਬਹੁਲਕ == OUTPUT_ਬਹੁਲਕ)
    ;
  ਨਹੀਂਤਾਂ ਜੇ (human_output_opts & human_autoscale)
    ਸਿਰਲੇਖ_ਬਹੁਲਕ = HUMAN_ਬਹੁਲਕ;
  ਨਹੀਂਤਾਂ ਜੇ (posix_format)
    ਸਿਰਲੇਖ_ਬਹੁਲਕ = POSIX_ਬਹੁਲਕ;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool ਮੇਲ = false;
    ਲਈ (ਬਣਤਰ fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_ਅਗਲਾ)
      {
        ਲਈ (ਬਣਤਰ fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_ਅਗਲਾ)
          {
            ਜੇ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                ਮੇਲ = true;
                ਰੋਕੋ;
              }
          }
      }
    ਜੇ (ਮੇਲ)
      ਵਾਪਸ EXIT_FAILURE;
  }

  ਜੇ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, ਆਕਾਰ *stats);
      ਲਈ (ਪੂਰਨ i = optind; i < argc; ++i)
        {
          ਪੂਰਨ err = automount_stat_err (argv[i], &stats[i - optind]);
          ਜੇ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_ਸਥਿਤੀ = EXIT_FAILURE;
              argv[i] = ਨਿਰਾ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ਨਿਰਾ
                            || fs_exclude_list != ਨਿਰਾ
                            || print_type
                            || ਖੇਤਰ_ਅੰਕੜੇ_2[FSTYPE_ਖੇਤਰ].ਵਰਤਿਆ
                            || show_local_fs));

  ਜੇ (mount_list == ਨਿਰਾ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      ਪੂਰਨ ਸਥਿਤੀ = 0;
      ਜੇ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ਨਿਰਾ
               || fs_exclude_list != ਨਿਰਾ))
        {
          ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਅੱਖਰ ਅਚਲ *warning = (ਸਥਿਤੀ == 0 ? _("Warning: ") : "");
      error (ਸਥਿਤੀ, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ਜੇ (require_sync)
    sync ();

  get_ਖੇਤਰ_list ();
  get_ਸਿਰਲੇਖ ();

  ਜੇ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      ਲਈ (ਪੂਰਨ i = optind; i < argc; ++i)
        ਜੇ (argv[i])
          get_ਇੰਦਰਾਜ (argv[i], &stats[i - optind]);
    }
  ਨਹੀਂਤਾਂ
    get_all_entries ();

  ਜੇ (file_systems_processed)
    {
      ਜੇ (print_grand_ਕੁੱਲ)
        get_dev ("total",
                 (ਖੇਤਰ_ਅੰਕੜੇ_2[ਸਰੋਤ_ਖੇਤਰ].ਵਰਤਿਆ ? "-" : "total"),
                 ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, false, false, &grand_fsu, false);

      print_ਸਾਰਣੀ ();
    }
  ਨਹੀਂਤਾਂ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ਜੇ (exit_ਸਥਿਤੀ == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_ਸਥਿਤੀ);
}
