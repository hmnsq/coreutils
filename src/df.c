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
#وصف_ڪريو پروگرام_NAME "df"

#وصف_ڪريو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

بنية devlist
{
  dev_t dev_انگ;
  بنية mount_داخلا *me;
  بنية devlist *ايندڙ;
  بنية devlist *ايندڙ_same_dev;
  بنية devlist *ڏٺل_آخري; /* valid for hashed devlist entries only */
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
ساكن uintmax_t output_بلاڪ_سائيز;

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
  بنية fs_type_list *fs_ايندڙ;
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
ساكن بنية mount_داخلا *mount_list;

/* If true, print file system type as well.  */
ساكن bool print_type;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_ڪل;

/* Grand total data.  */
ساكن بنية fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساكن تعداد
{
  DEFAULT_گهڻائي,
  INODES_گهڻائي,
  HUMAN_گهڻائي,
  POSIX_گهڻائي,
  OUTPUT_گهڻائي
} سرو_گهڻائي = DEFAULT_گهڻائي;

/* Displayable fields.  */
تعريفنوع تعداد
{
  ذريعو_ميدان, /* file system */
  FSTYPE_ميدان, /* FS type */
  سائيز_ميدان,   /* FS size */
  استعمال_ٿيل_ميدان,   /* FS size used  */
  AVAIL_ميدان,  /* FS size available */
  PCENT_ميدان,  /* percent used */
  ITOTAL_ميدان, /* inode total */
  IUSED_ميدان,  /* inodes used */
  IAVAIL_ميدان, /* inodes available */
  IPCENT_ميدان, /* inodes used in percent */
  هدف_ميدان, /* mount point */
  FILE_ميدان,   /* specified file name */
  INVALID_ميدان /* validation marker */
} display_ميدان_t;

/* Flag if a field contains a block, an inode or another value.  */
تعريفنوع تعداد
{
  بلاڪ_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} ميدان_type_t;

/* Attributes of a display field.  */
بنية ميدان_معلومات_t
{
  display_ميدان_t ميدان;
  حرف ثابت *arg;
  ميدان_type_t ميدان_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحيح ويڪر;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_ساڄو;   /* Whether to right-align columns, not left-align.  */
  bool استعمال_ٿيل;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساكن بنية ميدان_معلومات_t ميدان_معلومات_2[] = {
  [ذريعو_ميدان] = { ذريعو_ميدان,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_ميدان] = { FSTYPE_ميدان,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [سائيز_ميدان] = { سائيز_ميدان,
    "size",   بلاڪ_FLD, N_("blocks"),      5, true, false },

  [استعمال_ٿيل_ميدان] = { استعمال_ٿيل_ميدان,
    "used",   بلاڪ_FLD, N_("Used"),        5, true, false },

  [AVAIL_ميدان] = { AVAIL_ميدان,
    "avail",  بلاڪ_FLD, N_("Available"),   5, true, false },

  [PCENT_ميدان] = { PCENT_ميدان,
    "pcent",  بلاڪ_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_ميدان] = { ITOTAL_ميدان,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_ميدان] = { IUSED_ميدان,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_ميدان] = { IAVAIL_ميدان,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_ميدان] = { IPCENT_ميدان,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [هدف_ميدان] = { هدف_ميدان,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_ميدان] = { FILE_ميدان,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ساكن حرف ثابت *all_دليلون_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساكن بنية
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***صفون COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  بنية ميدان_معلومات_t **columns COUNTED_BY (ncolumns);
} جدول;

/* Field values.  */
بنية ميدان_قدرون_t
{
  uintmax_t داخلا_units;
  uintmax_t output_units;
  uintmax_t ڪل;
  uintmax_t موجود;
  bool negate_موجود;
  uintmax_t موجود_منزل_پاڙ;
  uintmax_t استعمال_ٿيل;
  bool negate_استعمال_ٿيل;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  ڪل_OPTION,
  OUTPUT_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_دليل, عدم, 'a'},
  {"block-size", required_دليل, عدم, 'B'},
  {"inodes", no_دليل, عدم, 'i'},
  {"human-readable", no_دليل, عدم, 'h'},
  {"si", no_دليل, عدم, 'H'},
  {"local", no_دليل, عدم, 'l'},
  {"output", optional_دليل, عدم, OUTPUT_OPTION},
  {"portability", no_دليل, عدم, 'P'},
  {"print-type", no_دليل, عدم, 'T'},
  {"sync", no_دليل, عدم, SYNC_OPTION},
  {"no-sync", no_دليل, عدم, NO_SYNC_OPTION},
  {"total", no_دليل, عدم, ڪل_OPTION},
  {"type", required_دليل, عدم, 't'},
  {"exclude-type", required_دليل, عدم, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
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
  حرف *srcend = cell + لڙي_ڊيگهه (cell);
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
  ساكن صحيح tty_ٻاهريون = -1;
  إذا (tty_ٻاهريون < 0)
    tty_ٻاهريون = isatty (STDOUT_FILENO);

  (tty_ٻاهريون ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساكن فراغ
alloc_جدول_صف (فراغ)
{
  إذا (جدول.nrows == جدول.nrows_alloc)
    جدول.صفون = xpalloc (جدول.صفون, &جدول.nrows_alloc, 1, -1,
                          حجم *جدول.صفون);
  idx_t ثابت صف_idx = جدول.nrows++;
  جدول.صفون[صف_idx] = xinmalloc (جدول.ncolumns, حجم *جدول.صفون[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساكن فراغ
print_جدول (فراغ)
{
  لكل (idx_t صف = 0; صف < جدول.nrows; صف++)
    {
      لكل (idx_t کانو = 0; کانو < جدول.ncolumns; کانو++)
        {
          حرف *cell = جدول.صفون[صف][کانو];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          إذا (کانو != 0)
            اکر_لکو (' ');

          صحيح ويڪر = mbswidth (cell, MBSWIDTH_FLAGS);
          صحيح fill = ويڪر < 0 ? 0 : جدول.columns[کانو]->ويڪر - ويڪر;
          إذا (جدول.columns[کانو]->align_ساڄو)
            لكل (; 0 < fill; fill--)
              اکر_لکو (' ');
          fputs (cell, stdout);
          إذا (کانو + 1 < جدول.ncolumns)
            لكل (; 0 < fill; fill--)
              اکر_لکو (' ');
        }
      اکر_لکو ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساكن فراغ
alloc_ميدان (صحيح f, حرف ثابت *c)
{
  إذا (جدول.ncolumns == جدول.ncolumns_alloc)
    جدول.columns = xpalloc (جدول.columns, &جدول.ncolumns_alloc, 1, -1,
                             حجم *جدول.columns);
  idx_t ثابت ڪالم_idx = جدول.ncolumns++;
  جدول.columns[ڪالم_idx] = &ميدان_معلومات_2[f];
  إذا (c != عدم)
    ميدان_معلومات_2[f].caption = c;

  affirm (!ميدان_معلومات_2[f].استعمال_ٿيل);

  /* Mark field as used.  */
  ميدان_معلومات_2[f].استعمال_ٿيل = true;
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
      display_ميدان_t ميدان = INVALID_ميدان;
      لكل (idx_t i = 0; i < countof (ميدان_معلومات_2); i++)
        {
          إذا (streq (ميدان_معلومات_2[i].arg, s))
            {
              ميدان = i;
              اكسر;
            }
        }
      إذا (ميدان == INVALID_ميدان)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      إذا (ميدان_معلومات_2[ميدان].استعمال_ٿيل)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (ميدان_معلومات_2[ميدان].arg));
          usage (EXIT_FAILURE);
        }

      بدل (ميدان)
        {
        حالة ذريعو_ميدان:
        حالة FSTYPE_ميدان:
        حالة استعمال_ٿيل_ميدان:
        حالة PCENT_ميدان:
        حالة ITOTAL_ميدان:
        حالة IUSED_ميدان:
        حالة IAVAIL_ميدان:
        حالة IPCENT_ميدان:
        حالة هدف_ميدان:
        حالة FILE_ميدان:
          alloc_ميدان (ميدان, عدم);
          اكسر;

        حالة سائيز_ميدان:
          alloc_ميدان (ميدان, N_("Size"));
          اكسر;

        حالة AVAIL_ميدان:
          alloc_ميدان (ميدان, N_("Avail"));
          اكسر;

        حالة INVALID_ميدان:
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
get_ميدان_list (فراغ)
{
  بدل (سرو_گهڻائي)
    {
    حالة DEFAULT_گهڻائي:
      alloc_ميدان (ذريعو_ميدان, عدم);
      إذا (print_type)
        alloc_ميدان (FSTYPE_ميدان, عدم);
      alloc_ميدان (سائيز_ميدان,   عدم);
      alloc_ميدان (استعمال_ٿيل_ميدان,   عدم);
      alloc_ميدان (AVAIL_ميدان,  عدم);
      alloc_ميدان (PCENT_ميدان,  عدم);
      alloc_ميدان (هدف_ميدان, عدم);
      اكسر;

    حالة HUMAN_گهڻائي:
      alloc_ميدان (ذريعو_ميدان, عدم);
      إذا (print_type)
        alloc_ميدان (FSTYPE_ميدان, عدم);

      alloc_ميدان (سائيز_ميدان,   N_("Size"));
      alloc_ميدان (استعمال_ٿيل_ميدان,   عدم);
      alloc_ميدان (AVAIL_ميدان,  N_("Avail"));
      alloc_ميدان (PCENT_ميدان,  عدم);
      alloc_ميدان (هدف_ميدان, عدم);
      اكسر;

    حالة INODES_گهڻائي:
      alloc_ميدان (ذريعو_ميدان, عدم);
      إذا (print_type)
        alloc_ميدان (FSTYPE_ميدان, عدم);
      alloc_ميدان (ITOTAL_ميدان,  عدم);
      alloc_ميدان (IUSED_ميدان,   عدم);
      alloc_ميدان (IAVAIL_ميدان,  عدم);
      alloc_ميدان (IPCENT_ميدان,  عدم);
      alloc_ميدان (هدف_ميدان,  عدم);
      اكسر;

    حالة POSIX_گهڻائي:
      alloc_ميدان (ذريعو_ميدان, عدم);
      إذا (print_type)
        alloc_ميدان (FSTYPE_ميدان, عدم);
      alloc_ميدان (سائيز_ميدان,   عدم);
      alloc_ميدان (استعمال_ٿيل_ميدان,   عدم);
      alloc_ميدان (AVAIL_ميدان,  عدم);
      alloc_ميدان (PCENT_ميدان,  N_("Capacity"));
      alloc_ميدان (هدف_ميدان, عدم);
      اكسر;

    حالة OUTPUT_گهڻائي:
      إذا (!جدول.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_دليلون_string);
        }
      اكسر;

    افتراضي:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساكن فراغ
get_سرو (فراغ)
{
  alloc_جدول_صف ();

  لكل (idx_t کانو = 0; کانو < جدول.ncolumns; کانو++)
    {
      حرف *cell;
      حرف ثابت *سرو = _(جدول.columns[کانو]->caption);

      إذا (جدول.columns[کانو]->ميدان == سائيز_ميدان
          && (سرو_گهڻائي == DEFAULT_گهڻائي
              || (سرو_گهڻائي == OUTPUT_گهڻائي
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحيح opts = (human_suppress_نقطو_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_گروهه_digits | human_بنياد_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_بلاڪ_سائيز;
          uintmax_t q1024 = output_بلاڪ_سائيز;
          bool divisible_by_1000;
          bool divisible_by_1024;

          افعل
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          طالما (divisible_by_1000 & divisible_by_1024);

          إذا (divisible_by_1000 < divisible_by_1024)
            opts |= human_بنياد_1024;
          إذا (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_بنياد_1024;
          إذا (! (opts & human_بنياد_1024))
            opts |= human_B;

          حرف *انگ = human_readable (output_بلاڪ_سائيز, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          سرو = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), انگ, سرو);
        }
      وإلا إذا (سرو_گهڻائي == POSIX_گهڻائي
               && جدول.columns[کانو]->ميدان == سائيز_ميدان)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_بلاڪ_سائيز, سرو);
        }
      وإلا
        cell = xstrdup (سرو);

      replace_problematic_chars (cell);

      جدول.صفون[جدول.nrows - 1][کانو] = cell;

      صحيح cell_ويڪر = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[کانو]->ويڪر = MAX (جدول.columns[کانو]->ويڪر, cell_ويڪر);
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
       fsp = fsp->fs_ايندڙ)
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
       fsp = fsp->fs_ايندڙ)
    إذا (streq (fstype, fsp->fs_name))
      أرجع true;
  أرجع false;
}

ساكن size_t
devlist_hash (فراغ ثابت *x, size_t جدول_سائيز)
{
  بنية devlist ثابت *p = x;
  أرجع (uintmax_t) {p->dev_انگ} % جدول_سائيز;
}

ساكن bool
devlist_compare (فراغ ثابت *x, فراغ ثابت *y)
{
  بنية devlist ثابت *a = x;
  بنية devlist ثابت *b = y;
  أرجع a->dev_انگ == b->dev_انگ;
}

ساكن بنية devlist *
devlist_for_dev (dev_t dev)
{
  إذا (devlist_جدول == عدم)
    أرجع عدم;
  بنية devlist dev_داخلا;
  dev_داخلا.dev_انگ = dev;

  بنية devlist *مليو = hash_lookup (devlist_جدول, &dev_داخلا);
  إذا (مليو == عدم)
    أرجع عدم;

  /* Return the last devlist entry we have seen with this dev_num */
  أرجع مليو->ڏٺل_آخري;
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
  صحيح mount_list_سائيز = 0;

  لكل (بنية mount_داخلا *me = mount_list; me; me = me->me_ايندڙ)
    mount_list_سائيز++;

  devlist_جدول = hash_initialize (mount_list_سائيز, عدم,
                                   devlist_hash, devlist_compare, عدم);
  إذا (devlist_جدول == عدم)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  لكل (بنية mount_داخلا *me = mount_list; me;)
    {
      بنية stat buf;
      بنية mount_داخلا *discard_me = عدم;
      بنية devlist *آخري_ڏٺل_dev = عدم, *ڏٺل_dev = عدم;

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
          آخري_ڏٺل_dev = ڏٺل_dev = devlist_for_dev (buf.st_dev);

          لكل (; ڏٺل_dev && ! discard_me; ڏٺل_dev = ڏٺل_dev->ايندڙ_same_dev)
            {
              bool هدف_nearer_پاڙ = لڙي_ڊيگهه (ڏٺل_dev->me->me_mountdir)
                                        > لڙي_ڊيگهه (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool ذريعو_هيٺ_پاڙ = ڏٺل_dev->me->me_mntroot != عدم
                                       && me->me_mntroot != عدم
                                       && (لڙي_ڊيگهه (ڏٺل_dev->me->me_mntroot)
                                           < لڙي_ڊيگهه (me->me_mntroot));
              إذا (! print_grand_ڪل
                  && me->me_remote && ڏٺل_dev->me->me_remote
                  && ! streq (ڏٺل_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              وإلا إذا ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (ڏٺل_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (هدف_nearer_پاڙ && ! ذريعو_هيٺ_پاڙ)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (ڏٺل_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     ڏٺل_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = ڏٺل_dev->me;
                  ڏٺل_dev->me = me;
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
          me = me->me_ايندڙ;
          إذا (! devices_only)
            free_mount_داخلا (discard_me);
        }
      وإلا
        {
          /* Add the device number to the device_table.  */
          بنية devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_انگ = buf.st_dev;
          devlist->ايندڙ_same_dev = آخري_ڏٺل_dev;
          devlist->ايندڙ = device_list;
          device_list = devlist;

          بنية devlist *hash_داخلا = hash_وجهو (devlist_جدول, devlist);
          إذا (hash_داخلا == عدم)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_داخلا->ڏٺل_آخري = devlist;

          me = me->me_ايندڙ;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  إذا (! devices_only)
    {
      mount_list = عدم;
      طالما (device_list)
        {
          /* Add the mount entry.  */
          بنية mount_داخلا *me = device_list->me;
          me->me_ايندڙ = mount_list;
          mount_list = me;
          بنية devlist *ايندڙ = device_list->ايندڙ;
          حرر (device_list);
          device_list = ايندڙ;
        }
      hash_free (devlist_جدول);
      devlist_جدول = عدم;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_خالص
ساكن بنية mount_داخلا ثابت *
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
معلوم_قدر (uintmax_t n)
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
             uintmax_t داخلا_units, uintmax_t output_units)
{
  إذا (! معلوم_قدر (n) && !negative)
    أرجع "-";
  وإلا
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, داخلا_units, output_units);
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
جوڙيو_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
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
  size_t len = لڙي_ڊيگهه (s);
  أرجع (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساكن فراغ
get_ميدان_قدرون (بنية ميدان_قدرون_t *bv,
                  بنية ميدان_قدرون_t *iv,
                  ثابت بنية fs_usage *fsu)
{
  /* Inode values.  */
  iv->داخلا_units = iv->output_units = 1;
  iv->ڪل = fsu->fsu_files;
  iv->موجود = iv->موجود_منزل_پاڙ = fsu->fsu_ffree;
  iv->negate_موجود = false;

  iv->استعمال_ٿيل = UINTMAX_MAX;
  iv->negate_استعمال_ٿيل = false;
  إذا (معلوم_قدر (iv->ڪل) && معلوم_قدر (iv->موجود_منزل_پاڙ))
    {
      iv->استعمال_ٿيل = iv->ڪل - iv->موجود_منزل_پاڙ;
      iv->negate_استعمال_ٿيل = (iv->ڪل < iv->موجود_منزل_پاڙ);
    }

  /* Block values.  */
  bv->داخلا_units = fsu->fsu_blocksize;
  bv->output_units = output_بلاڪ_سائيز;
  bv->ڪل = fsu->fsu_بلاڪون;
  bv->موجود = fsu->fsu_bavail;
  bv->موجود_منزل_پاڙ = fsu->fsu_bfree;
  bv->negate_موجود = (fsu->fsu_bavail_چوٽي_ٻٽو_انگ_سيٽ
                         && معلوم_قدر (fsu->fsu_bavail));

  bv->استعمال_ٿيل = UINTMAX_MAX;
  bv->negate_استعمال_ٿيل = false;
  إذا (معلوم_قدر (bv->ڪل) && معلوم_قدر (bv->موجود_منزل_پاڙ))
    {
      bv->استعمال_ٿيل = bv->ڪل - bv->موجود_منزل_پاڙ;
      bv->negate_استعمال_ٿيل = (bv->ڪل < bv->موجود_منزل_پاڙ);
    }
}

/* Add block and inode values to grand total.  */
ساكن فراغ
جوڙيو_منزل_grand_ڪل (بنية ميدان_قدرون_t *bv, بنية ميدان_قدرون_t *iv)
{
  إذا (معلوم_قدر (iv->ڪل))
    grand_fsu.fsu_files += iv->ڪل;
  إذا (معلوم_قدر (iv->موجود))
    grand_fsu.fsu_ffree += iv->موجود;

  إذا (معلوم_قدر (bv->ڪل))
    grand_fsu.fsu_بلاڪون += bv->داخلا_units * bv->ڪل;
  إذا (معلوم_قدر (bv->موجود_منزل_پاڙ))
    grand_fsu.fsu_bfree += bv->داخلا_units * bv->موجود_منزل_پاڙ;
  إذا (معلوم_قدر (bv->موجود))
    جوڙيو_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_چوٽي_ٻٽو_انگ_سيٽ,
                            bv->داخلا_units * bv->موجود,
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
get_dev (حرف ثابت *device, حرف ثابت *mount_نقطو, حرف ثابت *file,
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
  إذا (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_نقطو))
    أرجع;

  إذا (!stat_file)
    stat_file = mount_نقطو;

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
          fsu.fsu_bavail_چوٽي_ٻٽو_انگ_سيٽ = false;
          fsu.fsu_blocksize = fsu.fsu_بلاڪون = fsu.fsu_bfree =
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
          بنية mount_داخلا ثابت * dev_me = me_for_dev (sb.st_dev);
          إذا (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_چوٽي_ٻٽو_انگ_سيٽ = false;
              fsu.fsu_blocksize = fsu.fsu_بلاڪون = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  إذا (fsu.fsu_بلاڪون == 0 && !show_all_fs && !show_listed_fs)
    أرجع;

  إذا (! force_fsu)
    file_systems_processed = true;

  alloc_جدول_صف ();

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
      && (resolved_dev = canonicalize_filename_گهڻائي (dev_name, CAN_EXISTING)))
    {
      حرر (dev_name);
      dev_name = resolved_dev;
    }

  إذا (! fstype)
    fstype = "-";		/* unknown */

  بنية ميدان_قدرون_t بلاڪ_قدرون;
  بنية ميدان_قدرون_t inode_قدرون;
  get_ميدان_قدرون (&بلاڪ_قدرون, &inode_قدرون, &fsu);

  /* Add to grand total unless processing grand total line.  */
  إذا (print_grand_ڪل && ! force_fsu)
    جوڙيو_منزل_grand_ڪل (&بلاڪ_قدرون, &inode_قدرون);

  لكل (idx_t کانو = 0; کانو < جدول.ncolumns; کانو++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      بنية ميدان_قدرون_t *v;
      بدل (جدول.columns[کانو]->ميدان_type)
        {
        حالة بلاڪ_FLD:
          v = &بلاڪ_قدرون;
          اكسر;
        حالة INODE_FLD:
          v = &inode_قدرون;
          اكسر;
        حالة OTHER_FLD:
          v = عدم;
          اكسر;
        افتراضي:
          affirm (!"bad field_type");
        }

      بدل (جدول.columns[کانو]->ميدان)
        {
        حالة ذريعو_ميدان:
          cell = xstrdup (dev_name);
          اكسر;

        حالة FSTYPE_ميدان:
          cell = xstrdup (fstype);
          اكسر;

        حالة سائيز_ميدان:
        حالة ITOTAL_ميدان:
          cell = xstrdup (df_readable (false, v->ڪل, buf,
                                       v->داخلا_units, v->output_units));
          اكسر;

        حالة استعمال_ٿيل_ميدان:
        حالة IUSED_ميدان:
          cell = xstrdup (df_readable (v->negate_استعمال_ٿيل, v->استعمال_ٿيل, buf,
                                       v->داخلا_units, v->output_units));
          اكسر;

        حالة AVAIL_ميدان:
        حالة IAVAIL_ميدان:
          cell = xstrdup (df_readable (v->negate_موجود, v->موجود, buf,
                                       v->داخلا_units, v->output_units));
          اكسر;

        حالة PCENT_ميدان:
        حالة IPCENT_ميدان:
          {
            مزدوج pct = -1;
            إذا (! معلوم_قدر (v->استعمال_ٿيل) || ! معلوم_قدر (v->موجود))
              ;
            وإلا إذا (!v->negate_استعمال_ٿيل
                     && v->استعمال_ٿيل <= UINTMAX_MAX / 100
                     && v->استعمال_ٿيل + v->موجود != 0
                     && (v->استعمال_ٿيل + v->موجود < v->استعمال_ٿيل)
                     == v->negate_موجود)
              {
                uintmax_t u100 = v->استعمال_ٿيل * 100;
                uintmax_t nonroot_ڪل = v->استعمال_ٿيل + v->موجود;
                pct = u100 / nonroot_ڪل + (u100 % nonroot_ڪل != 0);
              }
            وإلا
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                مزدوج u = v->negate_استعمال_ٿيل ? - (مزدوج) - v->استعمال_ٿيل : v->استعمال_ٿيل;
                مزدوج a = v->negate_موجود
                           ? - (مزدوج) - v->موجود : v->موجود;
                مزدوج nonroot_ڪل = u + a;
                إذا (nonroot_ڪل)
                  {
                    ڊگهو صحيح lipct = pct = u * 100 / nonroot_ڪل;
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

        حالة FILE_ميدان:
          cell = xstrdup (file);
          اكسر;

        حالة هدف_ميدان:
#جيڪڏهن_وصف_ٿيل HIDE_AUTOMOUNT_اڳياڙي
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          إذا (STRNCMP_LIT (mount_نقطو, "/auto/") == 0)
            mount_نقطو += 5;
          وإلا إذا (STRNCMP_LIT (mount_نقطو, "/tmp_mnt/") == 0)
            mount_نقطو += 8;
#جيڪڏهن_پورو
          cell = xstrdup (mount_نقطو);
          اكسر;

        حالة INVALID_ميدان:
        افتراضي:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحيح cell_ويڪر = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[کانو]->ويڪر = MAX (جدول.columns[کانو]->ويڪر, cell_ويڪر);
      جدول.صفون[جدول.nrows - 1][کانو] = cell;
    }
  حرر (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساكن حرف *
آخري_device_for_mount (حرف ثابت *mount)
{
  بنية mount_داخلا ثابت *le = عدم;

  لكل (بنية mount_داخلا ثابت *me = mount_list; me; me = me->me_ايندڙ)
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
  بنية mount_داخلا ثابت *بهترين_ميلاپ = عدم;
  bool بهترين_ميلاپ_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  إذا (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t بهترين_ميلاپ_len = SIZE_MAX;
  لكل (بنية mount_داخلا ثابت *me = mount_list; me; me = me->me_ايندڙ)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      إذا (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      إذا (streq (device, devname))
        {
          حرف *آخري_device = آخري_device_for_mount (me->me_mountdir);
          eclipsed_device = آخري_device && ! streq (آخري_device, devname);
          size_t len = لڙي_ڊيگهه (me->me_mountdir);

          إذا (! eclipsed_device
              && (! بهترين_ميلاپ_accessible || len < بهترين_ميلاپ_len))
            {
              بنية stat device_stats;
              bool this_ميلاپ_accessible = false;

              إذا (stat (me->me_mountdir, &device_stats) == 0)
                بهترين_ميلاپ_accessible = this_ميلاپ_accessible = true;

              إذا (this_ميلاپ_accessible
                  || (! بهترين_ميلاپ_accessible && len < بهترين_ميلاپ_len))
                {
                  بهترين_ميلاپ = me;
                  إذا (len == 1) /* Traditional root.  */
                    {
                      حرر (آخري_device);
                      حرر (canon_dev);
                      اكسر;
                    }
                  وإلا
                    بهترين_ميلاپ_len = len;
                }
            }

          حرر (آخري_device);
        }

      حرر (canon_dev);
    }

  حرر (resolved);

  إذا (بهترين_ميلاپ)
    {
      get_dev (بهترين_ميلاپ->me_devname, بهترين_ميلاپ->me_mountdir, file, عدم,
               بهترين_ميلاپ->me_type, بهترين_ميلاپ->me_dummy,
               بهترين_ميلاپ->me_remote, عدم, false);
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
get_نقطو (حرف ثابت *نقطو, ثابت بنية stat *statp)
{
  بنية stat device_stats;
  بنية mount_داخلا ثابت *بهترين_ميلاپ = عدم;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (نقطو);
  إذا (resolved && resolved[0] == '/')
    {
      size_t resolved_len = لڙي_ڊيگهه (resolved);
      size_t بهترين_ميلاپ_len = 0;

      لكل (بنية mount_داخلا *me = mount_list; me; me = me->me_ايندڙ)
        {
          إذا (!streq (me->me_type, "lofs")
              && (!بهترين_ميلاپ || بهترين_ميلاپ->me_dummy || !me->me_dummy))
            {
              size_t len = لڙي_ڊيگهه (me->me_mountdir);
              إذا (بهترين_ميلاپ_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  بهترين_ميلاپ = me;
                  بهترين_ميلاپ_len = len;
                }
            }
        }
    }
  حرر (resolved);
  إذا (بهترين_ميلاپ
      && (stat (بهترين_ميلاپ->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    بهترين_ميلاپ = عدم;

  إذا (! بهترين_ميلاپ)
    لكل (بنية mount_داخلا *me = mount_list; me; me = me->me_ايندڙ)
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
            && (!بهترين_ميلاپ || بهترين_ميلاپ->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            إذا (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            وإلا
              بهترين_ميلاپ = me;
          }
      }

  إذا (بهترين_ميلاپ)
    get_dev (بهترين_ميلاپ->me_devname, بهترين_ميلاپ->me_mountdir, نقطو, نقطو,
             بهترين_ميلاپ->me_type, بهترين_ميلاپ->me_dummy, بهترين_ميلاپ->me_remote,
             عدم, false);
  وإلا
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = ڳوليو_mount_نقطو (نقطو, statp);
      إذا (mp)
        {
          get_dev (عدم, mp, نقطو, عدم, عدم,
                   false, false, عدم, false);
          حرر (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساكن فراغ
get_داخلا (حرف ثابت *name, بنية stat ثابت *statp)
{
  إذا ((S_ISBLK (statp->st_گهڻائي) || S_ISCHR (statp->st_گهڻائي))
      && get_device (name))
    أرجع;

  get_نقطو (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساكن فراغ
get_all_entries (فراغ)
{
  filter_mount_list (show_all_fs);

  لكل (بنية mount_داخلا *me = mount_list; me; me = me->me_ايندڙ)
    get_dev (me->me_devname, me->me_mountdir, عدم, عدم, me->me_type,
             me->me_dummy, me->me_remote, عدم, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساكن فراغ
جوڙيو_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_ايندڙ = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساكن فراغ
جوڙيو_excluded_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_ايندڙ = fs_exclude_list;
  fs_exclude_list = fsp;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
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
  -B, --بلاڪ-سائيز=سائيز_2\n\
         scale sizes by سائيز_2 before printing them; see سائيز_2 format هيٺ;\n\
         E.g., '-BM' prints sizes اندريون units جو 1,048,576 بائيٽون\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes اندريون powers جو 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes اندريون powers جو 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead جو بلاڪ usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاڪ-سائيز=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         حد_بندي listing منزل local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         افعل not invoke sync before getting usage info (افتراضي)\n\
"));
      oputs (_("\
      --output[=ميدان_LIST]\n\
         use the output format defined by ميدان_LIST,\n\
         or print all fields إذا ميدان_LIST is omitted\n\
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
      --ڪل\n\
         elide all entries insignificant منزل موجود space,\n\
         and produce a grand ڪل\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         حد_بندي listing منزل file systems جو type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         حد_بندي listing منزل file systems not جو type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_سائيز_note ();
      fputs (_("\n\
ميدان_LIST is a comma-separated list جو columns منزل be included.  Valid\n\
ميدان نالا are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info صفحو).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  بنية stat *stats = عدم;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  حرف ثابت *نياپو_mut_excl = _("options %s and %s are mutually exclusive");

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
                                                 &output_بلاڪ_سائيز);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;
        حالة 'i':
          إذا (سرو_گهڻائي == OUTPUT_گهڻائي)
            {
              error (0, 0, نياپو_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          سرو_گهڻائي = INODES_گهڻائي;
          اكسر;
        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_بنياد_1024;
          output_بلاڪ_سائيز = 1;
          اكسر;
        حالة 'H':
          human_output_opts = human_autoscale | human_SI;
          output_بلاڪ_سائيز = 1;
          اكسر;
        حالة 'k':
          human_output_opts = 0;
          output_بلاڪ_سائيز = 1024;
          اكسر;
        حالة 'l':
          show_local_fs = true;
          اكسر;
        حالة 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_بلاڪ_سائيز = 1024 * 1024;
          اكسر;
        حالة 'T':
          إذا (سرو_گهڻائي == OUTPUT_گهڻائي)
            {
              error (0, 0, نياپو_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          اكسر;
        حالة 'P':
          إذا (سرو_گهڻائي == OUTPUT_گهڻائي)
            {
              error (0, 0, نياپو_mut_excl, "-P", "--output");
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
          جوڙيو_fs_type (optarg);
          اكسر;

        حالة 'v':		/* For SysV compatibility.  */
          /* ignore */
          اكسر;
        حالة 'x':
          جوڙيو_excluded_fs_type (optarg);
          اكسر;

        حالة OUTPUT_OPTION:
          إذا (سرو_گهڻائي == INODES_گهڻائي)
            {
              error (0, 0, نياپو_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (posix_format && سرو_گهڻائي == DEFAULT_گهڻائي)
            {
              error (0, 0, نياپو_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (print_type)
            {
              error (0, 0, نياپو_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          سرو_گهڻائي = OUTPUT_گهڻائي;
          إذا (optarg)
            decode_output_arg (optarg);
          اكسر;

        حالة ڪل_OPTION:
          print_grand_ڪل = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (human_output_opts == -1)
    {
      إذا (posix_format)
        {
          human_output_opts = 0;
          output_بلاڪ_سائيز = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      وإلا
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_بلاڪ_سائيز);
    }

  إذا (سرو_گهڻائي == INODES_گهڻائي || سرو_گهڻائي == OUTPUT_گهڻائي)
    ;
  وإلا إذا (human_output_opts & human_autoscale)
    سرو_گهڻائي = HUMAN_گهڻائي;
  وإلا إذا (posix_format)
    سرو_گهڻائي = POSIX_گهڻائي;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool ميلاپ = false;
    لكل (بنية fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_ايندڙ)
      {
        لكل (بنية fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_ايندڙ)
          {
            إذا (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                ميلاپ = true;
                اكسر;
              }
          }
      }
    إذا (ميلاپ)
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
                            || ميدان_معلومات_2[FSTYPE_ميدان].استعمال_ٿيل
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

  get_ميدان_list ();
  get_سرو ();

  إذا (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      لكل (صحيح i = optind; i < argc; ++i)
        إذا (argv[i])
          get_داخلا (argv[i], &stats[i - optind]);
    }
  وإلا
    get_all_entries ();

  إذا (file_systems_processed)
    {
      إذا (print_grand_ڪل)
        get_dev ("total",
                 (ميدان_معلومات_2[ذريعو_ميدان].استعمال_ٿيل ? "-" : "total"),
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
