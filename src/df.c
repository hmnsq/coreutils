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
#تعریف_کرو پروگرام_NAME "df"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

بنية devlist
{
  dev_t dev_عدد;
  بنية mount_اندراج *me;
  بنية devlist *اگلا;
  بنية devlist *اگلا_same_dev;
  بنية devlist *تکیا_آخری; /* valid for hashed devlist entries only */
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
ساكن uintmax_t output_بلاک_ناپ;

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
  بنية fs_type_list *fs_اگلا;
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
ساكن بنية mount_اندراج *mount_list;

/* If true, print file system type as well.  */
ساكن bool print_type;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_کل;

/* Grand total data.  */
ساكن بنية fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساكن تعداد
{
  DEFAULT_منوال,
  INODES_منوال,
  HUMAN_منوال,
  POSIX_منوال,
  OUTPUT_منوال
} سرخی_منوال = DEFAULT_منوال;

/* Displayable fields.  */
تعريفنوع تعداد
{
  سروت_میدان, /* file system */
  FSTYPE_میدان, /* FS type */
  ناپ_میدان,   /* FS size */
  ورتیا_میدان,   /* FS size used  */
  AVAIL_میدان,  /* FS size available */
  PCENT_میدان,  /* percent used */
  ITOTAL_میدان, /* inode total */
  IUSED_میدان,  /* inodes used */
  IAVAIL_میدان, /* inodes available */
  IPCENT_میدان, /* inodes used in percent */
  نشانہ_میدان, /* mount point */
  FILE_میدان,   /* specified file name */
  INVALID_میدان /* validation marker */
} display_میدان_t;

/* Flag if a field contains a block, an inode or another value.  */
تعريفنوع تعداد
{
  بلاک_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} میدان_type_t;

/* Attributes of a display field.  */
بنية میدان_انکڑے_t
{
  display_میدان_t میدان;
  حرف ثابت *arg;
  میدان_type_t میدان_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحيح چوڑائی;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_دایاں;   /* Whether to right-align columns, not left-align.  */
  bool ورتیا;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساكن بنية میدان_انکڑے_t میدان_انکڑے_2[] = {
  [سروت_میدان] = { سروت_میدان,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_میدان] = { FSTYPE_میدان,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [ناپ_میدان] = { ناپ_میدان,
    "size",   بلاک_FLD, N_("blocks"),      5, true, false },

  [ورتیا_میدان] = { ورتیا_میدان,
    "used",   بلاک_FLD, N_("Used"),        5, true, false },

  [AVAIL_میدان] = { AVAIL_میدان,
    "avail",  بلاک_FLD, N_("Available"),   5, true, false },

  [PCENT_میدان] = { PCENT_میدان,
    "pcent",  بلاک_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_میدان] = { ITOTAL_میدان,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_میدان] = { IUSED_میدان,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_میدان] = { IAVAIL_میدان,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_میدان] = { IPCENT_میدان,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [نشانہ_میدان] = { نشانہ_میدان,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_میدان] = { FILE_میدان,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ساكن حرف ثابت *all_دلیلاں_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساكن بنية
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***صفاں COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  بنية میدان_انکڑے_t **columns COUNTED_BY (ncolumns);
} جدول;

/* Field values.  */
بنية میدان_اقدار_t
{
  uintmax_t داخلہ_units;
  uintmax_t output_units;
  uintmax_t کل;
  uintmax_t دستیاب;
  bool negate_دستیاب;
  uintmax_t دستیاب_منزل_جڑ;
  uintmax_t ورتیا;
  bool negate_ورتیا;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  کل_OPTION,
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
  {"total", no_دلیل, عدم, کل_OPTION},
  {"type", required_دلیل, عدم, 't'},
  {"exclude-type", required_دلیل, عدم, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
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
  حرف *srcend = cell + لڑی_لمبائی (cell);
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
  ساكن صحيح tty_باہرلا = -1;
  إذا (tty_باہرلا < 0)
    tty_باہرلا = isatty (STDOUT_FILENO);

  (tty_باہرلا ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساكن فراغ
alloc_جدول_صف (فراغ)
{
  إذا (جدول.nrows == جدول.nrows_alloc)
    جدول.صفاں = xpalloc (جدول.صفاں, &جدول.nrows_alloc, 1, -1,
                          حجم *جدول.صفاں);
  idx_t ثابت صف_idx = جدول.nrows++;
  جدول.صفاں[صف_idx] = xinmalloc (جدول.ncolumns, حجم *جدول.صفاں[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساكن فراغ
print_جدول (فراغ)
{
  لكل (idx_t صف = 0; صف < جدول.nrows; صف++)
    {
      لكل (idx_t خانہ = 0; خانہ < جدول.ncolumns; خانہ++)
        {
          حرف *cell = جدول.صفاں[صف][خانہ];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          إذا (خانہ != 0)
            حرف_لکھو (' ');

          صحيح چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
          صحيح fill = چوڑائی < 0 ? 0 : جدول.columns[خانہ]->چوڑائی - چوڑائی;
          إذا (جدول.columns[خانہ]->align_دایاں)
            لكل (; 0 < fill; fill--)
              حرف_لکھو (' ');
          fputs (cell, stdout);
          إذا (خانہ + 1 < جدول.ncolumns)
            لكل (; 0 < fill; fill--)
              حرف_لکھو (' ');
        }
      حرف_لکھو ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساكن فراغ
alloc_میدان (صحيح f, حرف ثابت *c)
{
  إذا (جدول.ncolumns == جدول.ncolumns_alloc)
    جدول.columns = xpalloc (جدول.columns, &جدول.ncolumns_alloc, 1, -1,
                             حجم *جدول.columns);
  idx_t ثابت کالم_idx = جدول.ncolumns++;
  جدول.columns[کالم_idx] = &میدان_انکڑے_2[f];
  إذا (c != عدم)
    میدان_انکڑے_2[f].caption = c;

  affirm (!میدان_انکڑے_2[f].ورتیا);

  /* Mark field as used.  */
  میدان_انکڑے_2[f].ورتیا = true;
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
      display_میدان_t میدان = INVALID_میدان;
      لكل (idx_t i = 0; i < countof (میدان_انکڑے_2); i++)
        {
          إذا (streq (میدان_انکڑے_2[i].arg, s))
            {
              میدان = i;
              اكسر;
            }
        }
      إذا (میدان == INVALID_میدان)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      إذا (میدان_انکڑے_2[میدان].ورتیا)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (میدان_انکڑے_2[میدان].arg));
          usage (EXIT_FAILURE);
        }

      بدل (میدان)
        {
        حالة سروت_میدان:
        حالة FSTYPE_میدان:
        حالة ورتیا_میدان:
        حالة PCENT_میدان:
        حالة ITOTAL_میدان:
        حالة IUSED_میدان:
        حالة IAVAIL_میدان:
        حالة IPCENT_میدان:
        حالة نشانہ_میدان:
        حالة FILE_میدان:
          alloc_میدان (میدان, عدم);
          اكسر;

        حالة ناپ_میدان:
          alloc_میدان (میدان, N_("Size"));
          اكسر;

        حالة AVAIL_میدان:
          alloc_میدان (میدان, N_("Avail"));
          اكسر;

        حالة INVALID_میدان:
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
get_میدان_list (فراغ)
{
  بدل (سرخی_منوال)
    {
    حالة DEFAULT_منوال:
      alloc_میدان (سروت_میدان, عدم);
      إذا (print_type)
        alloc_میدان (FSTYPE_میدان, عدم);
      alloc_میدان (ناپ_میدان,   عدم);
      alloc_میدان (ورتیا_میدان,   عدم);
      alloc_میدان (AVAIL_میدان,  عدم);
      alloc_میدان (PCENT_میدان,  عدم);
      alloc_میدان (نشانہ_میدان, عدم);
      اكسر;

    حالة HUMAN_منوال:
      alloc_میدان (سروت_میدان, عدم);
      إذا (print_type)
        alloc_میدان (FSTYPE_میدان, عدم);

      alloc_میدان (ناپ_میدان,   N_("Size"));
      alloc_میدان (ورتیا_میدان,   عدم);
      alloc_میدان (AVAIL_میدان,  N_("Avail"));
      alloc_میدان (PCENT_میدان,  عدم);
      alloc_میدان (نشانہ_میدان, عدم);
      اكسر;

    حالة INODES_منوال:
      alloc_میدان (سروت_میدان, عدم);
      إذا (print_type)
        alloc_میدان (FSTYPE_میدان, عدم);
      alloc_میدان (ITOTAL_میدان,  عدم);
      alloc_میدان (IUSED_میدان,   عدم);
      alloc_میدان (IAVAIL_میدان,  عدم);
      alloc_میدان (IPCENT_میدان,  عدم);
      alloc_میدان (نشانہ_میدان,  عدم);
      اكسر;

    حالة POSIX_منوال:
      alloc_میدان (سروت_میدان, عدم);
      إذا (print_type)
        alloc_میدان (FSTYPE_میدان, عدم);
      alloc_میدان (ناپ_میدان,   عدم);
      alloc_میدان (ورتیا_میدان,   عدم);
      alloc_میدان (AVAIL_میدان,  عدم);
      alloc_میدان (PCENT_میدان,  N_("Capacity"));
      alloc_میدان (نشانہ_میدان, عدم);
      اكسر;

    حالة OUTPUT_منوال:
      إذا (!جدول.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_دلیلاں_string);
        }
      اكسر;

    افتراضي:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساكن فراغ
get_سرخی (فراغ)
{
  alloc_جدول_صف ();

  لكل (idx_t خانہ = 0; خانہ < جدول.ncolumns; خانہ++)
    {
      حرف *cell;
      حرف ثابت *سرخی = _(جدول.columns[خانہ]->caption);

      إذا (جدول.columns[خانہ]->میدان == ناپ_میدان
          && (سرخی_منوال == DEFAULT_منوال
              || (سرخی_منوال == OUTPUT_منوال
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحيح opts = (human_suppress_نقطہ_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_گروہ_digits | human_ادھار_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_بلاک_ناپ;
          uintmax_t q1024 = output_بلاک_ناپ;
          bool divisible_by_1000;
          bool divisible_by_1024;

          افعل
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          طالما (divisible_by_1000 & divisible_by_1024);

          إذا (divisible_by_1000 < divisible_by_1024)
            opts |= human_ادھار_1024;
          إذا (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_ادھار_1024;
          إذا (! (opts & human_ادھار_1024))
            opts |= human_B;

          حرف *عدد = human_readable (output_بلاک_ناپ, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          سرخی = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), عدد, سرخی);
        }
      وإلا إذا (سرخی_منوال == POSIX_منوال
               && جدول.columns[خانہ]->میدان == ناپ_میدان)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_بلاک_ناپ, سرخی);
        }
      وإلا
        cell = xstrdup (سرخی);

      replace_problematic_chars (cell);

      جدول.صفاں[جدول.nrows - 1][خانہ] = cell;

      صحيح cell_چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[خانہ]->چوڑائی = MAX (جدول.columns[خانہ]->چوڑائی, cell_چوڑائی);
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
       fsp = fsp->fs_اگلا)
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
       fsp = fsp->fs_اگلا)
    إذا (streq (fstype, fsp->fs_name))
      أرجع true;
  أرجع false;
}

ساكن size_t
devlist_hash (فراغ ثابت *x, size_t جدول_ناپ)
{
  بنية devlist ثابت *p = x;
  أرجع (uintmax_t) {p->dev_عدد} % جدول_ناپ;
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
  بنية devlist dev_اندراج;
  dev_اندراج.dev_عدد = dev;

  بنية devlist *لبھیا = hash_lookup (devlist_جدول, &dev_اندراج);
  إذا (لبھیا == عدم)
    أرجع عدم;

  /* Return the last devlist entry we have seen with this dev_num */
  أرجع لبھیا->تکیا_آخری;
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
  صحيح mount_list_ناپ = 0;

  لكل (بنية mount_اندراج *me = mount_list; me; me = me->me_اگلا)
    mount_list_ناپ++;

  devlist_جدول = hash_initialize (mount_list_ناپ, عدم,
                                   devlist_hash, devlist_compare, عدم);
  إذا (devlist_جدول == عدم)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  لكل (بنية mount_اندراج *me = mount_list; me;)
    {
      بنية stat buf;
      بنية mount_اندراج *discard_me = عدم;
      بنية devlist *آخری_تکیا_dev = عدم, *تکیا_dev = عدم;

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
          آخری_تکیا_dev = تکیا_dev = devlist_for_dev (buf.st_dev);

          لكل (; تکیا_dev && ! discard_me; تکیا_dev = تکیا_dev->اگلا_same_dev)
            {
              bool نشانہ_nearer_جڑ = لڑی_لمبائی (تکیا_dev->me->me_mountdir)
                                        > لڑی_لمبائی (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool سروت_ہیٹھاں_جڑ = تکیا_dev->me->me_mntroot != عدم
                                       && me->me_mntroot != عدم
                                       && (لڑی_لمبائی (تکیا_dev->me->me_mntroot)
                                           < لڑی_لمبائی (me->me_mntroot));
              إذا (! print_grand_کل
                  && me->me_remote && تکیا_dev->me->me_remote
                  && ! streq (تکیا_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              وإلا إذا ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (تکیا_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (نشانہ_nearer_جڑ && ! سروت_ہیٹھاں_جڑ)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (تکیا_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     تکیا_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = تکیا_dev->me;
                  تکیا_dev->me = me;
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
          me = me->me_اگلا;
          إذا (! devices_only)
            free_mount_اندراج (discard_me);
        }
      وإلا
        {
          /* Add the device number to the device_table.  */
          بنية devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_عدد = buf.st_dev;
          devlist->اگلا_same_dev = آخری_تکیا_dev;
          devlist->اگلا = device_list;
          device_list = devlist;

          بنية devlist *hash_اندراج = hash_داخل_کرو (devlist_جدول, devlist);
          إذا (hash_اندراج == عدم)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_اندراج->تکیا_آخری = devlist;

          me = me->me_اگلا;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  إذا (! devices_only)
    {
      mount_list = عدم;
      طالما (device_list)
        {
          /* Add the mount entry.  */
          بنية mount_اندراج *me = device_list->me;
          me->me_اگلا = mount_list;
          mount_list = me;
          بنية devlist *اگلا = device_list->اگلا;
          حرر (device_list);
          device_list = اگلا;
        }
      hash_free (devlist_جدول);
      devlist_جدول = عدم;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_خالص
ساكن بنية mount_اندراج ثابت *
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
             uintmax_t داخلہ_units, uintmax_t output_units)
{
  إذا (! معلوم_قدر (n) && !negative)
    أرجع "-";
  وإلا
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, داخلہ_units, output_units);
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
جوڑو_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
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
  size_t len = لڑی_لمبائی (s);
  أرجع (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساكن فراغ
get_میدان_اقدار (بنية میدان_اقدار_t *bv,
                  بنية میدان_اقدار_t *iv,
                  ثابت بنية fs_usage *fsu)
{
  /* Inode values.  */
  iv->داخلہ_units = iv->output_units = 1;
  iv->کل = fsu->fsu_files;
  iv->دستیاب = iv->دستیاب_منزل_جڑ = fsu->fsu_ffree;
  iv->negate_دستیاب = false;

  iv->ورتیا = UINTMAX_MAX;
  iv->negate_ورتیا = false;
  إذا (معلوم_قدر (iv->کل) && معلوم_قدر (iv->دستیاب_منزل_جڑ))
    {
      iv->ورتیا = iv->کل - iv->دستیاب_منزل_جڑ;
      iv->negate_ورتیا = (iv->کل < iv->دستیاب_منزل_جڑ);
    }

  /* Block values.  */
  bv->داخلہ_units = fsu->fsu_blocksize;
  bv->output_units = output_بلاک_ناپ;
  bv->کل = fsu->fsu_بلاکاں;
  bv->دستیاب = fsu->fsu_bavail;
  bv->دستیاب_منزل_جڑ = fsu->fsu_bfree;
  bv->negate_دستیاب = (fsu->fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ
                         && معلوم_قدر (fsu->fsu_bavail));

  bv->ورتیا = UINTMAX_MAX;
  bv->negate_ورتیا = false;
  إذا (معلوم_قدر (bv->کل) && معلوم_قدر (bv->دستیاب_منزل_جڑ))
    {
      bv->ورتیا = bv->کل - bv->دستیاب_منزل_جڑ;
      bv->negate_ورتیا = (bv->کل < bv->دستیاب_منزل_جڑ);
    }
}

/* Add block and inode values to grand total.  */
ساكن فراغ
جوڑو_منزل_grand_کل (بنية میدان_اقدار_t *bv, بنية میدان_اقدار_t *iv)
{
  إذا (معلوم_قدر (iv->کل))
    grand_fsu.fsu_files += iv->کل;
  إذا (معلوم_قدر (iv->دستیاب))
    grand_fsu.fsu_ffree += iv->دستیاب;

  إذا (معلوم_قدر (bv->کل))
    grand_fsu.fsu_بلاکاں += bv->داخلہ_units * bv->کل;
  إذا (معلوم_قدر (bv->دستیاب_منزل_جڑ))
    grand_fsu.fsu_bfree += bv->داخلہ_units * bv->دستیاب_منزل_جڑ;
  إذا (معلوم_قدر (bv->دستیاب))
    جوڑو_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ,
                            bv->داخلہ_units * bv->دستیاب,
                            bv->negate_دستیاب);
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
get_dev (حرف ثابت *device, حرف ثابت *mount_نقطہ, حرف ثابت *file,
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
  إذا (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_نقطہ))
    أرجع;

  إذا (!stat_file)
    stat_file = mount_نقطہ;

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
          fsu.fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ = false;
          fsu.fsu_blocksize = fsu.fsu_بلاکاں = fsu.fsu_bfree =
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
          بنية mount_اندراج ثابت * dev_me = me_for_dev (sb.st_dev);
          إذا (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ = false;
              fsu.fsu_blocksize = fsu.fsu_بلاکاں = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  إذا (fsu.fsu_بلاکاں == 0 && !show_all_fs && !show_listed_fs)
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
      && (resolved_dev = canonicalize_filename_منوال (dev_name, CAN_EXISTING)))
    {
      حرر (dev_name);
      dev_name = resolved_dev;
    }

  إذا (! fstype)
    fstype = "-";		/* unknown */

  بنية میدان_اقدار_t بلاک_اقدار;
  بنية میدان_اقدار_t inode_اقدار;
  get_میدان_اقدار (&بلاک_اقدار, &inode_اقدار, &fsu);

  /* Add to grand total unless processing grand total line.  */
  إذا (print_grand_کل && ! force_fsu)
    جوڑو_منزل_grand_کل (&بلاک_اقدار, &inode_اقدار);

  لكل (idx_t خانہ = 0; خانہ < جدول.ncolumns; خانہ++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      بنية میدان_اقدار_t *v;
      بدل (جدول.columns[خانہ]->میدان_type)
        {
        حالة بلاک_FLD:
          v = &بلاک_اقدار;
          اكسر;
        حالة INODE_FLD:
          v = &inode_اقدار;
          اكسر;
        حالة OTHER_FLD:
          v = عدم;
          اكسر;
        افتراضي:
          affirm (!"bad field_type");
        }

      بدل (جدول.columns[خانہ]->میدان)
        {
        حالة سروت_میدان:
          cell = xstrdup (dev_name);
          اكسر;

        حالة FSTYPE_میدان:
          cell = xstrdup (fstype);
          اكسر;

        حالة ناپ_میدان:
        حالة ITOTAL_میدان:
          cell = xstrdup (df_readable (false, v->کل, buf,
                                       v->داخلہ_units, v->output_units));
          اكسر;

        حالة ورتیا_میدان:
        حالة IUSED_میدان:
          cell = xstrdup (df_readable (v->negate_ورتیا, v->ورتیا, buf,
                                       v->داخلہ_units, v->output_units));
          اكسر;

        حالة AVAIL_میدان:
        حالة IAVAIL_میدان:
          cell = xstrdup (df_readable (v->negate_دستیاب, v->دستیاب, buf,
                                       v->داخلہ_units, v->output_units));
          اكسر;

        حالة PCENT_میدان:
        حالة IPCENT_میدان:
          {
            مزدوج pct = -1;
            إذا (! معلوم_قدر (v->ورتیا) || ! معلوم_قدر (v->دستیاب))
              ;
            وإلا إذا (!v->negate_ورتیا
                     && v->ورتیا <= UINTMAX_MAX / 100
                     && v->ورتیا + v->دستیاب != 0
                     && (v->ورتیا + v->دستیاب < v->ورتیا)
                     == v->negate_دستیاب)
              {
                uintmax_t u100 = v->ورتیا * 100;
                uintmax_t nonroot_کل = v->ورتیا + v->دستیاب;
                pct = u100 / nonroot_کل + (u100 % nonroot_کل != 0);
              }
            وإلا
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                مزدوج u = v->negate_ورتیا ? - (مزدوج) - v->ورتیا : v->ورتیا;
                مزدوج a = v->negate_دستیاب
                           ? - (مزدوج) - v->دستیاب : v->دستیاب;
                مزدوج nonroot_کل = u + a;
                إذا (nonroot_کل)
                  {
                    لما صحيح lipct = pct = u * 100 / nonroot_کل;
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

        حالة FILE_میدان:
          cell = xstrdup (file);
          اكسر;

        حالة نشانہ_میدان:
#جے_تعریف_شدہ HIDE_AUTOMOUNT_اگیتر
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          إذا (STRNCMP_LIT (mount_نقطہ, "/auto/") == 0)
            mount_نقطہ += 5;
          وإلا إذا (STRNCMP_LIT (mount_نقطہ, "/tmp_mnt/") == 0)
            mount_نقطہ += 8;
#جے_انت
          cell = xstrdup (mount_نقطہ);
          اكسر;

        حالة INVALID_میدان:
        افتراضي:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحيح cell_چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[خانہ]->چوڑائی = MAX (جدول.columns[خانہ]->چوڑائی, cell_چوڑائی);
      جدول.صفاں[جدول.nrows - 1][خانہ] = cell;
    }
  حرر (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساكن حرف *
آخری_device_for_mount (حرف ثابت *mount)
{
  بنية mount_اندراج ثابت *le = عدم;

  لكل (بنية mount_اندراج ثابت *me = mount_list; me; me = me->me_اگلا)
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
  بنية mount_اندراج ثابت *بہترین_میل = عدم;
  bool بہترین_میل_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  إذا (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t بہترین_میل_len = SIZE_MAX;
  لكل (بنية mount_اندراج ثابت *me = mount_list; me; me = me->me_اگلا)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      إذا (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      إذا (streq (device, devname))
        {
          حرف *آخری_device = آخری_device_for_mount (me->me_mountdir);
          eclipsed_device = آخری_device && ! streq (آخری_device, devname);
          size_t len = لڑی_لمبائی (me->me_mountdir);

          إذا (! eclipsed_device
              && (! بہترین_میل_accessible || len < بہترین_میل_len))
            {
              بنية stat device_stats;
              bool this_میل_accessible = false;

              إذا (stat (me->me_mountdir, &device_stats) == 0)
                بہترین_میل_accessible = this_میل_accessible = true;

              إذا (this_میل_accessible
                  || (! بہترین_میل_accessible && len < بہترین_میل_len))
                {
                  بہترین_میل = me;
                  إذا (len == 1) /* Traditional root.  */
                    {
                      حرر (آخری_device);
                      حرر (canon_dev);
                      اكسر;
                    }
                  وإلا
                    بہترین_میل_len = len;
                }
            }

          حرر (آخری_device);
        }

      حرر (canon_dev);
    }

  حرر (resolved);

  إذا (بہترین_میل)
    {
      get_dev (بہترین_میل->me_devname, بہترین_میل->me_mountdir, file, عدم,
               بہترین_میل->me_type, بہترین_میل->me_dummy,
               بہترین_میل->me_remote, عدم, false);
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
get_نقطہ (حرف ثابت *نقطہ, ثابت بنية stat *statp)
{
  بنية stat device_stats;
  بنية mount_اندراج ثابت *بہترین_میل = عدم;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (نقطہ);
  إذا (resolved && resolved[0] == '/')
    {
      size_t resolved_len = لڑی_لمبائی (resolved);
      size_t بہترین_میل_len = 0;

      لكل (بنية mount_اندراج *me = mount_list; me; me = me->me_اگلا)
        {
          إذا (!streq (me->me_type, "lofs")
              && (!بہترین_میل || بہترین_میل->me_dummy || !me->me_dummy))
            {
              size_t len = لڑی_لمبائی (me->me_mountdir);
              إذا (بہترین_میل_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  بہترین_میل = me;
                  بہترین_میل_len = len;
                }
            }
        }
    }
  حرر (resolved);
  إذا (بہترین_میل
      && (stat (بہترین_میل->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    بہترین_میل = عدم;

  إذا (! بہترین_میل)
    لكل (بنية mount_اندراج *me = mount_list; me; me = me->me_اگلا)
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
            && (!بہترین_میل || بہترین_میل->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            إذا (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            وإلا
              بہترین_میل = me;
          }
      }

  إذا (بہترین_میل)
    get_dev (بہترین_میل->me_devname, بہترین_میل->me_mountdir, نقطہ, نقطہ,
             بہترین_میل->me_type, بہترین_میل->me_dummy, بہترین_میل->me_remote,
             عدم, false);
  وإلا
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = لبھو_mount_نقطہ (نقطہ, statp);
      إذا (mp)
        {
          get_dev (عدم, mp, نقطہ, عدم, عدم,
                   false, false, عدم, false);
          حرر (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساكن فراغ
get_اندراج (حرف ثابت *name, بنية stat ثابت *statp)
{
  إذا ((S_ISBLK (statp->st_منوال) || S_ISCHR (statp->st_منوال))
      && get_device (name))
    أرجع;

  get_نقطہ (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساكن فراغ
get_all_entries (فراغ)
{
  filter_mount_list (show_all_fs);

  لكل (بنية mount_اندراج *me = mount_list; me; me = me->me_اگلا)
    get_dev (me->me_devname, me->me_mountdir, عدم, عدم, me->me_type,
             me->me_dummy, me->me_remote, عدم, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساكن فراغ
جوڑو_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_اگلا = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساكن فراغ
جوڑو_excluded_fs_type (حرف ثابت *fstype)
{
  بنية fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_اگلا = fs_exclude_list;
  fs_exclude_list = fsp;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
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
  -B, --بلاک-ناپ=ناپ_2\n\
         scale sizes by ناپ_2 before printing them; see ناپ_2 format ہیٹھاں;\n\
         E.g., '-BM' prints sizes وچ units دا 1,048,576 بائٹاں\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes وچ powers دا 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes وچ powers دا 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead دا بلاک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاک-ناپ=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         حد_بندی listing منزل local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         افعل not invoke sync before getting usage info (افتراضي)\n\
"));
      oputs (_("\
      --output[=میدان_LIST]\n\
         use the output format defined by میدان_LIST,\n\
         or print all fields إذا میدان_LIST is omitted\n\
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
      --کل\n\
         elide all entries insignificant منزل دستیاب space,\n\
         and produce a grand کل\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         حد_بندی listing منزل file systems دا type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         حد_بندی listing منزل file systems not دا type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_ناپ_note ();
      fputs (_("\n\
میدان_LIST is a comma-separated list دا columns منزل be included.  Valid\n\
میدان ناں are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info ورقہ).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  بنية stat *stats = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
                                                 &output_بلاک_ناپ);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;
        حالة 'i':
          إذا (سرخی_منوال == OUTPUT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          سرخی_منوال = INODES_منوال;
          اكسر;
        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_ادھار_1024;
          output_بلاک_ناپ = 1;
          اكسر;
        حالة 'H':
          human_output_opts = human_autoscale | human_SI;
          output_بلاک_ناپ = 1;
          اكسر;
        حالة 'k':
          human_output_opts = 0;
          output_بلاک_ناپ = 1024;
          اكسر;
        حالة 'l':
          show_local_fs = true;
          اكسر;
        حالة 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_بلاک_ناپ = 1024 * 1024;
          اكسر;
        حالة 'T':
          إذا (سرخی_منوال == OUTPUT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          اكسر;
        حالة 'P':
          إذا (سرخی_منوال == OUTPUT_منوال)
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
          جوڑو_fs_type (optarg);
          اكسر;

        حالة 'v':		/* For SysV compatibility.  */
          /* ignore */
          اكسر;
        حالة 'x':
          جوڑو_excluded_fs_type (optarg);
          اكسر;

        حالة OUTPUT_OPTION:
          إذا (سرخی_منوال == INODES_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (posix_format && سرخی_منوال == DEFAULT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          إذا (print_type)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          سرخی_منوال = OUTPUT_منوال;
          إذا (optarg)
            decode_output_arg (optarg);
          اكسر;

        حالة کل_OPTION:
          print_grand_کل = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (human_output_opts == -1)
    {
      إذا (posix_format)
        {
          human_output_opts = 0;
          output_بلاک_ناپ = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      وإلا
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_بلاک_ناپ);
    }

  إذا (سرخی_منوال == INODES_منوال || سرخی_منوال == OUTPUT_منوال)
    ;
  وإلا إذا (human_output_opts & human_autoscale)
    سرخی_منوال = HUMAN_منوال;
  وإلا إذا (posix_format)
    سرخی_منوال = POSIX_منوال;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool میل = false;
    لكل (بنية fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_اگلا)
      {
        لكل (بنية fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_اگلا)
          {
            إذا (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                میل = true;
                اكسر;
              }
          }
      }
    إذا (میل)
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
                            || میدان_انکڑے_2[FSTYPE_میدان].ورتیا
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

  get_میدان_list ();
  get_سرخی ();

  إذا (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      لكل (صحيح i = optind; i < argc; ++i)
        إذا (argv[i])
          get_اندراج (argv[i], &stats[i - optind]);
    }
  وإلا
    get_all_entries ();

  إذا (file_systems_processed)
    {
      إذا (print_grand_کل)
        get_dev ("total",
                 (میدان_انکڑے_2[سروت_میدان].ورتیا ? "-" : "total"),
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
