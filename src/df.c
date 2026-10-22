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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <getopt.h>
#ضمن <c-ctype.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "canonicalize.h"
#ضمن "fsusage.h"
#ضمن "human.h"
#ضمن "mbswidth.h"
#ضمن "mountlist.h"
#ضمن "quote.h"
#ضمن "find-mount-point.h"
#ضمن "hash.h"
#ضمن "xstrtol-error.h"
#ضمن "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "df"

#تعریف_کړه AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

بنية devlist
{
  dev_t dev_عدد;
  بنية mount_ثبت *me;
  بنية devlist *بل;
  بنية devlist *بل_same_dev;
  بنية devlist *کتل_شوی_وروستی; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ساكن Hash_جدول *devlist_جدول;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ساكن bool show_all_fs;

/* If true, show only local file systems.  */
ساكن bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ساكن bool show_listed_fs;

/* Human-readable options for output.  */
ساكن صحيح human_output_opts = -1;

/* The units to use when printing sizes.  */
ساكن uintmax_t output_بلاک_اندازه;

/* True if a file system has been processed for output.  */
ساكن bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ساكن bool require_sync;

/* Desired exit status.  */
ساكن صحيح exit_حالت;

/* A file system type to display.  */

بنية fs_type_list
{
  حرف *fs_name;
  بنية fs_type_list *fs_بل;
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

ساكن بنية fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ساكن بنية fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ساكن بنية mount_ثبت *mount_list;

/* If true, print file system type as well.  */
ساكن bool print_type;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_ټول;

/* Grand total data.  */
ساكن بنية fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساكن تعداد
{
  DEFAULT_ډېرپېښ,
  INODES_ډېرپېښ,
  HUMAN_ډېرپېښ,
  POSIX_ډېرپېښ,
  OUTPUT_ډېرپېښ
} سرلیک_ډېرپېښ = DEFAULT_ډېرپېښ;

/* Displayable fields.  */
تعريفنوع تعداد
{
  سرچینه_ډګر, /* file system */
  FSTYPE_ډګر, /* FS type */
  اندازه_ډګر,   /* FS size */
  کارول_شوی_ډګر,   /* FS size used  */
  AVAIL_ډګر,  /* FS size available */
  PCENT_ډګر,  /* percent used */
  ITOTAL_ډګر, /* inode total */
  IUSED_ډګر,  /* inodes used */
  IAVAIL_ډګر, /* inodes available */
  IPCENT_ډګر, /* inodes used in percent */
  موخه_ډګر, /* mount point */
  FILE_ډګر,   /* specified file name */
  INVALID_ډګر /* validation marker */
} display_ډګر_t;

/* Flag if a field contains a block, an inode or another value.  */
تعريفنوع تعداد
{
  بلاک_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} ډګر_type_t;

/* Attributes of a display field.  */
بنية ډګر_معلومات_t
{
  display_ډګر_t ډګر;
  حرف ثابت *arg;
  ډګر_type_t ډګر_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحيح پلنوالی;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ښی;   /* Whether to right-align columns, not left-align.  */
  bool کارول_شوی;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساكن بنية ډګر_معلومات_t ډګر_معلومات_2[] = {
  [سرچینه_ډګر] = { سرچینه_ډګر,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_ډګر] = { FSTYPE_ډګر,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [اندازه_ډګر] = { اندازه_ډګر,
    "size",   بلاک_FLD, N_("blocks"),      5, true, false },

  [کارول_شوی_ډګر] = { کارول_شوی_ډګر,
    "used",   بلاک_FLD, N_("Used"),        5, true, false },

  [AVAIL_ډګر] = { AVAIL_ډګر,
    "avail",  بلاک_FLD, N_("Available"),   5, true, false },

  [PCENT_ډګر] = { PCENT_ډګر,
    "pcent",  بلاک_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_ډګر] = { ITOTAL_ډګر,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_ډګر] = { IUSED_ډګر,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_ډګر] = { IAVAIL_ډګر,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_ډګر] = { IPCENT_ډګر,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [موخه_ډګر] = { موخه_ډګر,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_ډګر] = { FILE_ډګر,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ساكن حرف ثابت *all_دلیلونه_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساكن بنية
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***لیکې COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  بنية ډګر_معلومات_t **columns COUNTED_BY (ncolumns);
} جدول;

/* Field values.  */
بنية ډګر_ارزښتونه_t
{
  uintmax_t ننوت_units;
  uintmax_t output_units;
  uintmax_t ټول;
  uintmax_t شته;
  bool negate_شته;
  uintmax_t شته_منزل_ریښه;
  uintmax_t کارول_شوی;
  bool negate_کارول_شوی;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  ټول_OPTION,
  OUTPUT_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_دلیل, عدم, 'a'},
  {"block-size", required_دلیل, عدم, 'B'},
  {"inodes", no_دلیل, عدم, 'i'},
  {"human-readable", no_دلیل, عدم, 'h'},
  {"si", no_دلیل, عدم, 'H'},
  {"local", no_دلیل, عدم, 'l'},
  {"output", optional_دلیل, عدم, OUTPUT_OPTION},
  {"portability", no_دلیل, عدم, 'P'},
  {"print-type", no_دلیل, عدم, 'T'},
  {"sync", no_دلیل, عدم, SYNC_OPTION},
  {"no-sync", no_دلیل, عدم, NO_SYNC_OPTION},
  {"total", no_دلیل, عدم, ټول_OPTION},
  {"type", required_دلیل, عدم, 't'},
  {"exclude-type", required_دلیل, عدم, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ساكن صحيح
automount_stat_err (حرف ثابت *file, بنية stat *st)
{
  صحيح fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  إذا (fd < 0)
    {
      إذا (errno == ENOENT || errno == ENOTDIR)
        أرجع errno;
      أرجع stat (file, st) == 0 ? 0 : errno;
    }
  وإلا
    {
      صحيح err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      أرجع err;
    }
}

تعداد { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ساكن فراغ
replace_control_chars (حرف *cell)
{
  حرف *p = cell;
  طالما (*p)
    {
      إذا (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ساكن فراغ
replace_invalid_chars (حرف *cell)
{
  حرف *srcend = cell + مزي_اوږدوالی (cell);
  حرف *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  لكل (حرف *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      إذا (ok)
        ok = !c32iscntrl (wc);
      وإلا
        n = 1;

      إذا (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      وإلا
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ساكن فراغ
replace_problematic_chars (حرف *cell)
{
  ساكن صحيح tty_بهر = -1;
  إذا (tty_بهر < 0)
    tty_بهر = isatty (STDOUT_FILENO);

  (tty_بهر ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساكن فراغ
alloc_جدول_لیکه (فراغ)
{
  إذا (جدول.nrows == جدول.nrows_alloc)
    جدول.لیکې = xpalloc (جدول.لیکې, &جدول.nrows_alloc, 1, -1,
                          حجم *جدول.لیکې);
  idx_t ثابت لیکه_idx = جدول.nrows++;
  جدول.لیکې[لیکه_idx] = xinmalloc (جدول.ncolumns, حجم *جدول.لیکې[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساكن فراغ
print_جدول (فراغ)
{
  لكل (idx_t لیکه = 0; لیکه < جدول.nrows; لیکه++)
    {
      لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
        {
          حرف *cell = جدول.لیکې[لیکه][ستون];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          إذا (ستون != 0)
            توری_چاپ_کړه (' ');

          صحيح پلنوالی = mbswidth (cell, MBSWIDTH_FLAGS);
          صحيح fill = پلنوالی < 0 ? 0 : جدول.columns[ستون]->پلنوالی - پلنوالی;
          إذا (جدول.columns[ستون]->align_ښی)
            لكل (; 0 < fill; fill--)
              توری_چاپ_کړه (' ');
          fputs (cell, stdout);
          إذا (ستون + 1 < جدول.ncolumns)
            لكل (; 0 < fill; fill--)
              توری_چاپ_کړه (' ');
        }
      توری_چاپ_کړه ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساكن فراغ
alloc_ډګر (صحيح f, حرف ثابت *c)
{
  إذا (جدول.ncolumns == جدول.ncolumns_alloc)
    جدول.columns = xpalloc (جدول.columns, &جدول.ncolumns_alloc, 1, -1,
                             حجم *جدول.columns);
  idx_t ثابت ستنه_idx = جدول.ncolumns++;
  جدول.columns[ستنه_idx] = &ډګر_معلومات_2[f];
  إذا (c != عدم)
    ډګر_معلومات_2[f].caption = c;

  affirm (!ډګر_معلومات_2[f].کارول_شوی);

  /* Mark field as used.  */
  ډګر_معلومات_2[f].کارول_شوی = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ساكن فراغ
decode_output_arg (حرف ثابت *arg)
{
  حرف *arg_writable = xstrdup (arg);
  حرف *s = arg_writable;
  افعل
    {
      /* find next comma */
      حرف *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      إذا (comma)
        *comma++ = 0;

      /* process S.  */
      display_ډګر_t ډګر = INVALID_ډګر;
      لكل (idx_t i = 0; i < countof (ډګر_معلومات_2); i++)
        {
          إذا (streq (ډګر_معلومات_2[i].arg, s))
            {
              ډګر = i;
              اكسر;
            }
        }
      إذا (ډګر == INVALID_ډګر)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      إذا (ډګر_معلومات_2[ډګر].کارول_شوی)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (ډګر_معلومات_2[ډګر].arg));
          usage (EXIT_FAILURE);
        }

      بدل (ډګر)
        {
        حالة سرچینه_ډګر:
        حالة FSTYPE_ډګر:
        حالة کارول_شوی_ډګر:
        حالة PCENT_ډګر:
        حالة ITOTAL_ډګر:
        حالة IUSED_ډګر:
        حالة IAVAIL_ډګر:
        حالة IPCENT_ډګر:
        حالة موخه_ډګر:
        حالة FILE_ډګر:
          alloc_ډګر (ډګر, عدم);
          اكسر;

        حالة اندازه_ډګر:
          alloc_ډګر (ډګر, N_("Size"));
          اكسر;

        حالة AVAIL_ډګر:
          alloc_ډګر (ډګر, N_("Avail"));
          اكسر;

        حالة INVALID_ډګر:
        افتراضي:
          affirm (!"invalid field");
        }
      s = comma;
    }
  طالما (s);

  حرر (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ساكن فراغ
get_ډګر_list (فراغ)
{
  بدل (سرلیک_ډېرپېښ)
    {
    حالة DEFAULT_ډېرپېښ:
      alloc_ډګر (سرچینه_ډګر, عدم);
      إذا (print_type)
        alloc_ډګر (FSTYPE_ډګر, عدم);
      alloc_ډګر (اندازه_ډګر,   عدم);
      alloc_ډګر (کارول_شوی_ډګر,   عدم);
      alloc_ډګر (AVAIL_ډګر,  عدم);
      alloc_ډګر (PCENT_ډګر,  عدم);
      alloc_ډګر (موخه_ډګر, عدم);
      اكسر;

    حالة HUMAN_ډېرپېښ:
      alloc_ډګر (سرچینه_ډګر, عدم);
      إذا (print_type)
        alloc_ډګر (FSTYPE_ډګر, عدم);

      alloc_ډګر (اندازه_ډګر,   N_("Size"));
      alloc_ډګر (کارول_شوی_ډګر,   عدم);
      alloc_ډګر (AVAIL_ډګر,  N_("Avail"));
      alloc_ډګر (PCENT_ډګر,  عدم);
      alloc_ډګر (موخه_ډګر, عدم);
      اكسر;

    حالة INODES_ډېرپېښ:
      alloc_ډګر (سرچینه_ډګر, عدم);
      إذا (print_type)
        alloc_ډګر (FSTYPE_ډګر, عدم);
      alloc_ډګر (ITOTAL_ډګر,  عدم);
      alloc_ډګر (IUSED_ډګر,   عدم);
      alloc_ډګر (IAVAIL_ډګر,  عدم);
      alloc_ډګر (IPCENT_ډګر,  عدم);
      alloc_ډګر (موخه_ډګر,  عدم);
      اكسر;

    حالة POSIX_ډېرپېښ:
      alloc_ډګر (سرچینه_ډګر, عدم);
      إذا (print_type)
        alloc_ډګر (FSTYPE_ډګر, عدم);
      alloc_ډګر (اندازه_ډګر,   عدم);
      alloc_ډګر (کارول_شوی_ډګر,   عدم);
      alloc_ډګر (AVAIL_ډګر,  عدم);
      alloc_ډګر (PCENT_ډګر,  N_("Capacity"));
      alloc_ډګر (موخه_ډګر, عدم);
      اكسر;

    حالة OUTPUT_ډېرپېښ:
      إذا (!جدول.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_دلیلونه_string);
        }
      اكسر;

    افتراضي:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساكن فراغ
get_سرلیک (فراغ)
{
  alloc_جدول_لیکه ();

  لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف *cell;
      حرف ثابت *سرلیک = _(جدول.columns[ستون]->caption);

      إذا (جدول.columns[ستون]->ډګر == اندازه_ډګر
          && (سرلیک_ډېرپېښ == DEFAULT_ډېرپېښ
              || (سرلیک_ډېرپېښ == OUTPUT_ډېرپېښ
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحيح opts = (human_suppress_ټکی_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_ډله_digits | human_بنسټ_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_بلاک_اندازه;
          uintmax_t q1024 = output_بلاک_اندازه;
          bool divisible_by_1000;
          bool divisible_by_1024;

          افعل
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          طالما (divisible_by_1000 & divisible_by_1024);

          إذا (divisible_by_1000 < divisible_by_1024)
            opts |= human_بنسټ_1024;
          إذا (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_بنسټ_1024;
          إذا (! (opts & human_بنسټ_1024))
            opts |= human_B;

          حرف *عدد = human_readable (output_بلاک_اندازه, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          سرلیک = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), عدد, سرلیک);
        }
      وإلا إذا (سرلیک_ډېرپېښ == POSIX_ډېرپېښ
               && جدول.columns[ستون]->ډګر == اندازه_ډګر)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_بلاک_اندازه, سرلیک);
        }
      وإلا
        cell = xstrdup (سرلیک);

      replace_problematic_chars (cell);

      جدول.لیکې[جدول.nrows - 1][ستون] = cell;

      صحيح cell_پلنوالی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->پلنوالی = MAX (جدول.columns[ستون]->پلنوالی, cell_پلنوالی);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_خالص
ساكن bool
selected_fstype (حرف ثابت *fstype)
{
  إذا (fs_select_list == عدم || fstype == عدم)
    أرجع true;
  لكل (ثابت بنية fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_بل)
    إذا (streq (fstype, fsp->fs_name))
      أرجع true;
  أرجع false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_خالص
ساكن bool
excluded_fstype (حرف ثابت *fstype)
{
  إذا (fs_exclude_list == عدم || fstype == عدم)
    أرجع false;
  لكل (ثابت بنية fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_بل)
    إذا (streq (fstype, fsp->fs_name))
      أرجع true;
  أرجع false;
}

ساكن size_t
devlist_hash (فراغ ثابت *x, size_t جدول_اندازه)
{
  بنية devlist ثابت *p = x;
  أرجع (uintmax_t) {p->dev_عدد} % جدول_اندازه;
}

ساكن bool
devlist_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية devlist ثابت *a = x;
  بنية devlist ثابت *b = y;
  أرجع a->dev_عدد == b->dev_عدد;
}

ساكن بنية devlist *
devlist_for_dev (dev_t dev)
{
  إذا (devlist_جدول == عدم)
    أرجع عدم;
  بنية devlist dev_ثبت;
  dev_ثبت.dev_عدد = dev;

  بنية devlist *وموندل = hash_lookup (devlist_جدول, &dev_ثبت);
  إذا (وموندل == عدم)
    أرجع عدم;

  /* Return the last devlist entry we have seen with this dev_num */
  أرجع وموندل->کتل_شوی_وروستی;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ساكن فراغ
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  بنية devlist *device_list = عدم;
  صحيح mount_list_اندازه = 0;

  لكل (بنية mount_ثبت *me = mount_list; me; me = me->me_بل)
    mount_list_اندازه++;

  devlist_جدول = hash_initialize (mount_list_اندازه, عدم,
                                   devlist_hash, devlist_compare, عدم);
  إذا (devlist_جدول == عدم)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  لكل (بنية mount_ثبت *me = mount_list; me;)
    {
      بنية stat buf;
      بنية mount_ثبت *discard_me = عدم;
      بنية devlist *وروستی_کتل_شوی_dev = عدم, *کتل_شوی_dev = عدم;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      إذا ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      وإلا
        {
          /* If we've already seen this device...  */
          وروستی_کتل_شوی_dev = کتل_شوی_dev = devlist_for_dev (buf.st_dev);

          لكل (; کتل_شوی_dev && ! discard_me; کتل_شوی_dev = کتل_شوی_dev->بل_same_dev)
            {
              bool موخه_nearer_ریښه = مزي_اوږدوالی (کتل_شوی_dev->me->me_mountdir)
                                        > مزي_اوږدوالی (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool سرچینه_لاندې_ریښه = کتل_شوی_dev->me->me_mntroot != عدم
                                       && me->me_mntroot != عدم
                                       && (مزي_اوږدوالی (کتل_شوی_dev->me->me_mntroot)
                                           < مزي_اوږدوالی (me->me_mntroot));
              إذا (! print_grand_ټول
                  && me->me_remote && کتل_شوی_dev->me->me_remote
                  && ! streq (کتل_شوی_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              وإلا إذا ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (کتل_شوی_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (موخه_nearer_ریښه && ! سرچینه_لاندې_ریښه)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (کتل_شوی_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     کتل_شوی_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = کتل_شوی_dev->me;
                  کتل_شوی_dev->me = me;
                }
              وإلا
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      إذا (discard_me)
        {
          me = me->me_بل;
          إذا (! devices_only)
            free_mount_ثبت (discard_me);
        }
      وإلا
        {
          /* Add the device number to the device_table.  */
          بنية devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_عدد = buf.st_dev;
          devlist->بل_same_dev = وروستی_کتل_شوی_dev;
          devlist->بل = device_list;
          device_list = devlist;

          بنية devlist *hash_ثبت = hash_ورننباسه (devlist_جدول, devlist);
          إذا (hash_ثبت == عدم)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_ثبت->کتل_شوی_وروستی = devlist;

          me = me->me_بل;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  إذا (! devices_only)
    {
      mount_list = عدم;
      طالما (device_list)
        {
          /* Add the mount entry.  */
          بنية mount_ثبت *me = device_list->me;
          me->me_بل = mount_list;
          mount_list = me;
          بنية devlist *بل = device_list->بل;
          حرر (device_list);
          device_list = بل;
        }
      hash_free (devlist_جدول);
      devlist_جدول = عدم;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_خالص
ساكن بنية mount_ثبت ثابت *
me_for_dev (dev_t dev)
{
  بنية devlist *dl = devlist_for_dev (dev);
  إذا (dl)
        أرجع dl->me;

  أرجع عدم;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ساكن bool
معلوم_ارزښت (uintmax_t n)
{
  أرجع n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ساكن حرف ثابت *
df_readable (bool negative, uintmax_t n, حرف *buf,
             uintmax_t ننوت_units, uintmax_t output_units)
{
  إذا (! معلوم_ارزښت (n) && !negative)
    أرجع "-";
  وإلا
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ننوت_units, output_units);
      إذا (negative)
        *--p = '-';
      أرجع p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ساكن فراغ
جمع_کړه_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  إذا (*dest_neg == src_neg)
    {
      *dest += src;
      أرجع;
    }

  إذا (*dest_neg)
    *dest = -*dest;

  إذا (src_neg)
    src = -src;

  إذا (src < *dest)
    *dest -= src;
  وإلا
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  إذا (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_خالص
ساكن bool
has_uuid_suffix (حرف ثابت *s)
{
  size_t len = مزي_اوږدوالی (s);
  أرجع (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساكن فراغ
get_ډګر_ارزښتونه (بنية ډګر_ارزښتونه_t *bv,
                  بنية ډګر_ارزښتونه_t *iv,
                  ثابت بنية fs_usage *fsu)
{
  /* Inode values.  */
  iv->ننوت_units = iv->output_units = 1;
  iv->ټول = fsu->fsu_files;
  iv->شته = iv->شته_منزل_ریښه = fsu->fsu_ffree;
  iv->negate_شته = false;

  iv->کارول_شوی = UINTMAX_MAX;
  iv->negate_کارول_شوی = false;
  إذا (معلوم_ارزښت (iv->ټول) && معلوم_ارزښت (iv->شته_منزل_ریښه))
    {
      iv->کارول_شوی = iv->ټول - iv->شته_منزل_ریښه;
      iv->negate_کارول_شوی = (iv->ټول < iv->شته_منزل_ریښه);
    }

  /* Block values.  */
  bv->ننوت_units = fsu->fsu_blocksize;
  bv->output_units = output_بلاک_اندازه;
  bv->ټول = fsu->fsu_بلاکونه;
  bv->شته = fsu->fsu_bavail;
  bv->شته_منزل_ریښه = fsu->fsu_bfree;
  bv->negate_شته = (fsu->fsu_bavail_سر_بټ_سېټ
                         && معلوم_ارزښت (fsu->fsu_bavail));

  bv->کارول_شوی = UINTMAX_MAX;
  bv->negate_کارول_شوی = false;
  إذا (معلوم_ارزښت (bv->ټول) && معلوم_ارزښت (bv->شته_منزل_ریښه))
    {
      bv->کارول_شوی = bv->ټول - bv->شته_منزل_ریښه;
      bv->negate_کارول_شوی = (bv->ټول < bv->شته_منزل_ریښه);
    }
}

/* Add block and inode values to grand total.  */
ساكن فراغ
جمع_کړه_منزل_grand_ټول (بنية ډګر_ارزښتونه_t *bv, بنية ډګر_ارزښتونه_t *iv)
{
  إذا (معلوم_ارزښت (iv->ټول))
    grand_fsu.fsu_files += iv->ټول;
  إذا (معلوم_ارزښت (iv->شته))
    grand_fsu.fsu_ffree += iv->شته;

  إذا (معلوم_ارزښت (bv->ټول))
    grand_fsu.fsu_بلاکونه += bv->ننوت_units * bv->ټول;
  إذا (معلوم_ارزښت (bv->شته_منزل_ریښه))
    grand_fsu.fsu_bfree += bv->ننوت_units * bv->شته_منزل_ریښه;
  إذا (معلوم_ارزښت (bv->شته))
    جمع_کړه_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_سر_بټ_سېټ,
                            bv->ننوت_units * bv->شته,
                            bv->negate_شته);
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

ساكن فراغ
get_dev (حرف ثابت *device, حرف ثابت *mount_ټکی, حرف ثابت *file,
         حرف ثابت *stat_file, حرف ثابت *fstype,
         bool me_dummy, bool me_remote,
         ثابت بنية fs_usage *force_fsu,
         bool process_all)
{
  إذا (me_remote && show_local_fs)
    أرجع;

  إذا (me_dummy && !show_all_fs && !show_listed_fs)
    أرجع;

  إذا (!selected_fstype (fstype) || excluded_fstype (fstype))
    أرجع;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  إذا (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_ټکی))
    أرجع;

  إذا (!stat_file)
    stat_file = mount_ټکی;

  بنية fs_usage fsu;
  إذا (force_fsu)
    fsu = *force_fsu;
  وإلا إذا (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      إذا (process_all && (errno == EACCES || errno == ENOENT))
        {
          إذا (! show_all_fs)
            أرجع;

          fstype = "-";
          fsu.fsu_bavail_سر_بټ_سېټ = false;
          fsu.fsu_blocksize = fsu.fsu_بلاکونه = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      وإلا
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_حالت = EXIT_FAILURE;
          أرجع;
        }
    }
  وإلا إذا (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      بنية stat sb;
      إذا (stat (stat_file, &sb) == 0)
        {
          بنية mount_ثبت ثابت * dev_me = me_for_dev (sb.st_dev);
          إذا (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_سر_بټ_سېټ = false;
              fsu.fsu_blocksize = fsu.fsu_بلاکونه = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  إذا (fsu.fsu_بلاکونه == 0 && !show_all_fs && !show_listed_fs)
    أرجع;

  إذا (! force_fsu)
    file_systems_processed = true;

  alloc_جدول_لیکه ();

  إذا (! device)
    device = "-";		/* unknown */

  إذا (! file)
    file = "-";			/* unspecified */

  حرف *dev_name = xstrdup (device);
  حرف *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  إذا (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_ډېرپېښ (dev_name, CAN_EXISTING)))
    {
      حرر (dev_name);
      dev_name = resolved_dev;
    }

  إذا (! fstype)
    fstype = "-";		/* unknown */

  بنية ډګر_ارزښتونه_t بلاک_ارزښتونه;
  بنية ډګر_ارزښتونه_t inode_ارزښتونه;
  get_ډګر_ارزښتونه (&بلاک_ارزښتونه, &inode_ارزښتونه, &fsu);

  /* Add to grand total unless processing grand total line.  */
  إذا (print_grand_ټول && ! force_fsu)
    جمع_کړه_منزل_grand_ټول (&بلاک_ارزښتونه, &inode_ارزښتونه);

  لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      بنية ډګر_ارزښتونه_t *v;
      بدل (جدول.columns[ستون]->ډګر_type)
        {
        حالة بلاک_FLD:
          v = &بلاک_ارزښتونه;
          اكسر;
        حالة INODE_FLD:
          v = &inode_ارزښتونه;
          اكسر;
        حالة OTHER_FLD:
          v = عدم;
          اكسر;
        افتراضي:
          affirm (!"bad field_type");
        }

      بدل (جدول.columns[ستون]->ډګر)
        {
        حالة سرچینه_ډګر:
          cell = xstrdup (dev_name);
          اكسر;

        حالة FSTYPE_ډګر:
          cell = xstrdup (fstype);
          اكسر;

        حالة اندازه_ډګر:
        حالة ITOTAL_ډګر:
          cell = xstrdup (df_readable (false, v->ټول, buf,
                                       v->ننوت_units, v->output_units));
          اكسر;

        حالة کارول_شوی_ډګر:
        حالة IUSED_ډګر:
          cell = xstrdup (df_readable (v->negate_کارول_شوی, v->کارول_شوی, buf,
                                       v->ننوت_units, v->output_units));
          اكسر;

        حالة AVAIL_ډګر:
        حالة IAVAIL_ډګر:
          cell = xstrdup (df_readable (v->negate_شته, v->شته, buf,
                                       v->ننوت_units, v->output_units));
          اكسر;

        حالة PCENT_ډګر:
        حالة IPCENT_ډګر:
          {
            مزدوج pct = -1;
            إذا (! معلوم_ارزښت (v->کارول_شوی) || ! معلوم_ارزښت (v->شته))
              ;
            وإلا إذا (!v->negate_کارول_شوی
                     && v->کارول_شوی <= UINTMAX_MAX / 100
                     && v->کارول_شوی + v->شته != 0
                     && (v->کارول_شوی + v->شته < v->کارول_شوی)
                     == v->negate_شته)
              {
                uintmax_t u100 = v->کارول_شوی * 100;
                uintmax_t nonroot_ټول = v->کارول_شوی + v->شته;
                pct = u100 / nonroot_ټول + (u100 % nonroot_ټول != 0);
              }
            وإلا
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                مزدوج u = v->negate_کارول_شوی ? - (مزدوج) - v->کارول_شوی : v->کارول_شوی;
                مزدوج a = v->negate_شته
                           ? - (مزدوج) - v->شته : v->شته;
                مزدوج nonroot_ټول = u + a;
                إذا (nonroot_ټول)
                  {
                    اوږد صحيح lipct = pct = u * 100 / nonroot_ټول;
                    مزدوج ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    إذا (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            اكسر;
          }

        حالة FILE_ډګر:
          cell = xstrdup (file);
          اكسر;

        حالة موخه_ډګر:
#که_تعریف_شوی HIDE_AUTOMOUNT_مختاړی
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          إذا (STRNCMP_LIT (mount_ټکی, "/auto/") == 0)
            mount_ټکی += 5;
          وإلا إذا (STRNCMP_LIT (mount_ټکی, "/tmp_mnt/") == 0)
            mount_ټکی += 8;
#د_که_پای
          cell = xstrdup (mount_ټکی);
          اكسر;

        حالة INVALID_ډګر:
        افتراضي:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحيح cell_پلنوالی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->پلنوالی = MAX (جدول.columns[ستون]->پلنوالی, cell_پلنوالی);
      جدول.لیکې[جدول.nrows - 1][ستون] = cell;
    }
  حرر (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساكن حرف *
وروستی_device_for_mount (حرف ثابت *mount)
{
  بنية mount_ثبت ثابت *le = عدم;

  لكل (بنية mount_ثبت ثابت *me = mount_list; me; me = me->me_بل)
    {
      إذا (streq (me->me_mountdir, mount))
        le = me;
    }

  إذا (le)
    {
      حرف *devname = le->me_devname;
      حرف *canon_dev = canonicalize_file_name (devname);
      إذا (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        أرجع canon_dev;
      حرر (canon_dev);
      أرجع xstrdup (le->me_devname);
    }
  وإلا
    أرجع عدم;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ساكن bool
get_device (حرف ثابت *device)
{
  بنية mount_ثبت ثابت *غوره_سمون = عدم;
  bool غوره_سمون_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  إذا (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t غوره_سمون_len = SIZE_MAX;
  لكل (بنية mount_ثبت ثابت *me = mount_list; me; me = me->me_بل)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      إذا (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      إذا (streq (device, devname))
        {
          حرف *وروستی_device = وروستی_device_for_mount (me->me_mountdir);
          eclipsed_device = وروستی_device && ! streq (وروستی_device, devname);
          size_t len = مزي_اوږدوالی (me->me_mountdir);

          إذا (! eclipsed_device
              && (! غوره_سمون_accessible || len < غوره_سمون_len))
            {
              بنية stat device_stats;
              bool this_سمون_accessible = false;

              إذا (stat (me->me_mountdir, &device_stats) == 0)
                غوره_سمون_accessible = this_سمون_accessible = true;

              إذا (this_سمون_accessible
                  || (! غوره_سمون_accessible && len < غوره_سمون_len))
                {
                  غوره_سمون = me;
                  إذا (len == 1) /* Traditional root.  */
                    {
                      حرر (وروستی_device);
                      حرر (canon_dev);
                      اكسر;
                    }
                  وإلا
                    غوره_سمون_len = len;
                }
            }

          حرر (وروستی_device);
        }

      حرر (canon_dev);
    }

  حرر (resolved);

  إذا (غوره_سمون)
    {
      get_dev (غوره_سمون->me_devname, غوره_سمون->me_mountdir, file, عدم,
               غوره_سمون->me_type, غوره_سمون->me_dummy,
               غوره_سمون->me_remote, عدم, false);
      أرجع true;
    }
  وإلا إذا (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_حالت = EXIT_FAILURE;
      أرجع true;
    }

  أرجع false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ساكن فراغ
get_ټکی (حرف ثابت *ټکی, ثابت بنية stat *statp)
{
  بنية stat device_stats;
  بنية mount_ثبت ثابت *غوره_سمون = عدم;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (ټکی);
  إذا (resolved && resolved[0] == '/')
    {
      size_t resolved_len = مزي_اوږدوالی (resolved);
      size_t غوره_سمون_len = 0;

      لكل (بنية mount_ثبت *me = mount_list; me; me = me->me_بل)
        {
          إذا (!streq (me->me_type, "lofs")
              && (!غوره_سمون || غوره_سمون->me_dummy || !me->me_dummy))
            {
              size_t len = مزي_اوږدوالی (me->me_mountdir);
              إذا (غوره_سمون_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  غوره_سمون = me;
                  غوره_سمون_len = len;
                }
            }
        }
    }
  حرر (resolved);
  إذا (غوره_سمون
      && (stat (غوره_سمون->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    غوره_سمون = عدم;

  إذا (! غوره_سمون)
    لكل (بنية mount_ثبت *me = mount_list; me; me = me->me_بل)
      {
        إذا (me->me_dev == (dev_t) -1)
          {
            إذا (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            وإلا
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                إذا (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_حالت = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        إذا (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!غوره_سمون || غوره_سمون->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            إذا (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            وإلا
              غوره_سمون = me;
          }
      }

  إذا (غوره_سمون)
    get_dev (غوره_سمون->me_devname, غوره_سمون->me_mountdir, ټکی, ټکی,
             غوره_سمون->me_type, غوره_سمون->me_dummy, غوره_سمون->me_remote,
             عدم, false);
  وإلا
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = پیدا_کړه_mount_ټکی (ټکی, statp);
      إذا (mp)
        {
          get_dev (عدم, mp, ټکی, عدم, عدم,
                   false, false, عدم, false);
          حرر (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساكن فراغ
get_ثبت (حرف ثابت *name, بنية stat ثابت *statp)
{
  إذا ((S_ISBLK (statp->st_ډېرپېښ) || S_ISCHR (statp->st_ډېرپېښ))
      && get_device (name))
    أرجع;

  get_ټکی (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساكن فراغ
get_all_entries (فراغ)
{
  filter_mount_list (show_all_fs);

  لكل (بنية mount_ثبت *me = mount_list; me; me = me->me_بل)
    get_dev (me->me_devname, me->me_mountdir, عدم, عدم, me->me_type,
             me->me_dummy, me->me_remote, عدم, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساكن فراغ
جمع_کړه_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_بل = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساكن فراغ
جمع_کړه_excluded_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_بل = fs_exclude_list;
  fs_exclude_list = fsp;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s [OPTION]... [FILE]...\n"), پروګرام_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by افتراضي.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --بلاک-اندازه=اندازه_2\n\
         scale sizes by اندازه_2 before printing them; see اندازه_2 format لاندې;\n\
         E.g., '-BM' prints sizes دننه units د 1,048,576 بایټونه\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes دننه powers د 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes دننه powers د 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead د بلاک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاک-اندازه=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         حد listing منزل local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         افعل not invoke sync before getting usage info (افتراضي)\n\
"));
      oputs (_("\
      --output[=ډګر_LIST]\n\
         use the output format defined by ډګر_LIST,\n\
         or print all fields إذا ډګر_LIST is omitted\n\
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
      --ټول\n\
         elide all entries insignificant منزل شته space,\n\
         and produce a grand ټول\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         حد listing منزل file systems د type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         حد listing منزل file systems not د type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_اندازه_note ();
      fputs (_("\n\
ډګر_LIST is a comma-separated list د columns منزل be included.  Valid\n\
ډګر نومونه are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info پاڼه).\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  بنية stat *stats = عدم;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  حرف ثابت *پیغام_mut_excl = _("options %s and %s are mutually exclusive");

  طالما (true)
    {
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 'a':
          show_all_fs = true;
          اكسر;
        حالة 'B':
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاک_اندازه);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;
        حالة 'i':
          إذا (سرلیک_ډېرپېښ == OUTPUT_ډېرپېښ)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          سرلیک_ډېرپېښ = INODES_ډېرپېښ;
          اكسر;
        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_بنسټ_1024;
          output_بلاک_اندازه = 1;
          اكسر;
        حالة 'H':
          human_output_opts = human_autoscale | human_SI;
          output_بلاک_اندازه = 1;
          اكسر;
        حالة 'k':
          human_output_opts = 0;
          output_بلاک_اندازه = 1024;
          اكسر;
        حالة 'l':
          show_local_fs = true;
          اكسر;
        حالة 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_بلاک_اندازه = 1024 * 1024;
          اكسر;
        حالة 'T':
          إذا (سرلیک_ډېرپېښ == OUTPUT_ډېرپېښ)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          اكسر;
        حالة 'P':
          إذا (سرلیک_ډېرپېښ == OUTPUT_ډېرپېښ)
            {
              error (0, 0, پیغام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          اكسر;
        حالة SYNC_OPTION:
          require_sync = true;
          اكسر;
        حالة NO_SYNC_OPTION:
          require_sync = false;
          اكسر;

        حالة 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        حالة 't':
          جمع_کړه_fs_type (optarg);
          اكسر;

        حالة 'v':		/* For SysV compatibility.  */
          /* ignore */
          اكسر;
        حالة 'x':
          جمع_کړه_excluded_fs_type (optarg);
          اكسر;

        حالة OUTPUT_OPTION:
          إذا (سرلیک_ډېرپېښ == INODES_ډېرپېښ)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (posix_format && سرلیک_ډېرپېښ == DEFAULT_ډېرپېښ)
            {
              error (0, 0, پیغام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (print_type)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          سرلیک_ډېرپېښ = OUTPUT_ډېرپېښ;
          إذا (optarg)
            decode_output_arg (optarg);
          اكسر;

        حالة ټول_OPTION:
          print_grand_ټول = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (human_output_opts == -1)
    {
      إذا (posix_format)
        {
          human_output_opts = 0;
          output_بلاک_اندازه = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      وإلا
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_بلاک_اندازه);
    }

  إذا (سرلیک_ډېرپېښ == INODES_ډېرپېښ || سرلیک_ډېرپېښ == OUTPUT_ډېرپېښ)
    ;
  وإلا إذا (human_output_opts & human_autoscale)
    سرلیک_ډېرپېښ = HUMAN_ډېرپېښ;
  وإلا إذا (posix_format)
    سرلیک_ډېرپېښ = POSIX_ډېرپېښ;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool سمون = false;
    لكل (بنية fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_بل)
      {
        لكل (بنية fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_بل)
          {
            إذا (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                سمون = true;
                اكسر;
              }
          }
      }
    إذا (سمون)
      أرجع EXIT_FAILURE;
  }

  إذا (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, حجم *stats);
      لكل (صحيح i = optind; i < argc; ++i)
        {
          صحيح err = automount_stat_err (argv[i], &stats[i - optind]);
          إذا (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_حالت = EXIT_FAILURE;
              argv[i] = عدم;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != عدم
                            || fs_exclude_list != عدم
                            || print_type
                            || ډګر_معلومات_2[FSTYPE_ډګر].کارول_شوی
                            || show_local_fs));

  إذا (mount_list == عدم)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      صحيح حالت = 0;
      إذا ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != عدم
               || fs_exclude_list != عدم))
        {
          حالت = EXIT_FAILURE;
        }
      حرف ثابت *warning = (حالت == 0 ? _("Warning: ") : "");
      error (حالت, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  إذا (require_sync)
    sync ();

  get_ډګر_list ();
  get_سرلیک ();

  إذا (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      لكل (صحيح i = optind; i < argc; ++i)
        إذا (argv[i])
          get_ثبت (argv[i], &stats[i - optind]);
    }
  وإلا
    get_all_entries ();

  إذا (file_systems_processed)
    {
      إذا (print_grand_ټول)
        get_dev ("total",
                 (ډګر_معلومات_2[سرچینه_ډګر].کارول_شوی ? "-" : "total"),
                 عدم, عدم, عدم, false, false, &grand_fsu, false);

      print_جدول ();
    }
  وإلا
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      إذا (exit_حالت == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_حالت);
}
