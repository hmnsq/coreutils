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
#تعریف_کن برنامه_NAME "df"

#تعریف_کن AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

بنية devlist
{
  dev_t dev_رقم;
  بنية mount_مدخل *me;
  بنية devlist *بعدی;
  بنية devlist *بعدی_same_dev;
  بنية devlist *دیده_آخرین; /* valid for hashed devlist entries only */
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
ساكن uintmax_t output_بلوک_اندازه;

/* True if a file system has been processed for output.  */
ساكن bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ساكن bool require_sync;

/* Desired exit status.  */
ساكن صحيح exit_وضعیت;

/* A file system type to display.  */

بنية fs_type_list
{
  حرف *fs_name;
  بنية fs_type_list *fs_بعدی;
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
ساكن بنية mount_مدخل *mount_list;

/* If true, print file system type as well.  */
ساكن bool print_type;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_مجموع;

/* Grand total data.  */
ساكن بنية fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساكن تعداد
{
  DEFAULT_نما,
  INODES_نما,
  HUMAN_نما,
  POSIX_نما,
  OUTPUT_نما
} سرآیند_نما = DEFAULT_نما;

/* Displayable fields.  */
تعريفنوع تعداد
{
  منبع_فیلد, /* file system */
  FSTYPE_فیلد, /* FS type */
  اندازه_فیلد,   /* FS size */
  استفاده_شده_فیلد,   /* FS size used  */
  AVAIL_فیلد,  /* FS size available */
  PCENT_فیلد,  /* percent used */
  ITOTAL_فیلد, /* inode total */
  IUSED_فیلد,  /* inodes used */
  IAVAIL_فیلد, /* inodes available */
  IPCENT_فیلد, /* inodes used in percent */
  هدف_فیلد, /* mount point */
  FILE_فیلد,   /* specified file name */
  INVALID_فیلد /* validation marker */
} display_فیلد_t;

/* Flag if a field contains a block, an inode or another value.  */
تعريفنوع تعداد
{
  بلوک_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} فیلد_type_t;

/* Attributes of a display field.  */
بنية فیلد_داده_t
{
  display_فیلد_t فیلد;
  حرف ثابت *arg;
  فیلد_type_t فیلد_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحيح عرض;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_راست;   /* Whether to right-align columns, not left-align.  */
  bool استفاده_شده;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساكن بنية فیلد_داده_t فیلد_داده_2[] = {
  [منبع_فیلد] = { منبع_فیلد,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_فیلد] = { FSTYPE_فیلد,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [اندازه_فیلد] = { اندازه_فیلد,
    "size",   بلوک_FLD, N_("blocks"),      5, true, false },

  [استفاده_شده_فیلد] = { استفاده_شده_فیلد,
    "used",   بلوک_FLD, N_("Used"),        5, true, false },

  [AVAIL_فیلد] = { AVAIL_فیلد,
    "avail",  بلوک_FLD, N_("Available"),   5, true, false },

  [PCENT_فیلد] = { PCENT_فیلد,
    "pcent",  بلوک_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_فیلد] = { ITOTAL_فیلد,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_فیلد] = { IUSED_فیلد,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_فیلد] = { IAVAIL_فیلد,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_فیلد] = { IPCENT_فیلد,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [هدف_فیلد] = { هدف_فیلد,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_فیلد] = { FILE_فیلد,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ساكن حرف ثابت *all_آرگومانها_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساكن بنية
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***ردیفها COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  بنية فیلد_داده_t **columns COUNTED_BY (ncolumns);
} جدول;

/* Field values.  */
بنية فیلد_مقادیر_t
{
  uintmax_t درون_داد_units;
  uintmax_t output_units;
  uintmax_t مجموع;
  uintmax_t موجود;
  bool negate_موجود;
  uintmax_t موجود_مقصد_ریشه;
  uintmax_t استفاده_شده;
  bool negate_استفاده_شده;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  مجموع_OPTION,
  OUTPUT_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_آرگومان, عدم, 'a'},
  {"block-size", required_آرگومان, عدم, 'B'},
  {"inodes", no_آرگومان, عدم, 'i'},
  {"human-readable", no_آرگومان, عدم, 'h'},
  {"si", no_آرگومان, عدم, 'H'},
  {"local", no_آرگومان, عدم, 'l'},
  {"output", optional_آرگومان, عدم, OUTPUT_OPTION},
  {"portability", no_آرگومان, عدم, 'P'},
  {"print-type", no_آرگومان, عدم, 'T'},
  {"sync", no_آرگومان, عدم, SYNC_OPTION},
  {"no-sync", no_آرگومان, عدم, NO_SYNC_OPTION},
  {"total", no_آرگومان, عدم, مجموع_OPTION},
  {"type", required_آرگومان, عدم, 't'},
  {"exclude-type", required_آرگومان, عدم, 'x'},
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
  حرف *srcend = cell + طول_رشته (cell);
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
  ساكن صحيح tty_بیرون = -1;
  إذا (tty_بیرون < 0)
    tty_بیرون = isatty (STDOUT_FILENO);

  (tty_بیرون ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساكن فراغ
alloc_جدول_ردیف (فراغ)
{
  إذا (جدول.nrows == جدول.nrows_alloc)
    جدول.ردیفها = xpalloc (جدول.ردیفها, &جدول.nrows_alloc, 1, -1,
                          حجم *جدول.ردیفها);
  idx_t ثابت ردیف_idx = جدول.nrows++;
  جدول.ردیفها[ردیف_idx] = xinmalloc (جدول.ncolumns, حجم *جدول.ردیفها[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساكن فراغ
print_جدول (فراغ)
{
  لكل (idx_t ردیف = 0; ردیف < جدول.nrows; ردیف++)
    {
      لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
        {
          حرف *cell = جدول.ردیفها[ردیف][ستون];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          إذا (ستون != 0)
            حرف_چاپ_کن (' ');

          صحيح عرض = mbswidth (cell, MBSWIDTH_FLAGS);
          صحيح fill = عرض < 0 ? 0 : جدول.columns[ستون]->عرض - عرض;
          إذا (جدول.columns[ستون]->align_راست)
            لكل (; 0 < fill; fill--)
              حرف_چاپ_کن (' ');
          fputs (cell, stdout);
          إذا (ستون + 1 < جدول.ncolumns)
            لكل (; 0 < fill; fill--)
              حرف_چاپ_کن (' ');
        }
      حرف_چاپ_کن ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساكن فراغ
alloc_فیلد (صحيح f, حرف ثابت *c)
{
  إذا (جدول.ncolumns == جدول.ncolumns_alloc)
    جدول.columns = xpalloc (جدول.columns, &جدول.ncolumns_alloc, 1, -1,
                             حجم *جدول.columns);
  idx_t ثابت ستونی_idx = جدول.ncolumns++;
  جدول.columns[ستونی_idx] = &فیلد_داده_2[f];
  إذا (c != عدم)
    فیلد_داده_2[f].caption = c;

  affirm (!فیلد_داده_2[f].استفاده_شده);

  /* Mark field as used.  */
  فیلد_داده_2[f].استفاده_شده = true;
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
      display_فیلد_t فیلد = INVALID_فیلد;
      لكل (idx_t i = 0; i < countof (فیلد_داده_2); i++)
        {
          إذا (streq (فیلد_داده_2[i].arg, s))
            {
              فیلد = i;
              اكسر;
            }
        }
      إذا (فیلد == INVALID_فیلد)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      إذا (فیلد_داده_2[فیلد].استفاده_شده)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (فیلد_داده_2[فیلد].arg));
          usage (EXIT_FAILURE);
        }

      بدل (فیلد)
        {
        حالة منبع_فیلد:
        حالة FSTYPE_فیلد:
        حالة استفاده_شده_فیلد:
        حالة PCENT_فیلد:
        حالة ITOTAL_فیلد:
        حالة IUSED_فیلد:
        حالة IAVAIL_فیلد:
        حالة IPCENT_فیلد:
        حالة هدف_فیلد:
        حالة FILE_فیلد:
          alloc_فیلد (فیلد, عدم);
          اكسر;

        حالة اندازه_فیلد:
          alloc_فیلد (فیلد, N_("Size"));
          اكسر;

        حالة AVAIL_فیلد:
          alloc_فیلد (فیلد, N_("Avail"));
          اكسر;

        حالة INVALID_فیلد:
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
get_فیلد_list (فراغ)
{
  بدل (سرآیند_نما)
    {
    حالة DEFAULT_نما:
      alloc_فیلد (منبع_فیلد, عدم);
      إذا (print_type)
        alloc_فیلد (FSTYPE_فیلد, عدم);
      alloc_فیلد (اندازه_فیلد,   عدم);
      alloc_فیلد (استفاده_شده_فیلد,   عدم);
      alloc_فیلد (AVAIL_فیلد,  عدم);
      alloc_فیلد (PCENT_فیلد,  عدم);
      alloc_فیلد (هدف_فیلد, عدم);
      اكسر;

    حالة HUMAN_نما:
      alloc_فیلد (منبع_فیلد, عدم);
      إذا (print_type)
        alloc_فیلد (FSTYPE_فیلد, عدم);

      alloc_فیلد (اندازه_فیلد,   N_("Size"));
      alloc_فیلد (استفاده_شده_فیلد,   عدم);
      alloc_فیلد (AVAIL_فیلد,  N_("Avail"));
      alloc_فیلد (PCENT_فیلد,  عدم);
      alloc_فیلد (هدف_فیلد, عدم);
      اكسر;

    حالة INODES_نما:
      alloc_فیلد (منبع_فیلد, عدم);
      إذا (print_type)
        alloc_فیلد (FSTYPE_فیلد, عدم);
      alloc_فیلد (ITOTAL_فیلد,  عدم);
      alloc_فیلد (IUSED_فیلد,   عدم);
      alloc_فیلد (IAVAIL_فیلد,  عدم);
      alloc_فیلد (IPCENT_فیلد,  عدم);
      alloc_فیلد (هدف_فیلد,  عدم);
      اكسر;

    حالة POSIX_نما:
      alloc_فیلد (منبع_فیلد, عدم);
      إذا (print_type)
        alloc_فیلد (FSTYPE_فیلد, عدم);
      alloc_فیلد (اندازه_فیلد,   عدم);
      alloc_فیلد (استفاده_شده_فیلد,   عدم);
      alloc_فیلد (AVAIL_فیلد,  عدم);
      alloc_فیلد (PCENT_فیلد,  N_("Capacity"));
      alloc_فیلد (هدف_فیلد, عدم);
      اكسر;

    حالة OUTPUT_نما:
      إذا (!جدول.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_آرگومانها_string);
        }
      اكسر;

    افتراضي:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساكن فراغ
get_سرآیند (فراغ)
{
  alloc_جدول_ردیف ();

  لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف *cell;
      حرف ثابت *سرآیند = _(جدول.columns[ستون]->caption);

      إذا (جدول.columns[ستون]->فیلد == اندازه_فیلد
          && (سرآیند_نما == DEFAULT_نما
              || (سرآیند_نما == OUTPUT_نما
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحيح opts = (human_suppress_نقطه_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_گروه_digits | human_پایه_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_بلوک_اندازه;
          uintmax_t q1024 = output_بلوک_اندازه;
          bool divisible_by_1000;
          bool divisible_by_1024;

          افعل
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          طالما (divisible_by_1000 & divisible_by_1024);

          إذا (divisible_by_1000 < divisible_by_1024)
            opts |= human_پایه_1024;
          إذا (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_پایه_1024;
          إذا (! (opts & human_پایه_1024))
            opts |= human_B;

          حرف *رقم = human_readable (output_بلوک_اندازه, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          سرآیند = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), رقم, سرآیند);
        }
      وإلا إذا (سرآیند_نما == POSIX_نما
               && جدول.columns[ستون]->فیلد == اندازه_فیلد)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_بلوک_اندازه, سرآیند);
        }
      وإلا
        cell = xstrdup (سرآیند);

      replace_problematic_chars (cell);

      جدول.ردیفها[جدول.nrows - 1][ستون] = cell;

      صحيح cell_عرض = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->عرض = MAX (جدول.columns[ستون]->عرض, cell_عرض);
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
       fsp = fsp->fs_بعدی)
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
       fsp = fsp->fs_بعدی)
    إذا (streq (fstype, fsp->fs_name))
      أرجع true;
  أرجع false;
}

ساكن size_t
devlist_hash (فراغ ثابت *x, size_t جدول_اندازه)
{
  بنية devlist ثابت *p = x;
  أرجع (uintmax_t) {p->dev_رقم} % جدول_اندازه;
}

ساكن bool
devlist_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية devlist ثابت *a = x;
  بنية devlist ثابت *b = y;
  أرجع a->dev_رقم == b->dev_رقم;
}

ساكن بنية devlist *
devlist_for_dev (dev_t dev)
{
  إذا (devlist_جدول == عدم)
    أرجع عدم;
  بنية devlist dev_مدخل;
  dev_مدخل.dev_رقم = dev;

  بنية devlist *یافت = hash_lookup (devlist_جدول, &dev_مدخل);
  إذا (یافت == عدم)
    أرجع عدم;

  /* Return the last devlist entry we have seen with this dev_num */
  أرجع یافت->دیده_آخرین;
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

  لكل (بنية mount_مدخل *me = mount_list; me; me = me->me_بعدی)
    mount_list_اندازه++;

  devlist_جدول = hash_initialize (mount_list_اندازه, عدم,
                                   devlist_hash, devlist_compare, عدم);
  إذا (devlist_جدول == عدم)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  لكل (بنية mount_مدخل *me = mount_list; me;)
    {
      بنية stat buf;
      بنية mount_مدخل *discard_me = عدم;
      بنية devlist *آخرین_دیده_dev = عدم, *دیده_dev = عدم;

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
          آخرین_دیده_dev = دیده_dev = devlist_for_dev (buf.st_dev);

          لكل (; دیده_dev && ! discard_me; دیده_dev = دیده_dev->بعدی_same_dev)
            {
              bool هدف_nearer_ریشه = طول_رشته (دیده_dev->me->me_mountdir)
                                        > طول_رشته (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool منبع_زیر_ریشه = دیده_dev->me->me_mntroot != عدم
                                       && me->me_mntroot != عدم
                                       && (طول_رشته (دیده_dev->me->me_mntroot)
                                           < طول_رشته (me->me_mntroot));
              إذا (! print_grand_مجموع
                  && me->me_remote && دیده_dev->me->me_remote
                  && ! streq (دیده_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              وإلا إذا ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (دیده_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (هدف_nearer_ریشه && ! منبع_زیر_ریشه)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (دیده_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     دیده_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = دیده_dev->me;
                  دیده_dev->me = me;
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
          me = me->me_بعدی;
          إذا (! devices_only)
            free_mount_مدخل (discard_me);
        }
      وإلا
        {
          /* Add the device number to the device_table.  */
          بنية devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_رقم = buf.st_dev;
          devlist->بعدی_same_dev = آخرین_دیده_dev;
          devlist->بعدی = device_list;
          device_list = devlist;

          بنية devlist *hash_مدخل = hash_درج (devlist_جدول, devlist);
          إذا (hash_مدخل == عدم)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_مدخل->دیده_آخرین = devlist;

          me = me->me_بعدی;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  إذا (! devices_only)
    {
      mount_list = عدم;
      طالما (device_list)
        {
          /* Add the mount entry.  */
          بنية mount_مدخل *me = device_list->me;
          me->me_بعدی = mount_list;
          mount_list = me;
          بنية devlist *بعدی = device_list->بعدی;
          حرر (device_list);
          device_list = بعدی;
        }
      hash_free (devlist_جدول);
      devlist_جدول = عدم;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_خالص
ساكن بنية mount_مدخل ثابت *
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
معلوم_مقدار (uintmax_t n)
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
             uintmax_t درون_داد_units, uintmax_t output_units)
{
  إذا (! معلوم_مقدار (n) && !negative)
    أرجع "-";
  وإلا
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, درون_داد_units, output_units);
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
جمع_کن_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
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
  size_t len = طول_رشته (s);
  أرجع (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساكن فراغ
get_فیلد_مقادیر (بنية فیلد_مقادیر_t *bv,
                  بنية فیلد_مقادیر_t *iv,
                  ثابت بنية fs_usage *fsu)
{
  /* Inode values.  */
  iv->درون_داد_units = iv->output_units = 1;
  iv->مجموع = fsu->fsu_files;
  iv->موجود = iv->موجود_مقصد_ریشه = fsu->fsu_ffree;
  iv->negate_موجود = false;

  iv->استفاده_شده = UINTMAX_MAX;
  iv->negate_استفاده_شده = false;
  إذا (معلوم_مقدار (iv->مجموع) && معلوم_مقدار (iv->موجود_مقصد_ریشه))
    {
      iv->استفاده_شده = iv->مجموع - iv->موجود_مقصد_ریشه;
      iv->negate_استفاده_شده = (iv->مجموع < iv->موجود_مقصد_ریشه);
    }

  /* Block values.  */
  bv->درون_داد_units = fsu->fsu_blocksize;
  bv->output_units = output_بلوک_اندازه;
  bv->مجموع = fsu->fsu_بلوکها;
  bv->موجود = fsu->fsu_bavail;
  bv->موجود_مقصد_ریشه = fsu->fsu_bfree;
  bv->negate_موجود = (fsu->fsu_bavail_بالاترین_بیت_مجموعه
                         && معلوم_مقدار (fsu->fsu_bavail));

  bv->استفاده_شده = UINTMAX_MAX;
  bv->negate_استفاده_شده = false;
  إذا (معلوم_مقدار (bv->مجموع) && معلوم_مقدار (bv->موجود_مقصد_ریشه))
    {
      bv->استفاده_شده = bv->مجموع - bv->موجود_مقصد_ریشه;
      bv->negate_استفاده_شده = (bv->مجموع < bv->موجود_مقصد_ریشه);
    }
}

/* Add block and inode values to grand total.  */
ساكن فراغ
جمع_کن_مقصد_grand_مجموع (بنية فیلد_مقادیر_t *bv, بنية فیلد_مقادیر_t *iv)
{
  إذا (معلوم_مقدار (iv->مجموع))
    grand_fsu.fsu_files += iv->مجموع;
  إذا (معلوم_مقدار (iv->موجود))
    grand_fsu.fsu_ffree += iv->موجود;

  إذا (معلوم_مقدار (bv->مجموع))
    grand_fsu.fsu_بلوکها += bv->درون_داد_units * bv->مجموع;
  إذا (معلوم_مقدار (bv->موجود_مقصد_ریشه))
    grand_fsu.fsu_bfree += bv->درون_داد_units * bv->موجود_مقصد_ریشه;
  إذا (معلوم_مقدار (bv->موجود))
    جمع_کن_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_بالاترین_بیت_مجموعه,
                            bv->درون_داد_units * bv->موجود,
                            bv->negate_موجود);
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
get_dev (حرف ثابت *device, حرف ثابت *mount_نقطه, حرف ثابت *file,
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
  إذا (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_نقطه))
    أرجع;

  إذا (!stat_file)
    stat_file = mount_نقطه;

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
          fsu.fsu_bavail_بالاترین_بیت_مجموعه = false;
          fsu.fsu_blocksize = fsu.fsu_بلوکها = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      وإلا
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_وضعیت = EXIT_FAILURE;
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
          بنية mount_مدخل ثابت * dev_me = me_for_dev (sb.st_dev);
          إذا (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_بالاترین_بیت_مجموعه = false;
              fsu.fsu_blocksize = fsu.fsu_بلوکها = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  إذا (fsu.fsu_بلوکها == 0 && !show_all_fs && !show_listed_fs)
    أرجع;

  إذا (! force_fsu)
    file_systems_processed = true;

  alloc_جدول_ردیف ();

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
      && (resolved_dev = canonicalize_filename_نما (dev_name, CAN_EXISTING)))
    {
      حرر (dev_name);
      dev_name = resolved_dev;
    }

  إذا (! fstype)
    fstype = "-";		/* unknown */

  بنية فیلد_مقادیر_t بلوک_مقادیر;
  بنية فیلد_مقادیر_t inode_مقادیر;
  get_فیلد_مقادیر (&بلوک_مقادیر, &inode_مقادیر, &fsu);

  /* Add to grand total unless processing grand total line.  */
  إذا (print_grand_مجموع && ! force_fsu)
    جمع_کن_مقصد_grand_مجموع (&بلوک_مقادیر, &inode_مقادیر);

  لكل (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      بنية فیلد_مقادیر_t *v;
      بدل (جدول.columns[ستون]->فیلد_type)
        {
        حالة بلوک_FLD:
          v = &بلوک_مقادیر;
          اكسر;
        حالة INODE_FLD:
          v = &inode_مقادیر;
          اكسر;
        حالة OTHER_FLD:
          v = عدم;
          اكسر;
        افتراضي:
          affirm (!"bad field_type");
        }

      بدل (جدول.columns[ستون]->فیلد)
        {
        حالة منبع_فیلد:
          cell = xstrdup (dev_name);
          اكسر;

        حالة FSTYPE_فیلد:
          cell = xstrdup (fstype);
          اكسر;

        حالة اندازه_فیلد:
        حالة ITOTAL_فیلد:
          cell = xstrdup (df_readable (false, v->مجموع, buf,
                                       v->درون_داد_units, v->output_units));
          اكسر;

        حالة استفاده_شده_فیلد:
        حالة IUSED_فیلد:
          cell = xstrdup (df_readable (v->negate_استفاده_شده, v->استفاده_شده, buf,
                                       v->درون_داد_units, v->output_units));
          اكسر;

        حالة AVAIL_فیلد:
        حالة IAVAIL_فیلد:
          cell = xstrdup (df_readable (v->negate_موجود, v->موجود, buf,
                                       v->درون_داد_units, v->output_units));
          اكسر;

        حالة PCENT_فیلد:
        حالة IPCENT_فیلد:
          {
            مزدوج pct = -1;
            إذا (! معلوم_مقدار (v->استفاده_شده) || ! معلوم_مقدار (v->موجود))
              ;
            وإلا إذا (!v->negate_استفاده_شده
                     && v->استفاده_شده <= UINTMAX_MAX / 100
                     && v->استفاده_شده + v->موجود != 0
                     && (v->استفاده_شده + v->موجود < v->استفاده_شده)
                     == v->negate_موجود)
              {
                uintmax_t u100 = v->استفاده_شده * 100;
                uintmax_t nonroot_مجموع = v->استفاده_شده + v->موجود;
                pct = u100 / nonroot_مجموع + (u100 % nonroot_مجموع != 0);
              }
            وإلا
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                مزدوج u = v->negate_استفاده_شده ? - (مزدوج) - v->استفاده_شده : v->استفاده_شده;
                مزدوج a = v->negate_موجود
                           ? - (مزدوج) - v->موجود : v->موجود;
                مزدوج nonroot_مجموع = u + a;
                إذا (nonroot_مجموع)
                  {
                    دراز صحيح lipct = pct = u * 100 / nonroot_مجموع;
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

        حالة FILE_فیلد:
          cell = xstrdup (file);
          اكسر;

        حالة هدف_فیلد:
#اگر_تعریف_شده HIDE_AUTOMOUNT_پیشوند
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          إذا (STRNCMP_LIT (mount_نقطه, "/auto/") == 0)
            mount_نقطه += 5;
          وإلا إذا (STRNCMP_LIT (mount_نقطه, "/tmp_mnt/") == 0)
            mount_نقطه += 8;
#پایان_اگر
          cell = xstrdup (mount_نقطه);
          اكسر;

        حالة INVALID_فیلد:
        افتراضي:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحيح cell_عرض = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->عرض = MAX (جدول.columns[ستون]->عرض, cell_عرض);
      جدول.ردیفها[جدول.nrows - 1][ستون] = cell;
    }
  حرر (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساكن حرف *
آخرین_device_for_mount (حرف ثابت *mount)
{
  بنية mount_مدخل ثابت *le = عدم;

  لكل (بنية mount_مدخل ثابت *me = mount_list; me; me = me->me_بعدی)
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
  بنية mount_مدخل ثابت *بهترین_تطابق = عدم;
  bool بهترین_تطابق_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  إذا (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t بهترین_تطابق_len = SIZE_MAX;
  لكل (بنية mount_مدخل ثابت *me = mount_list; me; me = me->me_بعدی)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      إذا (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      إذا (streq (device, devname))
        {
          حرف *آخرین_device = آخرین_device_for_mount (me->me_mountdir);
          eclipsed_device = آخرین_device && ! streq (آخرین_device, devname);
          size_t len = طول_رشته (me->me_mountdir);

          إذا (! eclipsed_device
              && (! بهترین_تطابق_accessible || len < بهترین_تطابق_len))
            {
              بنية stat device_stats;
              bool this_تطابق_accessible = false;

              إذا (stat (me->me_mountdir, &device_stats) == 0)
                بهترین_تطابق_accessible = this_تطابق_accessible = true;

              إذا (this_تطابق_accessible
                  || (! بهترین_تطابق_accessible && len < بهترین_تطابق_len))
                {
                  بهترین_تطابق = me;
                  إذا (len == 1) /* Traditional root.  */
                    {
                      حرر (آخرین_device);
                      حرر (canon_dev);
                      اكسر;
                    }
                  وإلا
                    بهترین_تطابق_len = len;
                }
            }

          حرر (آخرین_device);
        }

      حرر (canon_dev);
    }

  حرر (resolved);

  إذا (بهترین_تطابق)
    {
      get_dev (بهترین_تطابق->me_devname, بهترین_تطابق->me_mountdir, file, عدم,
               بهترین_تطابق->me_type, بهترین_تطابق->me_dummy,
               بهترین_تطابق->me_remote, عدم, false);
      أرجع true;
    }
  وإلا إذا (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_وضعیت = EXIT_FAILURE;
      أرجع true;
    }

  أرجع false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ساكن فراغ
get_نقطه (حرف ثابت *نقطه, ثابت بنية stat *statp)
{
  بنية stat device_stats;
  بنية mount_مدخل ثابت *بهترین_تطابق = عدم;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (نقطه);
  إذا (resolved && resolved[0] == '/')
    {
      size_t resolved_len = طول_رشته (resolved);
      size_t بهترین_تطابق_len = 0;

      لكل (بنية mount_مدخل *me = mount_list; me; me = me->me_بعدی)
        {
          إذا (!streq (me->me_type, "lofs")
              && (!بهترین_تطابق || بهترین_تطابق->me_dummy || !me->me_dummy))
            {
              size_t len = طول_رشته (me->me_mountdir);
              إذا (بهترین_تطابق_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  بهترین_تطابق = me;
                  بهترین_تطابق_len = len;
                }
            }
        }
    }
  حرر (resolved);
  إذا (بهترین_تطابق
      && (stat (بهترین_تطابق->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    بهترین_تطابق = عدم;

  إذا (! بهترین_تطابق)
    لكل (بنية mount_مدخل *me = mount_list; me; me = me->me_بعدی)
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
                    exit_وضعیت = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        إذا (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!بهترین_تطابق || بهترین_تطابق->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            إذا (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            وإلا
              بهترین_تطابق = me;
          }
      }

  إذا (بهترین_تطابق)
    get_dev (بهترین_تطابق->me_devname, بهترین_تطابق->me_mountdir, نقطه, نقطه,
             بهترین_تطابق->me_type, بهترین_تطابق->me_dummy, بهترین_تطابق->me_remote,
             عدم, false);
  وإلا
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = پیدا_کن_mount_نقطه (نقطه, statp);
      إذا (mp)
        {
          get_dev (عدم, mp, نقطه, عدم, عدم,
                   false, false, عدم, false);
          حرر (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساكن فراغ
get_مدخل (حرف ثابت *name, بنية stat ثابت *statp)
{
  إذا ((S_ISBLK (statp->st_نما) || S_ISCHR (statp->st_نما))
      && get_device (name))
    أرجع;

  get_نقطه (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساكن فراغ
get_all_entries (فراغ)
{
  filter_mount_list (show_all_fs);

  لكل (بنية mount_مدخل *me = mount_list; me; me = me->me_بعدی)
    get_dev (me->me_devname, me->me_mountdir, عدم, عدم, me->me_type,
             me->me_dummy, me->me_remote, عدم, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساكن فراغ
جمع_کن_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_بعدی = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساكن فراغ
جمع_کن_excluded_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_بعدی = fs_exclude_list;
  fs_exclude_list = fsp;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... [FILE]...\n"), برنامه_name);
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
  -B, --بلوک-اندازه=اندازه_2\n\
         scale sizes by اندازه_2 before printing them; see اندازه_2 format زیر;\n\
         E.g., '-BM' prints sizes درون units از 1,048,576 بایتها\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes درون powers از 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes درون powers از 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead از بلوک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلوک-اندازه=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         حد listing مقصد local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         افعل not invoke sync before getting usage info (افتراضي)\n\
"));
      oputs (_("\
      --output[=فیلد_LIST]\n\
         use the output format defined by فیلد_LIST,\n\
         or print all fields إذا فیلد_LIST is omitted\n\
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
      --مجموع\n\
         elide all entries insignificant مقصد موجود space,\n\
         and produce a grand مجموع\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         حد listing مقصد file systems از type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         حد listing مقصد file systems not از type TYPE\n\
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
فیلد_LIST is a comma-separated list از columns مقصد be included.  Valid\n\
فیلد نامها are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info صفحه).\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  بنية stat *stats = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  حرف ثابت *پیام_mut_excl = _("options %s and %s are mutually exclusive");

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
                                                 &output_بلوک_اندازه);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;
        حالة 'i':
          إذا (سرآیند_نما == OUTPUT_نما)
            {
              error (0, 0, پیام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          سرآیند_نما = INODES_نما;
          اكسر;
        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_پایه_1024;
          output_بلوک_اندازه = 1;
          اكسر;
        حالة 'H':
          human_output_opts = human_autoscale | human_SI;
          output_بلوک_اندازه = 1;
          اكسر;
        حالة 'k':
          human_output_opts = 0;
          output_بلوک_اندازه = 1024;
          اكسر;
        حالة 'l':
          show_local_fs = true;
          اكسر;
        حالة 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_بلوک_اندازه = 1024 * 1024;
          اكسر;
        حالة 'T':
          إذا (سرآیند_نما == OUTPUT_نما)
            {
              error (0, 0, پیام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          اكسر;
        حالة 'P':
          إذا (سرآیند_نما == OUTPUT_نما)
            {
              error (0, 0, پیام_mut_excl, "-P", "--output");
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
          جمع_کن_fs_type (optarg);
          اكسر;

        حالة 'v':		/* For SysV compatibility.  */
          /* ignore */
          اكسر;
        حالة 'x':
          جمع_کن_excluded_fs_type (optarg);
          اكسر;

        حالة OUTPUT_OPTION:
          إذا (سرآیند_نما == INODES_نما)
            {
              error (0, 0, پیام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (posix_format && سرآیند_نما == DEFAULT_نما)
            {
              error (0, 0, پیام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (print_type)
            {
              error (0, 0, پیام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          سرآیند_نما = OUTPUT_نما;
          إذا (optarg)
            decode_output_arg (optarg);
          اكسر;

        حالة مجموع_OPTION:
          print_grand_مجموع = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (human_output_opts == -1)
    {
      إذا (posix_format)
        {
          human_output_opts = 0;
          output_بلوک_اندازه = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      وإلا
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_بلوک_اندازه);
    }

  إذا (سرآیند_نما == INODES_نما || سرآیند_نما == OUTPUT_نما)
    ;
  وإلا إذا (human_output_opts & human_autoscale)
    سرآیند_نما = HUMAN_نما;
  وإلا إذا (posix_format)
    سرآیند_نما = POSIX_نما;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool تطابق = false;
    لكل (بنية fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_بعدی)
      {
        لكل (بنية fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_بعدی)
          {
            إذا (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                تطابق = true;
                اكسر;
              }
          }
      }
    إذا (تطابق)
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
              exit_وضعیت = EXIT_FAILURE;
              argv[i] = عدم;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != عدم
                            || fs_exclude_list != عدم
                            || print_type
                            || فیلد_داده_2[FSTYPE_فیلد].استفاده_شده
                            || show_local_fs));

  إذا (mount_list == عدم)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      صحيح وضعیت = 0;
      إذا ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != عدم
               || fs_exclude_list != عدم))
        {
          وضعیت = EXIT_FAILURE;
        }
      حرف ثابت *warning = (وضعیت == 0 ? _("Warning: ") : "");
      error (وضعیت, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  إذا (require_sync)
    sync ();

  get_فیلد_list ();
  get_سرآیند ();

  إذا (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      لكل (صحيح i = optind; i < argc; ++i)
        إذا (argv[i])
          get_مدخل (argv[i], &stats[i - optind]);
    }
  وإلا
    get_all_entries ();

  إذا (file_systems_processed)
    {
      إذا (print_grand_مجموع)
        get_dev ("total",
                 (فیلد_داده_2[منبع_فیلد].استفاده_شده ? "-" : "total"),
                 عدم, عدم, عدم, false, false, &grand_fsu, false);

      print_جدول ();
    }
  وإلا
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      إذا (exit_وضعیت == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_وضعیت);
}
