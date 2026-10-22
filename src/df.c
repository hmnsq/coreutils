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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>
#شامل <c-ctype.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "canonicalize.h"
#شامل "fsusage.h"
#شامل "human.h"
#شامل "mbswidth.h"
#شامل "mountlist.h"
#شامل "quote.h"
#شامل "find-mount-point.h"
#شامل "hash.h"
#شامل "xstrtol-error.h"
#شامل "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "df"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

ساخت devlist
{
  dev_t dev_عدد;
  ساخت mount_اندراج *me;
  ساخت devlist *اگلا;
  ساخت devlist *اگلا_same_dev;
  ساخت devlist *نظر_آیا_آخری; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
ساکن Hash_جدول *devlist_جدول;

/* If true, show even file systems with zero size or
   uninteresting types.  */
ساکن bool show_all_fs;

/* If true, show only local file systems.  */
ساکن bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
ساکن bool show_listed_fs;

/* Human-readable options for output.  */
ساکن صحیح human_output_opts = -1;

/* The units to use when printing sizes.  */
ساکن uintmax_t output_بلاک_سائز;

/* True if a file system has been processed for output.  */
ساکن bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
ساکن bool require_sync;

/* Desired exit status.  */
ساکن صحیح exit_حالت;

/* A file system type to display.  */

ساخت fs_type_list
{
  حرف *fs_name;
  ساخت fs_type_list *fs_اگلا;
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

ساکن ساخت fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

ساکن ساخت fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
ساکن ساخت mount_اندراج *mount_list;

/* If true, print file system type as well.  */
ساکن bool print_type;

/* If true, print a grand total at the end.  */
ساکن bool print_grand_کل;

/* Grand total data.  */
ساکن ساخت fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
ساکن شمار
{
  DEFAULT_منوال,
  INODES_منوال,
  HUMAN_منوال,
  POSIX_منوال,
  OUTPUT_منوال
} سرخی_منوال = DEFAULT_منوال;

/* Displayable fields.  */
قسمتعریف شمار
{
  ماخذ_میدان, /* file system */
  FSTYPE_میدان, /* FS type */
  سائز_میدان,   /* FS size */
  مستعمل_میدان,   /* FS size used  */
  AVAIL_میدان,  /* FS size available */
  PCENT_میدان,  /* percent used */
  ITOTAL_میدان, /* inode total */
  IUSED_میدان,  /* inodes used */
  IAVAIL_میدان, /* inodes available */
  IPCENT_میدان, /* inodes used in percent */
  ہدف_میدان, /* mount point */
  FILE_میدان,   /* specified file name */
  INVALID_میدان /* validation marker */
} display_میدان_t;

/* Flag if a field contains a block, an inode or another value.  */
قسمتعریف شمار
{
  بلاک_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} میدان_type_t;

/* Attributes of a display field.  */
ساخت میدان_ڈیٹا_t
{
  display_میدان_t میدان;
  حرف ثابت *arg;
  میدان_type_t میدان_type;
  حرف ثابت *caption;/* NULL means use default header of this field.  */
  صحیح چوڑائی;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_دایاں;   /* Whether to right-align columns, not left-align.  */
  bool مستعمل;
};

/* Header strings, minimum width and alignment for the above fields.  */
ساکن ساخت میدان_ڈیٹا_t میدان_ڈیٹا_2[] = {
  [ماخذ_میدان] = { ماخذ_میدان,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_میدان] = { FSTYPE_میدان,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [سائز_میدان] = { سائز_میدان,
    "size",   بلاک_FLD, N_("blocks"),      5, true, false },

  [مستعمل_میدان] = { مستعمل_میدان,
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

  [ہدف_میدان] = { ہدف_میدان,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_میدان] = { FILE_میدان,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

ساکن حرف ثابت *all_دلائل_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

ساکن ساخت
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  حرف ***صفیں COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  ساخت میدان_ڈیٹا_t **columns COUNTED_BY (ncolumns);
} جدول;

/* Field values.  */
ساخت میدان_اقدار_t
{
  uintmax_t داخلہ_units;
  uintmax_t output_units;
  uintmax_t کل;
  uintmax_t دستیاب;
  bool negate_دستیاب;
  uintmax_t دستیاب_مقصد_جڑ;
  uintmax_t مستعمل;
  bool negate_مستعمل;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  کل_OPTION,
  OUTPUT_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"block-size", required_دلیل, تہی, 'B'},
  {"inodes", no_دلیل, تہی, 'i'},
  {"human-readable", no_دلیل, تہی, 'h'},
  {"si", no_دلیل, تہی, 'H'},
  {"local", no_دلیل, تہی, 'l'},
  {"output", optional_دلیل, تہی, OUTPUT_OPTION},
  {"portability", no_دلیل, تہی, 'P'},
  {"print-type", no_دلیل, تہی, 'T'},
  {"sync", no_دلیل, تہی, SYNC_OPTION},
  {"no-sync", no_دلیل, تہی, NO_SYNC_OPTION},
  {"total", no_دلیل, تہی, کل_OPTION},
  {"type", required_دلیل, تہی, 't'},
  {"exclude-type", required_دلیل, تہی, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

ساکن صحیح
automount_stat_err (حرف ثابت *file, ساخت stat *st)
{
  صحیح fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  اگر (fd < 0)
    {
      اگر (errno == ENOENT || errno == ENOTDIR)
        واپس errno;
      واپس stat (file, st) == 0 ? 0 : errno;
    }
  ورنہ
    {
      صحیح err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      واپس err;
    }
}

شمار { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

ساکن خالی
replace_control_chars (حرف *cell)
{
  حرف *p = cell;
  جب (*p)
    {
      اگر (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

ساکن خالی
replace_invalid_chars (حرف *cell)
{
  حرف *srcend = cell + لڑی_لمبائی (cell);
  حرف *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  برائے (حرف *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      اگر (ok)
        ok = !c32iscntrl (wc);
      ورنہ
        n = 1;

      اگر (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      ورنہ
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

ساکن خالی
replace_problematic_chars (حرف *cell)
{
  ساکن صحیح tty_برآمد = -1;
  اگر (tty_برآمد < 0)
    tty_برآمد = isatty (STDOUT_FILENO);

  (tty_برآمد ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

ساکن خالی
alloc_جدول_صف (خالی)
{
  اگر (جدول.nrows == جدول.nrows_alloc)
    جدول.صفیں = xpalloc (جدول.صفیں, &جدول.nrows_alloc, 1, -1,
                          حجم *جدول.صفیں);
  idx_t ثابت صف_idx = جدول.nrows++;
  جدول.صفیں[صف_idx] = xinmalloc (جدول.ncolumns, حجم *جدول.صفیں[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

ساکن خالی
print_جدول (خالی)
{
  برائے (idx_t صف = 0; صف < جدول.nrows; صف++)
    {
      برائے (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
        {
          حرف *cell = جدول.صفیں[صف][ستون];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          اگر (ستون != 0)
            حرف_لکھو (' ');

          صحیح چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
          صحیح fill = چوڑائی < 0 ? 0 : جدول.columns[ستون]->چوڑائی - چوڑائی;
          اگر (جدول.columns[ستون]->align_دایاں)
            برائے (; 0 < fill; fill--)
              حرف_لکھو (' ');
          fputs (cell, stdout);
          اگر (ستون + 1 < جدول.ncolumns)
            برائے (; 0 < fill; fill--)
              حرف_لکھو (' ');
        }
      حرف_لکھو ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

ساکن خالی
alloc_میدان (صحیح f, حرف ثابت *c)
{
  اگر (جدول.ncolumns == جدول.ncolumns_alloc)
    جدول.columns = xpalloc (جدول.columns, &جدول.ncolumns_alloc, 1, -1,
                             حجم *جدول.columns);
  idx_t ثابت عمود_idx = جدول.ncolumns++;
  جدول.columns[عمود_idx] = &میدان_ڈیٹا_2[f];
  اگر (c != تہی)
    میدان_ڈیٹا_2[f].caption = c;

  affirm (!میدان_ڈیٹا_2[f].مستعمل);

  /* Mark field as used.  */
  میدان_ڈیٹا_2[f].مستعمل = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
ساکن خالی
decode_output_arg (حرف ثابت *arg)
{
  حرف *arg_writable = xstrdup (arg);
  حرف *s = arg_writable;
  کرو
    {
      /* find next comma */
      حرف *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      اگر (comma)
        *comma++ = 0;

      /* process S.  */
      display_میدان_t میدان = INVALID_میدان;
      برائے (idx_t i = 0; i < countof (میدان_ڈیٹا_2); i++)
        {
          اگر (streq (میدان_ڈیٹا_2[i].arg, s))
            {
              میدان = i;
              توڑو;
            }
        }
      اگر (میدان == INVALID_میدان)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      اگر (میدان_ڈیٹا_2[میدان].مستعمل)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (میدان_ڈیٹا_2[میدان].arg));
          usage (EXIT_FAILURE);
        }

      انتخاب (میدان)
        {
        حالت ماخذ_میدان:
        حالت FSTYPE_میدان:
        حالت مستعمل_میدان:
        حالت PCENT_میدان:
        حالت ITOTAL_میدان:
        حالت IUSED_میدان:
        حالت IAVAIL_میدان:
        حالت IPCENT_میدان:
        حالت ہدف_میدان:
        حالت FILE_میدان:
          alloc_میدان (میدان, تہی);
          توڑو;

        حالت سائز_میدان:
          alloc_میدان (میدان, N_("Size"));
          توڑو;

        حالت AVAIL_میدان:
          alloc_میدان (میدان, N_("Avail"));
          توڑو;

        حالت INVALID_میدان:
        طےشدہ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  جب (s);

  آزاد (arg_writable);
}

/* Get the appropriate columns for the mode.  */
ساکن خالی
get_میدان_list (خالی)
{
  انتخاب (سرخی_منوال)
    {
    حالت DEFAULT_منوال:
      alloc_میدان (ماخذ_میدان, تہی);
      اگر (print_type)
        alloc_میدان (FSTYPE_میدان, تہی);
      alloc_میدان (سائز_میدان,   تہی);
      alloc_میدان (مستعمل_میدان,   تہی);
      alloc_میدان (AVAIL_میدان,  تہی);
      alloc_میدان (PCENT_میدان,  تہی);
      alloc_میدان (ہدف_میدان, تہی);
      توڑو;

    حالت HUMAN_منوال:
      alloc_میدان (ماخذ_میدان, تہی);
      اگر (print_type)
        alloc_میدان (FSTYPE_میدان, تہی);

      alloc_میدان (سائز_میدان,   N_("Size"));
      alloc_میدان (مستعمل_میدان,   تہی);
      alloc_میدان (AVAIL_میدان,  N_("Avail"));
      alloc_میدان (PCENT_میدان,  تہی);
      alloc_میدان (ہدف_میدان, تہی);
      توڑو;

    حالت INODES_منوال:
      alloc_میدان (ماخذ_میدان, تہی);
      اگر (print_type)
        alloc_میدان (FSTYPE_میدان, تہی);
      alloc_میدان (ITOTAL_میدان,  تہی);
      alloc_میدان (IUSED_میدان,   تہی);
      alloc_میدان (IAVAIL_میدان,  تہی);
      alloc_میدان (IPCENT_میدان,  تہی);
      alloc_میدان (ہدف_میدان,  تہی);
      توڑو;

    حالت POSIX_منوال:
      alloc_میدان (ماخذ_میدان, تہی);
      اگر (print_type)
        alloc_میدان (FSTYPE_میدان, تہی);
      alloc_میدان (سائز_میدان,   تہی);
      alloc_میدان (مستعمل_میدان,   تہی);
      alloc_میدان (AVAIL_میدان,  تہی);
      alloc_میدان (PCENT_میدان,  N_("Capacity"));
      alloc_میدان (ہدف_میدان, تہی);
      توڑو;

    حالت OUTPUT_منوال:
      اگر (!جدول.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_دلائل_string);
        }
      توڑو;

    طےشدہ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

ساکن خالی
get_سرخی (خالی)
{
  alloc_جدول_صف ();

  برائے (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف *cell;
      حرف ثابت *سرخی = _(جدول.columns[ستون]->caption);

      اگر (جدول.columns[ستون]->میدان == سائز_میدان
          && (سرخی_منوال == DEFAULT_منوال
              || (سرخی_منوال == OUTPUT_منوال
                  && !(human_output_opts & human_autoscale))))
        {
          حرف buf[LONGEST_HUMAN_READABLE + 1];

          صحیح opts = (human_suppress_نقطہ_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_گروہ_digits | human_بنیاد_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_بلاک_سائز;
          uintmax_t q1024 = output_بلاک_سائز;
          bool divisible_by_1000;
          bool divisible_by_1024;

          کرو
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          جب (divisible_by_1000 & divisible_by_1024);

          اگر (divisible_by_1000 < divisible_by_1024)
            opts |= human_بنیاد_1024;
          اگر (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_بنیاد_1024;
          اگر (! (opts & human_بنیاد_1024))
            opts |= human_B;

          حرف *عدد = human_readable (output_بلاک_سائز, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          سرخی = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), عدد, سرخی);
        }
      ورنہ اگر (سرخی_منوال == POSIX_منوال
               && جدول.columns[ستون]->میدان == سائز_میدان)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_بلاک_سائز, سرخی);
        }
      ورنہ
        cell = xstrdup (سرخی);

      replace_problematic_chars (cell);

      جدول.صفیں[جدول.nrows - 1][ستون] = cell;

      صحیح cell_چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->چوڑائی = MAX (جدول.columns[ستون]->چوڑائی, cell_چوڑائی);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_خالص
ساکن bool
selected_fstype (حرف ثابت *fstype)
{
  اگر (fs_select_list == تہی || fstype == تہی)
    واپس true;
  برائے (ثابت ساخت fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_اگلا)
    اگر (streq (fstype, fsp->fs_name))
      واپس true;
  واپس false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_خالص
ساکن bool
excluded_fstype (حرف ثابت *fstype)
{
  اگر (fs_exclude_list == تہی || fstype == تہی)
    واپس false;
  برائے (ثابت ساخت fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_اگلا)
    اگر (streq (fstype, fsp->fs_name))
      واپس true;
  واپس false;
}

ساکن size_t
devlist_hash (خالی ثابت *x, size_t جدول_سائز)
{
  ساخت devlist ثابت *p = x;
  واپس (uintmax_t) {p->dev_عدد} % جدول_سائز;
}

ساکن bool
devlist_compare (خالی ثابت *x, خالی ثابت *y)
{
  ساخت devlist ثابت *a = x;
  ساخت devlist ثابت *b = y;
  واپس a->dev_عدد == b->dev_عدد;
}

ساکن ساخت devlist *
devlist_for_dev (dev_t dev)
{
  اگر (devlist_جدول == تہی)
    واپس تہی;
  ساخت devlist dev_اندراج;
  dev_اندراج.dev_عدد = dev;

  ساخت devlist *ملا = hash_lookup (devlist_جدول, &dev_اندراج);
  اگر (ملا == تہی)
    واپس تہی;

  /* Return the last devlist entry we have seen with this dev_num */
  واپس ملا->نظر_آیا_آخری;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

ساکن خالی
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  ساخت devlist *device_list = تہی;
  صحیح mount_list_سائز = 0;

  برائے (ساخت mount_اندراج *me = mount_list; me; me = me->me_اگلا)
    mount_list_سائز++;

  devlist_جدول = hash_initialize (mount_list_سائز, تہی,
                                   devlist_hash, devlist_compare, تہی);
  اگر (devlist_جدول == تہی)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  برائے (ساخت mount_اندراج *me = mount_list; me;)
    {
      ساخت stat buf;
      ساخت mount_اندراج *discard_me = تہی;
      ساخت devlist *آخری_نظر_آیا_dev = تہی, *نظر_آیا_dev = تہی;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      اگر ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      ورنہ
        {
          /* If we've already seen this device...  */
          آخری_نظر_آیا_dev = نظر_آیا_dev = devlist_for_dev (buf.st_dev);

          برائے (; نظر_آیا_dev && ! discard_me; نظر_آیا_dev = نظر_آیا_dev->اگلا_same_dev)
            {
              bool ہدف_nearer_جڑ = لڑی_لمبائی (نظر_آیا_dev->me->me_mountdir)
                                        > لڑی_لمبائی (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool ماخذ_نیچے_جڑ = نظر_آیا_dev->me->me_mntroot != تہی
                                       && me->me_mntroot != تہی
                                       && (لڑی_لمبائی (نظر_آیا_dev->me->me_mntroot)
                                           < لڑی_لمبائی (me->me_mntroot));
              اگر (! print_grand_کل
                  && me->me_remote && نظر_آیا_dev->me->me_remote
                  && ! streq (نظر_آیا_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              ورنہ اگر ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (نظر_آیا_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (ہدف_nearer_جڑ && ! ماخذ_نیچے_جڑ)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (نظر_آیا_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     نظر_آیا_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = نظر_آیا_dev->me;
                  نظر_آیا_dev->me = me;
                }
              ورنہ
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      اگر (discard_me)
        {
          me = me->me_اگلا;
          اگر (! devices_only)
            free_mount_اندراج (discard_me);
        }
      ورنہ
        {
          /* Add the device number to the device_table.  */
          ساخت devlist *devlist = xmalloc (حجم *devlist);
          devlist->me = me;
          devlist->dev_عدد = buf.st_dev;
          devlist->اگلا_same_dev = آخری_نظر_آیا_dev;
          devlist->اگلا = device_list;
          device_list = devlist;

          ساخت devlist *hash_اندراج = hash_داخل_کرو (devlist_جدول, devlist);
          اگر (hash_اندراج == تہی)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_اندراج->نظر_آیا_آخری = devlist;

          me = me->me_اگلا;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  اگر (! devices_only)
    {
      mount_list = تہی;
      جب (device_list)
        {
          /* Add the mount entry.  */
          ساخت mount_اندراج *me = device_list->me;
          me->me_اگلا = mount_list;
          mount_list = me;
          ساخت devlist *اگلا = device_list->اگلا;
          آزاد (device_list);
          device_list = اگلا;
        }
      hash_free (devlist_جدول);
      devlist_جدول = تہی;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_خالص
ساکن ساخت mount_اندراج ثابت *
me_for_dev (dev_t dev)
{
  ساخت devlist *dl = devlist_for_dev (dev);
  اگر (dl)
        واپس dl->me;

  واپس تہی;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
ساکن bool
معلوم_قدر (uintmax_t n)
{
  واپس n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

ساکن حرف ثابت *
df_readable (bool negative, uintmax_t n, حرف *buf,
             uintmax_t داخلہ_units, uintmax_t output_units)
{
  اگر (! معلوم_قدر (n) && !negative)
    واپس "-";
  ورنہ
    {
      حرف *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, داخلہ_units, output_units);
      اگر (negative)
        *--p = '-';
      واپس p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
ساکن خالی
جوڑو_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  اگر (*dest_neg == src_neg)
    {
      *dest += src;
      واپس;
    }

  اگر (*dest_neg)
    *dest = -*dest;

  اگر (src_neg)
    src = -src;

  اگر (src < *dest)
    *dest -= src;
  ورنہ
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  اگر (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_خالص
ساکن bool
has_uuid_suffix (حرف ثابت *s)
{
  size_t len = لڑی_لمبائی (s);
  واپس (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
ساکن خالی
get_میدان_اقدار (ساخت میدان_اقدار_t *bv,
                  ساخت میدان_اقدار_t *iv,
                  ثابت ساخت fs_usage *fsu)
{
  /* Inode values.  */
  iv->داخلہ_units = iv->output_units = 1;
  iv->کل = fsu->fsu_files;
  iv->دستیاب = iv->دستیاب_مقصد_جڑ = fsu->fsu_ffree;
  iv->negate_دستیاب = false;

  iv->مستعمل = UINTMAX_MAX;
  iv->negate_مستعمل = false;
  اگر (معلوم_قدر (iv->کل) && معلوم_قدر (iv->دستیاب_مقصد_جڑ))
    {
      iv->مستعمل = iv->کل - iv->دستیاب_مقصد_جڑ;
      iv->negate_مستعمل = (iv->کل < iv->دستیاب_مقصد_جڑ);
    }

  /* Block values.  */
  bv->داخلہ_units = fsu->fsu_blocksize;
  bv->output_units = output_بلاک_سائز;
  bv->کل = fsu->fsu_بلاکیں;
  bv->دستیاب = fsu->fsu_bavail;
  bv->دستیاب_مقصد_جڑ = fsu->fsu_bfree;
  bv->negate_دستیاب = (fsu->fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ
                         && معلوم_قدر (fsu->fsu_bavail));

  bv->مستعمل = UINTMAX_MAX;
  bv->negate_مستعمل = false;
  اگر (معلوم_قدر (bv->کل) && معلوم_قدر (bv->دستیاب_مقصد_جڑ))
    {
      bv->مستعمل = bv->کل - bv->دستیاب_مقصد_جڑ;
      bv->negate_مستعمل = (bv->کل < bv->دستیاب_مقصد_جڑ);
    }
}

/* Add block and inode values to grand total.  */
ساکن خالی
جوڑو_مقصد_grand_کل (ساخت میدان_اقدار_t *bv, ساخت میدان_اقدار_t *iv)
{
  اگر (معلوم_قدر (iv->کل))
    grand_fsu.fsu_files += iv->کل;
  اگر (معلوم_قدر (iv->دستیاب))
    grand_fsu.fsu_ffree += iv->دستیاب;

  اگر (معلوم_قدر (bv->کل))
    grand_fsu.fsu_بلاکیں += bv->داخلہ_units * bv->کل;
  اگر (معلوم_قدر (bv->دستیاب_مقصد_جڑ))
    grand_fsu.fsu_bfree += bv->داخلہ_units * bv->دستیاب_مقصد_جڑ;
  اگر (معلوم_قدر (bv->دستیاب))
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

ساکن خالی
get_dev (حرف ثابت *device, حرف ثابت *mount_نقطہ, حرف ثابت *file,
         حرف ثابت *stat_file, حرف ثابت *fstype,
         bool me_dummy, bool me_remote,
         ثابت ساخت fs_usage *force_fsu,
         bool process_all)
{
  اگر (me_remote && show_local_fs)
    واپس;

  اگر (me_dummy && !show_all_fs && !show_listed_fs)
    واپس;

  اگر (!selected_fstype (fstype) || excluded_fstype (fstype))
    واپس;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  اگر (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_نقطہ))
    واپس;

  اگر (!stat_file)
    stat_file = mount_نقطہ;

  ساخت fs_usage fsu;
  اگر (force_fsu)
    fsu = *force_fsu;
  ورنہ اگر (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      اگر (process_all && (errno == EACCES || errno == ENOENT))
        {
          اگر (! show_all_fs)
            واپس;

          fstype = "-";
          fsu.fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ = false;
          fsu.fsu_blocksize = fsu.fsu_بلاکیں = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      ورنہ
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_حالت = EXIT_FAILURE;
          واپس;
        }
    }
  ورنہ اگر (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      ساخت stat sb;
      اگر (stat (stat_file, &sb) == 0)
        {
          ساخت mount_اندراج ثابت * dev_me = me_for_dev (sb.st_dev);
          اگر (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_چوٹی_ثنائی_ہندسہ_سیٹ = false;
              fsu.fsu_blocksize = fsu.fsu_بلاکیں = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  اگر (fsu.fsu_بلاکیں == 0 && !show_all_fs && !show_listed_fs)
    واپس;

  اگر (! force_fsu)
    file_systems_processed = true;

  alloc_جدول_صف ();

  اگر (! device)
    device = "-";		/* unknown */

  اگر (! file)
    file = "-";			/* unspecified */

  حرف *dev_name = xstrdup (device);
  حرف *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  اگر (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_منوال (dev_name, CAN_EXISTING)))
    {
      آزاد (dev_name);
      dev_name = resolved_dev;
    }

  اگر (! fstype)
    fstype = "-";		/* unknown */

  ساخت میدان_اقدار_t بلاک_اقدار;
  ساخت میدان_اقدار_t inode_اقدار;
  get_میدان_اقدار (&بلاک_اقدار, &inode_اقدار, &fsu);

  /* Add to grand total unless processing grand total line.  */
  اگر (print_grand_کل && ! force_fsu)
    جوڑو_مقصد_grand_کل (&بلاک_اقدار, &inode_اقدار);

  برائے (idx_t ستون = 0; ستون < جدول.ncolumns; ستون++)
    {
      حرف buf[LONGEST_HUMAN_READABLE + 2];
      حرف *cell;

      ساخت میدان_اقدار_t *v;
      انتخاب (جدول.columns[ستون]->میدان_type)
        {
        حالت بلاک_FLD:
          v = &بلاک_اقدار;
          توڑو;
        حالت INODE_FLD:
          v = &inode_اقدار;
          توڑو;
        حالت OTHER_FLD:
          v = تہی;
          توڑو;
        طےشدہ:
          affirm (!"bad field_type");
        }

      انتخاب (جدول.columns[ستون]->میدان)
        {
        حالت ماخذ_میدان:
          cell = xstrdup (dev_name);
          توڑو;

        حالت FSTYPE_میدان:
          cell = xstrdup (fstype);
          توڑو;

        حالت سائز_میدان:
        حالت ITOTAL_میدان:
          cell = xstrdup (df_readable (false, v->کل, buf,
                                       v->داخلہ_units, v->output_units));
          توڑو;

        حالت مستعمل_میدان:
        حالت IUSED_میدان:
          cell = xstrdup (df_readable (v->negate_مستعمل, v->مستعمل, buf,
                                       v->داخلہ_units, v->output_units));
          توڑو;

        حالت AVAIL_میدان:
        حالت IAVAIL_میدان:
          cell = xstrdup (df_readable (v->negate_دستیاب, v->دستیاب, buf,
                                       v->داخلہ_units, v->output_units));
          توڑو;

        حالت PCENT_میدان:
        حالت IPCENT_میدان:
          {
            دوہرا pct = -1;
            اگر (! معلوم_قدر (v->مستعمل) || ! معلوم_قدر (v->دستیاب))
              ;
            ورنہ اگر (!v->negate_مستعمل
                     && v->مستعمل <= UINTMAX_MAX / 100
                     && v->مستعمل + v->دستیاب != 0
                     && (v->مستعمل + v->دستیاب < v->مستعمل)
                     == v->negate_دستیاب)
              {
                uintmax_t u100 = v->مستعمل * 100;
                uintmax_t nonroot_کل = v->مستعمل + v->دستیاب;
                pct = u100 / nonroot_کل + (u100 % nonroot_کل != 0);
              }
            ورنہ
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                دوہرا u = v->negate_مستعمل ? - (دوہرا) - v->مستعمل : v->مستعمل;
                دوہرا a = v->negate_دستیاب
                           ? - (دوہرا) - v->دستیاب : v->دستیاب;
                دوہرا nonroot_کل = u + a;
                اگر (nonroot_کل)
                  {
                    طویل صحیح lipct = pct = u * 100 / nonroot_کل;
                    دوہرا ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    اگر (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            توڑو;
          }

        حالت FILE_میدان:
          cell = xstrdup (file);
          توڑو;

        حالت ہدف_میدان:
#اگر_تعریف_شدہ HIDE_AUTOMOUNT_سابقہ
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          اگر (STRNCMP_LIT (mount_نقطہ, "/auto/") == 0)
            mount_نقطہ += 5;
          ورنہ اگر (STRNCMP_LIT (mount_نقطہ, "/tmp_mnt/") == 0)
            mount_نقطہ += 8;
#اگر_ختم
          cell = xstrdup (mount_نقطہ);
          توڑو;

        حالت INVALID_میدان:
        طےشدہ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      صحیح cell_چوڑائی = mbswidth (cell, MBSWIDTH_FLAGS);
      جدول.columns[ستون]->چوڑائی = MAX (جدول.columns[ستون]->چوڑائی, cell_چوڑائی);
      جدول.صفیں[جدول.nrows - 1][ستون] = cell;
    }
  آزاد (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
ساکن حرف *
آخری_device_for_mount (حرف ثابت *mount)
{
  ساخت mount_اندراج ثابت *le = تہی;

  برائے (ساخت mount_اندراج ثابت *me = mount_list; me; me = me->me_اگلا)
    {
      اگر (streq (me->me_mountdir, mount))
        le = me;
    }

  اگر (le)
    {
      حرف *devname = le->me_devname;
      حرف *canon_dev = canonicalize_file_name (devname);
      اگر (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        واپس canon_dev;
      آزاد (canon_dev);
      واپس xstrdup (le->me_devname);
    }
  ورنہ
    واپس تہی;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
ساکن bool
get_device (حرف ثابت *device)
{
  ساخت mount_اندراج ثابت *بہترین_مطابقت = تہی;
  bool بہترین_مطابقت_accessible = false;
  bool eclipsed_device = false;
  حرف ثابت *file = device;

  حرف *resolved = canonicalize_file_name (device);
  اگر (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t بہترین_مطابقت_len = SIZE_MAX;
  برائے (ساخت mount_اندراج ثابت *me = mount_list; me; me = me->me_اگلا)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      حرف *devname = me->me_devname;
      حرف *canon_dev = canonicalize_file_name (me->me_devname);
      اگر (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      اگر (streq (device, devname))
        {
          حرف *آخری_device = آخری_device_for_mount (me->me_mountdir);
          eclipsed_device = آخری_device && ! streq (آخری_device, devname);
          size_t len = لڑی_لمبائی (me->me_mountdir);

          اگر (! eclipsed_device
              && (! بہترین_مطابقت_accessible || len < بہترین_مطابقت_len))
            {
              ساخت stat device_stats;
              bool this_مطابقت_accessible = false;

              اگر (stat (me->me_mountdir, &device_stats) == 0)
                بہترین_مطابقت_accessible = this_مطابقت_accessible = true;

              اگر (this_مطابقت_accessible
                  || (! بہترین_مطابقت_accessible && len < بہترین_مطابقت_len))
                {
                  بہترین_مطابقت = me;
                  اگر (len == 1) /* Traditional root.  */
                    {
                      آزاد (آخری_device);
                      آزاد (canon_dev);
                      توڑو;
                    }
                  ورنہ
                    بہترین_مطابقت_len = len;
                }
            }

          آزاد (آخری_device);
        }

      آزاد (canon_dev);
    }

  آزاد (resolved);

  اگر (بہترین_مطابقت)
    {
      get_dev (بہترین_مطابقت->me_devname, بہترین_مطابقت->me_mountdir, file, تہی,
               بہترین_مطابقت->me_type, بہترین_مطابقت->me_dummy,
               بہترین_مطابقت->me_remote, تہی, false);
      واپس true;
    }
  ورنہ اگر (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_حالت = EXIT_FAILURE;
      واپس true;
    }

  واپس false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
ساکن خالی
get_نقطہ (حرف ثابت *نقطہ, ثابت ساخت stat *statp)
{
  ساخت stat device_stats;
  ساخت mount_اندراج ثابت *بہترین_مطابقت = تہی;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  حرف *resolved = canonicalize_file_name (نقطہ);
  اگر (resolved && resolved[0] == '/')
    {
      size_t resolved_len = لڑی_لمبائی (resolved);
      size_t بہترین_مطابقت_len = 0;

      برائے (ساخت mount_اندراج *me = mount_list; me; me = me->me_اگلا)
        {
          اگر (!streq (me->me_type, "lofs")
              && (!بہترین_مطابقت || بہترین_مطابقت->me_dummy || !me->me_dummy))
            {
              size_t len = لڑی_لمبائی (me->me_mountdir);
              اگر (بہترین_مطابقت_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  بہترین_مطابقت = me;
                  بہترین_مطابقت_len = len;
                }
            }
        }
    }
  آزاد (resolved);
  اگر (بہترین_مطابقت
      && (stat (بہترین_مطابقت->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    بہترین_مطابقت = تہی;

  اگر (! بہترین_مطابقت)
    برائے (ساخت mount_اندراج *me = mount_list; me; me = me->me_اگلا)
      {
        اگر (me->me_dev == (dev_t) -1)
          {
            اگر (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            ورنہ
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                اگر (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_حالت = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        اگر (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!بہترین_مطابقت || بہترین_مطابقت->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            اگر (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            ورنہ
              بہترین_مطابقت = me;
          }
      }

  اگر (بہترین_مطابقت)
    get_dev (بہترین_مطابقت->me_devname, بہترین_مطابقت->me_mountdir, نقطہ, نقطہ,
             بہترین_مطابقت->me_type, بہترین_مطابقت->me_dummy, بہترین_مطابقت->me_remote,
             تہی, false);
  ورنہ
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      حرف *mp = ڈھونڈو_mount_نقطہ (نقطہ, statp);
      اگر (mp)
        {
          get_dev (تہی, mp, نقطہ, تہی, تہی,
                   false, false, تہی, false);
          آزاد (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

ساکن خالی
get_اندراج (حرف ثابت *name, ساخت stat ثابت *statp)
{
  اگر ((S_ISBLK (statp->st_منوال) || S_ISCHR (statp->st_منوال))
      && get_device (name))
    واپس;

  get_نقطہ (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

ساکن خالی
get_all_entries (خالی)
{
  filter_mount_list (show_all_fs);

  برائے (ساخت mount_اندراج *me = mount_list; me; me = me->me_اگلا)
    get_dev (me->me_devname, me->me_mountdir, تہی, تہی, me->me_type,
             me->me_dummy, me->me_remote, تہی, true);
}

/* Add FSTYPE to the list of file system types to display.  */

ساکن خالی
جوڑو_fs_type (حرف ثابت *fstype)
{
  ساخت fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_اگلا = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

ساکن خالی
جوڑو_excluded_fs_type (حرف ثابت *fstype)
{
  ساخت fs_type_list *fsp;

  fsp = xmalloc (حجم *fsp);
  fsp->fs_name = (حرف *) fstype;
  fsp->fs_اگلا = fs_exclude_list;
  fs_exclude_list = fsp;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]...\n"), پروگرام_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by طےشدہ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --بلاک-سائز=سائز_2\n\
         scale sizes by سائز_2 before printing them; see سائز_2 format نیچے;\n\
         E.g., '-BM' prints sizes اندر units کا 1,048,576 بائٹیں\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes اندر powers کا 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes اندر powers کا 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead کا بلاک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاک-سائز=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         حد_بندی listing مقصد local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         کرو not invoke sync before getting usage info (طےشدہ)\n\
"));
      oputs (_("\
      --output[=میدان_LIST]\n\
         use the output format defined by میدان_LIST,\n\
         or print all fields اگر میدان_LIST is omitted\n\
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
         elide all entries insignificant مقصد دستیاب space,\n\
         and produce a grand کل\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         حد_بندی listing مقصد file systems کا type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         حد_بندی listing مقصد file systems not کا type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_سائز_note ();
      fputs (_("\n\
میدان_LIST is a comma-separated list کا columns مقصد be included.  Valid\n\
میدان نام are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info صفحہ).\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  ساخت stat *stats = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  حرف ثابت *پیغام_mut_excl = _("options %s and %s are mutually exclusive");

  جب (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'a':
          show_all_fs = true;
          توڑو;
        حالت 'B':
          {
            شمار strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاک_سائز);
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          توڑو;
        حالت 'i':
          اگر (سرخی_منوال == OUTPUT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          سرخی_منوال = INODES_منوال;
          توڑو;
        حالت 'h':
          human_output_opts = human_autoscale | human_SI | human_بنیاد_1024;
          output_بلاک_سائز = 1;
          توڑو;
        حالت 'H':
          human_output_opts = human_autoscale | human_SI;
          output_بلاک_سائز = 1;
          توڑو;
        حالت 'k':
          human_output_opts = 0;
          output_بلاک_سائز = 1024;
          توڑو;
        حالت 'l':
          show_local_fs = true;
          توڑو;
        حالت 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_بلاک_سائز = 1024 * 1024;
          توڑو;
        حالت 'T':
          اگر (سرخی_منوال == OUTPUT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          توڑو;
        حالت 'P':
          اگر (سرخی_منوال == OUTPUT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          توڑو;
        حالت SYNC_OPTION:
          require_sync = true;
          توڑو;
        حالت NO_SYNC_OPTION:
          require_sync = false;
          توڑو;

        حالت 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        حالت 't':
          جوڑو_fs_type (optarg);
          توڑو;

        حالت 'v':		/* For SysV compatibility.  */
          /* ignore */
          توڑو;
        حالت 'x':
          جوڑو_excluded_fs_type (optarg);
          توڑو;

        حالت OUTPUT_OPTION:
          اگر (سرخی_منوال == INODES_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          اگر (posix_format && سرخی_منوال == DEFAULT_منوال)
            {
              error (0, 0, پیغام_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          اگر (print_type)
            {
              error (0, 0, پیغام_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          سرخی_منوال = OUTPUT_منوال;
          اگر (optarg)
            decode_output_arg (optarg);
          توڑو;

        حالت کل_OPTION:
          print_grand_کل = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (human_output_opts == -1)
    {
      اگر (posix_format)
        {
          human_output_opts = 0;
          output_بلاک_سائز = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      ورنہ
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_بلاک_سائز);
    }

  اگر (سرخی_منوال == INODES_منوال || سرخی_منوال == OUTPUT_منوال)
    ;
  ورنہ اگر (human_output_opts & human_autoscale)
    سرخی_منوال = HUMAN_منوال;
  ورنہ اگر (posix_format)
    سرخی_منوال = POSIX_منوال;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool مطابقت = false;
    برائے (ساخت fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_اگلا)
      {
        برائے (ساخت fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_اگلا)
          {
            اگر (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                مطابقت = true;
                توڑو;
              }
          }
      }
    اگر (مطابقت)
      واپس EXIT_FAILURE;
  }

  اگر (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, حجم *stats);
      برائے (صحیح i = optind; i < argc; ++i)
        {
          صحیح err = automount_stat_err (argv[i], &stats[i - optind]);
          اگر (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_حالت = EXIT_FAILURE;
              argv[i] = تہی;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != تہی
                            || fs_exclude_list != تہی
                            || print_type
                            || میدان_ڈیٹا_2[FSTYPE_میدان].مستعمل
                            || show_local_fs));

  اگر (mount_list == تہی)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      صحیح حالت_2 = 0;
      اگر ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != تہی
               || fs_exclude_list != تہی))
        {
          حالت_2 = EXIT_FAILURE;
        }
      حرف ثابت *warning = (حالت_2 == 0 ? _("Warning: ") : "");
      error (حالت_2, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  اگر (require_sync)
    sync ();

  get_میدان_list ();
  get_سرخی ();

  اگر (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      برائے (صحیح i = optind; i < argc; ++i)
        اگر (argv[i])
          get_اندراج (argv[i], &stats[i - optind]);
    }
  ورنہ
    get_all_entries ();

  اگر (file_systems_processed)
    {
      اگر (print_grand_کل)
        get_dev ("total",
                 (میدان_ڈیٹا_2[ماخذ_میدان].مستعمل ? "-" : "total"),
                 تہی, تہی, تہی, false, false, &grand_fsu, false);

      print_جدول ();
    }
  ورنہ
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      اگر (exit_حالت == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_حالت);
}
