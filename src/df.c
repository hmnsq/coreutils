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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <c-ctype.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "fsusage.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "mbswidth.h"
#అన్తర్భావ "mountlist.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "find-mount-point.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "xstrtol-error.h"
#అన్తర్భావ "xvasprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "df"

#నిర్వచించు AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert")

నిర్మాణం devlist
{
  dev_t dev_అంకె;
  నిర్మాణం mount_నమోదు *me;
  నిర్మాణం devlist *తదుపరి;
  నిర్మాణం devlist *తదుపరి_same_dev;
  నిర్మాణం devlist *చూసిన_చివరి; /* valid for hashed devlist entries only */
};

/* Filled with device numbers of examined file systems to avoid
   duplicates in output.  */
స్థిర Hash_పట్టిక *devlist_పట్టిక;

/* If true, show even file systems with zero size or
   uninteresting types.  */
స్థిర bool show_all_fs;

/* If true, show only local file systems.  */
స్థిర bool show_local_fs;

/* If true, output data for each file system corresponding to a
   command line argument -- even if it's a dummy (automounter) entry.  */
స్థిర bool show_listed_fs;

/* Human-readable options for output.  */
స్థిర పూర్ణాంకం human_output_opts = -1;

/* The units to use when printing sizes.  */
స్థిర uintmax_t output_ఖండం_కొలత;

/* True if a file system has been processed for output.  */
స్థిర bool file_systems_processed;

/* If true, invoke the 'sync' system call before getting any usage data.
   Using this option can make df very slow, especially with many or very
   busy file systems.  This may make a difference on some systems --
   SunOS 4.1.3, for one.  It is *not* necessary on GNU/Linux.  */
స్థిర bool require_sync;

/* Desired exit status.  */
స్థిర పూర్ణాంకం exit_స్థితి;

/* A file system type to display.  */

నిర్మాణం fs_type_list
{
  అక్షరం *fs_name;
  నిర్మాణం fs_type_list *fs_తదుపరి;
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

స్థిర నిర్మాణం fs_type_list *fs_select_list;

/* Linked list of file system types to omit.
   If the list is empty, don't exclude any types.  */

స్థిర నిర్మాణం fs_type_list *fs_exclude_list;

/* Linked list of mounted file systems.  */
స్థిర నిర్మాణం mount_నమోదు *mount_list;

/* If true, print file system type as well.  */
స్థిర bool print_type;

/* If true, print a grand total at the end.  */
స్థిర bool print_grand_మొత్తం;

/* Grand total data.  */
స్థిర నిర్మాణం fs_usage grand_fsu = { .fsu_blocksize = 1 };

/* Display modes.  */
స్థిర గణన
{
  DEFAULT_బహుళకం,
  INODES_బహుళకం,
  HUMAN_బహుళకం,
  POSIX_బహుళకం,
  OUTPUT_బహుళకం
} శీర్షిక_బహుళకం = DEFAULT_బహుళకం;

/* Displayable fields.  */
రకనిర్వచనం గణన
{
  ఆధారం_క్షేత్రం, /* file system */
  FSTYPE_క్షేత్రం, /* FS type */
  కొలత_క్షేత్రం,   /* FS size */
  వాడిన_క్షేత్రం,   /* FS size used  */
  AVAIL_క్షేత్రం,  /* FS size available */
  PCENT_క్షేత్రం,  /* percent used */
  ITOTAL_క్షేత్రం, /* inode total */
  IUSED_క్షేత్రం,  /* inodes used */
  IAVAIL_క్షేత్రం, /* inodes available */
  IPCENT_క్షేత్రం, /* inodes used in percent */
  లక్ష్యం_క్షేత్రం, /* mount point */
  FILE_క్షేత్రం,   /* specified file name */
  INVALID_క్షేత్రం /* validation marker */
} display_క్షేత్రం_t;

/* Flag if a field contains a block, an inode or another value.  */
రకనిర్వచనం గణన
{
  ఖండం_FLD, /* Block values field */
  INODE_FLD, /* Inode values field */
  OTHER_FLD  /* Neutral field, e.g. target */
} క్షేత్రం_type_t;

/* Attributes of a display field.  */
నిర్మాణం క్షేత్రం_దత్తాంశం_t
{
  display_క్షేత్రం_t క్షేత్రం;
  అక్షరం స్థిరరాశి *arg;
  క్షేత్రం_type_t క్షేత్రం_type;
  అక్షరం స్థిరరాశి *caption;/* NULL means use default header of this field.  */
  పూర్ణాంకం వెడల్పు;          /* Auto adjusted (up) widths used to align columns.  */
  bool align_కుడి;   /* Whether to right-align columns, not left-align.  */
  bool వాడిన;
};

/* Header strings, minimum width and alignment for the above fields.  */
స్థిర నిర్మాణం క్షేత్రం_దత్తాంశం_t క్షేత్రం_దత్తాంశం_2[] = {
  [ఆధారం_క్షేత్రం] = { ఆధారం_క్షేత్రం,
    "source", OTHER_FLD, N_("Filesystem"), 14, false,  false },

  [FSTYPE_క్షేత్రం] = { FSTYPE_క్షేత్రం,
    "fstype", OTHER_FLD, N_("Type"),        4, false,  false },

  [కొలత_క్షేత్రం] = { కొలత_క్షేత్రం,
    "size",   ఖండం_FLD, N_("blocks"),      5, true, false },

  [వాడిన_క్షేత్రం] = { వాడిన_క్షేత్రం,
    "used",   ఖండం_FLD, N_("Used"),        5, true, false },

  [AVAIL_క్షేత్రం] = { AVAIL_క్షేత్రం,
    "avail",  ఖండం_FLD, N_("Available"),   5, true, false },

  [PCENT_క్షేత్రం] = { PCENT_క్షేత్రం,
    "pcent",  ఖండం_FLD, N_("Use%"),        4, true, false },

  [ITOTAL_క్షేత్రం] = { ITOTAL_క్షేత్రం,
    "itotal", INODE_FLD, N_("Inodes"),      5, true, false },

  [IUSED_క్షేత్రం] = { IUSED_క్షేత్రం,
    "iused",  INODE_FLD, N_("IUsed"),       5, true, false },

  [IAVAIL_క్షేత్రం] = { IAVAIL_క్షేత్రం,
    "iavail", INODE_FLD, N_("IFree"),       5, true, false },

  [IPCENT_క్షేత్రం] = { IPCENT_క్షేత్రం,
    "ipcent", INODE_FLD, N_("IUse%"),       4, true, false },

  [లక్ష్యం_క్షేత్రం] = { లక్ష్యం_క్షేత్రం,
    "target", OTHER_FLD, N_("Mounted on"),  0, false,  false },

  [FILE_క్షేత్రం] = { FILE_క్షేత్రం,
    "file",   OTHER_FLD, N_("File"),        0, false,  false }
};

స్థిర అక్షరం స్థిరరాశి *all_వాదనలు_string =
  "source,fstype,itotal,iused,iavail,ipcent,size,"
  "used,avail,pcent,file,target";

స్థిర నిర్మాణం
{
  /* The current number of processed rows (including header).  */
  idx_t nrows;

  /* The number of slots allocated for rows.  */
  idx_t nrows_alloc;

  /* Storage for pointers for each string (cell of table).  */
  అక్షరం ***అడ్డువరుసలు COUNTED_BY (nrows);

  /* The current number of output columns.  */
  idx_t ncolumns;

  /* The allocated number of output columns.  */
  idx_t ncolumns_alloc;

  /* Storage for the definition of output columns.  */
  నిర్మాణం క్షేత్రం_దత్తాంశం_t **columns COUNTED_BY (ncolumns);
} పట్టిక;

/* Field values.  */
నిర్మాణం క్షేత్రం_విలువలు_t
{
  uintmax_t ఆదానం_units;
  uintmax_t output_units;
  uintmax_t మొత్తం;
  uintmax_t అందుబాటు;
  bool negate_అందుబాటు;
  uintmax_t అందుబాటు_గమ్యం_మూలం;
  uintmax_t వాడిన;
  bool negate_వాడిన;
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  NO_SYNC_OPTION = CHAR_MAX + 1,
  SYNC_OPTION,
  మొత్తం_OPTION,
  OUTPUT_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"block-size", required_వాదం, ఖాళీ, 'B'},
  {"inodes", no_వాదం, ఖాళీ, 'i'},
  {"human-readable", no_వాదం, ఖాళీ, 'h'},
  {"si", no_వాదం, ఖాళీ, 'H'},
  {"local", no_వాదం, ఖాళీ, 'l'},
  {"output", optional_వాదం, ఖాళీ, OUTPUT_OPTION},
  {"portability", no_వాదం, ఖాళీ, 'P'},
  {"print-type", no_వాదం, ఖాళీ, 'T'},
  {"sync", no_వాదం, ఖాళీ, SYNC_OPTION},
  {"no-sync", no_వాదం, ఖాళీ, NO_SYNC_OPTION},
  {"total", no_వాదం, ఖాళీ, మొత్తం_OPTION},
  {"type", required_వాదం, ఖాళీ, 't'},
  {"exclude-type", required_వాదం, ఖాళీ, 'x'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Stat FILE and put the results into *ST.  Return 0 if successful, an
   error number otherwise.  Try to open FILE before statting, to
   trigger automounts.  */

స్థిర పూర్ణాంకం
automount_stat_err (అక్షరం స్థిరరాశి *file, నిర్మాణం stat *st)
{
  పూర్ణాంకం fd = open (file, O_RDONLY | O_NOCTTY | O_NONBLOCK);
  ఒకవేళ (fd < 0)
    {
      ఒకవేళ (errno == ENOENT || errno == ENOTDIR)
        తిరిగి errno;
      తిరిగి stat (file, st) == 0 ? 0 : errno;
    }
  లేకపోతే
    {
      పూర్ణాంకం err = fstat (fd, st) == 0 ? 0 : errno;
      close (fd);
      తిరిగి err;
    }
}

గణన { MBSWIDTH_FLAGS = MBSW_REJECT_INVALID | MBSW_REJECT_UNPRINTABLE };

/* Replace problematic chars with '?'.
   Since only control characters are currently considered,
   this should work in all encodings.  */

స్థిర శూన్య
replace_control_chars (అక్షరం *cell)
{
  అక్షరం *p = cell;
  అయితే (*p)
    {
      ఒకవేళ (c_iscntrl (*p))
        *p = '?';
      p++;
    }
}

/* Replace problematic chars with '?'.  */

స్థిర శూన్య
replace_invalid_chars (అక్షరం *cell)
{
  అక్షరం *srcend = cell + అక్షరశ్రేణి_పొడవు (cell);
  అక్షరం *dst = cell;
  mbstate_t mbstate; mbszero (&mbstate);
  size_t n;

  కొరకు (అక్షరం *src = cell; src != srcend; src += n)
    {
      char32_t wc;
      size_t srcbytes = srcend - src;
      n = mbrtoc32 (&wc, src, srcbytes, &mbstate);
      bool ok = n <= srcbytes;

      ఒకవేళ (ok)
        ok = !c32iscntrl (wc);
      లేకపోతే
        n = 1;

      ఒకవేళ (ok)
        {
          memmove (dst, src, n);
          dst += n;
        }
      లేకపోతే
        {
          *dst++ = '?';
          mbszero (&mbstate);
        }
    }

  *dst = '\0';
}

స్థిర శూన్య
replace_problematic_chars (అక్షరం *cell)
{
  స్థిర పూర్ణాంకం tty_బహిర్గతం = -1;
  ఒకవేళ (tty_బహిర్గతం < 0)
    tty_బహిర్గతం = isatty (STDOUT_FILENO);

  (tty_బహిర్గతం ? replace_invalid_chars : replace_control_chars) (cell) ;
}


/* Dynamically allocate a row of pointers in TABLE.ROWS, which
   can then be accessed with standard 2D array notation.  */

స్థిర శూన్య
alloc_పట్టిక_అడ్డువరుస (శూన్య)
{
  ఒకవేళ (పట్టిక.nrows == పట్టిక.nrows_alloc)
    పట్టిక.అడ్డువరుసలు = xpalloc (పట్టిక.అడ్డువరుసలు, &పట్టిక.nrows_alloc, 1, -1,
                          పరిమాణం *పట్టిక.అడ్డువరుసలు);
  idx_t స్థిరరాశి అడ్డువరుస_idx = పట్టిక.nrows++;
  పట్టిక.అడ్డువరుసలు[అడ్డువరుస_idx] = xinmalloc (పట్టిక.ncolumns, పరిమాణం *పట్టిక.అడ్డువరుసలు[0]);
}

/* Output each cell in the table, accounting for the
   alignment and max width of each column.  */

స్థిర శూన్య
print_పట్టిక (శూన్య)
{
  కొరకు (idx_t అడ్డువరుస = 0; అడ్డువరుస < పట్టిక.nrows; అడ్డువరుస++)
    {
      కొరకు (idx_t నిలువు = 0; నిలువు < పట్టిక.ncolumns; నిలువు++)
        {
          అక్షరం *cell = పట్టిక.అడ్డువరుసలు[అడ్డువరుస][నిలువు];

          /* Note the SOURCE_FIELD used to be displayed on it's own line
             if (!posix_format && mbswidth (cell) > 20), but that
             functionality was probably more problematic than helpful,
             hence changed in commit v8.10-40-g99679ff.  */
          ఒకవేళ (నిలువు != 0)
            అక్షరం_రాయి (' ');

          పూర్ణాంకం వెడల్పు = mbswidth (cell, MBSWIDTH_FLAGS);
          పూర్ణాంకం fill = వెడల్పు < 0 ? 0 : పట్టిక.columns[నిలువు]->వెడల్పు - వెడల్పు;
          ఒకవేళ (పట్టిక.columns[నిలువు]->align_కుడి)
            కొరకు (; 0 < fill; fill--)
              అక్షరం_రాయి (' ');
          fputs (cell, stdout);
          ఒకవేళ (నిలువు + 1 < పట్టిక.ncolumns)
            కొరకు (; 0 < fill; fill--)
              అక్షరం_రాయి (' ');
        }
      అక్షరం_రాయి ('\n');
    }
}

/* Dynamically allocate a struct field_t in TABLE.COLUMNS, which
   can then be accessed with standard array notation.  */

స్థిర శూన్య
alloc_క్షేత్రం (పూర్ణాంకం f, అక్షరం స్థిరరాశి *c)
{
  ఒకవేళ (పట్టిక.ncolumns == పట్టిక.ncolumns_alloc)
    పట్టిక.columns = xpalloc (పట్టిక.columns, &పట్టిక.ncolumns_alloc, 1, -1,
                             పరిమాణం *పట్టిక.columns);
  idx_t స్థిరరాశి స్తంభం_idx = పట్టిక.ncolumns++;
  పట్టిక.columns[స్తంభం_idx] = &క్షేత్రం_దత్తాంశం_2[f];
  ఒకవేళ (c != ఖాళీ)
    క్షేత్రం_దత్తాంశం_2[f].caption = c;

  affirm (!క్షేత్రం_దత్తాంశం_2[f].వాడిన);

  /* Mark field as used.  */
  క్షేత్రం_దత్తాంశం_2[f].వాడిన = true;
}


/* Given a string, ARG, containing a comma-separated list of arguments
   to the --output option, add the appropriate fields to columns.  */
స్థిర శూన్య
decode_output_arg (అక్షరం స్థిరరాశి *arg)
{
  అక్షరం *arg_writable = xstrdup (arg);
  అక్షరం *s = arg_writable;
  చేయి
    {
      /* find next comma */
      అక్షరం *comma = strchr (s, ',');

      /* If we found a comma, put a NUL in its place and advance.  */
      ఒకవేళ (comma)
        *comma++ = 0;

      /* process S.  */
      display_క్షేత్రం_t క్షేత్రం = INVALID_క్షేత్రం;
      కొరకు (idx_t i = 0; i < countof (క్షేత్రం_దత్తాంశం_2); i++)
        {
          ఒకవేళ (streq (క్షేత్రం_దత్తాంశం_2[i].arg, s))
            {
              క్షేత్రం = i;
              ఆపు;
            }
        }
      ఒకవేళ (క్షేత్రం == INVALID_క్షేత్రం)
        {
          error (0, 0, _("option --output: field %s unknown"), quote (s));
          usage (EXIT_FAILURE);
        }

      ఒకవేళ (క్షేత్రం_దత్తాంశం_2[క్షేత్రం].వాడిన)
        {
          /* Prevent the fields from being used more than once.  */
          error (0, 0, _("option --output: field %s used more than once"),
                 quote (క్షేత్రం_దత్తాంశం_2[క్షేత్రం].arg));
          usage (EXIT_FAILURE);
        }

      ఎంపిక (క్షేత్రం)
        {
        స్థితి ఆధారం_క్షేత్రం:
        స్థితి FSTYPE_క్షేత్రం:
        స్థితి వాడిన_క్షేత్రం:
        స్థితి PCENT_క్షేత్రం:
        స్థితి ITOTAL_క్షేత్రం:
        స్థితి IUSED_క్షేత్రం:
        స్థితి IAVAIL_క్షేత్రం:
        స్థితి IPCENT_క్షేత్రం:
        స్థితి లక్ష్యం_క్షేత్రం:
        స్థితి FILE_క్షేత్రం:
          alloc_క్షేత్రం (క్షేత్రం, ఖాళీ);
          ఆపు;

        స్థితి కొలత_క్షేత్రం:
          alloc_క్షేత్రం (క్షేత్రం, N_("Size"));
          ఆపు;

        స్థితి AVAIL_క్షేత్రం:
          alloc_క్షేత్రం (క్షేత్రం, N_("Avail"));
          ఆపు;

        స్థితి INVALID_క్షేత్రం:
        అప్రమేయ:
          affirm (!"invalid field");
        }
      s = comma;
    }
  అయితే (s);

  విడుదల (arg_writable);
}

/* Get the appropriate columns for the mode.  */
స్థిర శూన్య
get_క్షేత్రం_list (శూన్య)
{
  ఎంపిక (శీర్షిక_బహుళకం)
    {
    స్థితి DEFAULT_బహుళకం:
      alloc_క్షేత్రం (ఆధారం_క్షేత్రం, ఖాళీ);
      ఒకవేళ (print_type)
        alloc_క్షేత్రం (FSTYPE_క్షేత్రం, ఖాళీ);
      alloc_క్షేత్రం (కొలత_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (వాడిన_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (AVAIL_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (PCENT_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (లక్ష్యం_క్షేత్రం, ఖాళీ);
      ఆపు;

    స్థితి HUMAN_బహుళకం:
      alloc_క్షేత్రం (ఆధారం_క్షేత్రం, ఖాళీ);
      ఒకవేళ (print_type)
        alloc_క్షేత్రం (FSTYPE_క్షేత్రం, ఖాళీ);

      alloc_క్షేత్రం (కొలత_క్షేత్రం,   N_("Size"));
      alloc_క్షేత్రం (వాడిన_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (AVAIL_క్షేత్రం,  N_("Avail"));
      alloc_క్షేత్రం (PCENT_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (లక్ష్యం_క్షేత్రం, ఖాళీ);
      ఆపు;

    స్థితి INODES_బహుళకం:
      alloc_క్షేత్రం (ఆధారం_క్షేత్రం, ఖాళీ);
      ఒకవేళ (print_type)
        alloc_క్షేత్రం (FSTYPE_క్షేత్రం, ఖాళీ);
      alloc_క్షేత్రం (ITOTAL_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (IUSED_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (IAVAIL_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (IPCENT_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (లక్ష్యం_క్షేత్రం,  ఖాళీ);
      ఆపు;

    స్థితి POSIX_బహుళకం:
      alloc_క్షేత్రం (ఆధారం_క్షేత్రం, ఖాళీ);
      ఒకవేళ (print_type)
        alloc_క్షేత్రం (FSTYPE_క్షేత్రం, ఖాళీ);
      alloc_క్షేత్రం (కొలత_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (వాడిన_క్షేత్రం,   ఖాళీ);
      alloc_క్షేత్రం (AVAIL_క్షేత్రం,  ఖాళీ);
      alloc_క్షేత్రం (PCENT_క్షేత్రం,  N_("Capacity"));
      alloc_క్షేత్రం (లక్ష్యం_క్షేత్రం, ఖాళీ);
      ఆపు;

    స్థితి OUTPUT_బహుళకం:
      ఒకవేళ (!పట్టిక.ncolumns)
        {
          /* Add all fields if --output was given without a field list.  */
          decode_output_arg (all_వాదనలు_string);
        }
      ఆపు;

    అప్రమేయ:
      unreachable ();
    }
}

/* Obtain the appropriate header entries.  */

స్థిర శూన్య
get_శీర్షిక (శూన్య)
{
  alloc_పట్టిక_అడ్డువరుస ();

  కొరకు (idx_t నిలువు = 0; నిలువు < పట్టిక.ncolumns; నిలువు++)
    {
      అక్షరం *cell;
      అక్షరం స్థిరరాశి *శీర్షిక = _(పట్టిక.columns[నిలువు]->caption);

      ఒకవేళ (పట్టిక.columns[నిలువు]->క్షేత్రం == కొలత_క్షేత్రం
          && (శీర్షిక_బహుళకం == DEFAULT_బహుళకం
              || (శీర్షిక_బహుళకం == OUTPUT_బహుళకం
                  && !(human_output_opts & human_autoscale))))
        {
          అక్షరం buf[LONGEST_HUMAN_READABLE + 1];

          పూర్ణాంకం opts = (human_suppress_బిందువు_zero
                      | human_autoscale | human_SI
                      | (human_output_opts
                         & (human_సమూహం_digits | human_పునాది_1024 | human_B)));

          /* Prefer the base that makes the human-readable value more exact,
             if there is a difference.  */

          uintmax_t q1000 = output_ఖండం_కొలత;
          uintmax_t q1024 = output_ఖండం_కొలత;
          bool divisible_by_1000;
          bool divisible_by_1024;

          చేయి
            {
              divisible_by_1000 = q1000 % 1000 == 0;  q1000 /= 1000;
              divisible_by_1024 = q1024 % 1024 == 0;  q1024 /= 1024;
            }
          అయితే (divisible_by_1000 & divisible_by_1024);

          ఒకవేళ (divisible_by_1000 < divisible_by_1024)
            opts |= human_పునాది_1024;
          ఒకవేళ (divisible_by_1024 < divisible_by_1000)
            opts &= ~human_పునాది_1024;
          ఒకవేళ (! (opts & human_పునాది_1024))
            opts |= human_B;

          అక్షరం *అంకె = human_readable (output_ఖండం_కొలత, buf, opts, 1, 1);

          /* Reset the header back to the default in OUTPUT_MODE.  */
          శీర్షిక = _("blocks");

          /* TRANSLATORS: this is the "1K-blocks" header in "df" output.  */
          cell = xasprintf (_("%s-%s"), అంకె, శీర్షిక);
        }
      లేకపోతే ఒకవేళ (శీర్షిక_బహుళకం == POSIX_బహుళకం
               && పట్టిక.columns[నిలువు]->క్షేత్రం == కొలత_క్షేత్రం)
        {
          /* TRANSLATORS: this is the "1024-blocks" header in "df -P".  */
          cell = xasprintf (_("%ju-%s"), output_ఖండం_కొలత, శీర్షిక);
        }
      లేకపోతే
        cell = xstrdup (శీర్షిక);

      replace_problematic_chars (cell);

      పట్టిక.అడ్డువరుసలు[పట్టిక.nrows - 1][నిలువు] = cell;

      పూర్ణాంకం cell_వెడల్పు = mbswidth (cell, MBSWIDTH_FLAGS);
      పట్టిక.columns[నిలువు]->వెడల్పు = MAX (పట్టిక.columns[నిలువు]->వెడల్పు, cell_వెడల్పు);
    }
}

/* Is FSTYPE a type of file system that should be listed?  */

ATTRIBUTE_శుద్ధ
స్థిర bool
selected_fstype (అక్షరం స్థిరరాశి *fstype)
{
  ఒకవేళ (fs_select_list == ఖాళీ || fstype == ఖాళీ)
    తిరిగి true;
  కొరకు (స్థిరరాశి నిర్మాణం fs_type_list *fsp = fs_select_list; fsp;
       fsp = fsp->fs_తదుపరి)
    ఒకవేళ (streq (fstype, fsp->fs_name))
      తిరిగి true;
  తిరిగి false;
}

/* Is FSTYPE a type of file system that should be omitted?  */

ATTRIBUTE_శుద్ధ
స్థిర bool
excluded_fstype (అక్షరం స్థిరరాశి *fstype)
{
  ఒకవేళ (fs_exclude_list == ఖాళీ || fstype == ఖాళీ)
    తిరిగి false;
  కొరకు (స్థిరరాశి నిర్మాణం fs_type_list *fsp = fs_exclude_list; fsp;
       fsp = fsp->fs_తదుపరి)
    ఒకవేళ (streq (fstype, fsp->fs_name))
      తిరిగి true;
  తిరిగి false;
}

స్థిర size_t
devlist_hash (శూన్య స్థిరరాశి *x, size_t పట్టిక_కొలత)
{
  నిర్మాణం devlist స్థిరరాశి *p = x;
  తిరిగి (uintmax_t) {p->dev_అంకె} % పట్టిక_కొలత;
}

స్థిర bool
devlist_compare (శూన్య స్థిరరాశి *x, శూన్య స్థిరరాశి *y)
{
  నిర్మాణం devlist స్థిరరాశి *a = x;
  నిర్మాణం devlist స్థిరరాశి *b = y;
  తిరిగి a->dev_అంకె == b->dev_అంకె;
}

స్థిర నిర్మాణం devlist *
devlist_for_dev (dev_t dev)
{
  ఒకవేళ (devlist_పట్టిక == ఖాళీ)
    తిరిగి ఖాళీ;
  నిర్మాణం devlist dev_నమోదు;
  dev_నమోదు.dev_అంకె = dev;

  నిర్మాణం devlist *దొరికింది = hash_lookup (devlist_పట్టిక, &dev_నమోదు);
  ఒకవేళ (దొరికింది == ఖాళీ)
    తిరిగి ఖాళీ;

  /* Return the last devlist entry we have seen with this dev_num */
  తిరిగి దొరికింది->చూసిన_చివరి;
}

/* Filter mount list by skipping duplicate entries.
   In the case of duplicates - based on the device number - the mount entry
   with a '/' in its me_devname (i.e., not pseudo name like tmpfs) wins.
   If both have a real devname (e.g. bind mounts), then that with the shorter
   me_mountdir wins.  If DEVICES_ONLY is true (set with df -a), only update
   the global devlist_table, rather than filtering the global mount_list.  */

స్థిర శూన్య
filter_mount_list (bool devices_only)
{
  /* Temporary list to keep entries ordered.  */
  నిర్మాణం devlist *device_list = ఖాళీ;
  పూర్ణాంకం mount_list_కొలత = 0;

  కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me; me = me->me_తదుపరి)
    mount_list_కొలత++;

  devlist_పట్టిక = hash_initialize (mount_list_కొలత, ఖాళీ,
                                   devlist_hash, devlist_compare, ఖాళీ);
  ఒకవేళ (devlist_పట్టిక == ఖాళీ)
    xalloc_die ();

  /* Sort all 'wanted' entries into the list device_list.  */
  కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me;)
    {
      నిర్మాణం stat buf;
      నిర్మాణం mount_నమోదు *discard_me = ఖాళీ;
      నిర్మాణం devlist *చివరి_చూసిన_dev = ఖాళీ, *చూసిన_dev = ఖాళీ;

      /* Avoid stating remote file systems as that may hang.
         On Linux we probably have me_dev populated from /proc/self/mountinfo,
         however we still stat() in case another device was mounted later.  */
      ఒకవేళ ((me->me_remote && show_local_fs)
          || (me->me_dummy && !show_all_fs && !show_listed_fs)
          || (!selected_fstype (me->me_type) || excluded_fstype (me->me_type))
          || -1 == stat (me->me_mountdir, &buf))
        {
          /* If remote, and showing just local, or FS type is excluded,
             add ME for filtering later.
             If stat failed; add ME to be able to complain about it later.  */
          buf.st_dev = me->me_dev;
        }
      లేకపోతే
        {
          /* If we've already seen this device...  */
          చివరి_చూసిన_dev = చూసిన_dev = devlist_for_dev (buf.st_dev);

          కొరకు (; చూసిన_dev && ! discard_me; చూసిన_dev = చూసిన_dev->తదుపరి_same_dev)
            {
              bool లక్ష్యం_nearer_మూలం = అక్షరశ్రేణి_పొడవు (చూసిన_dev->me->me_mountdir)
                                        > అక్షరశ్రేణి_పొడవు (me->me_mountdir);
              /* With bind mounts, prefer items nearer the root of the source */
              bool ఆధారం_క్రింద_మూలం = చూసిన_dev->me->me_mntroot != ఖాళీ
                                       && me->me_mntroot != ఖాళీ
                                       && (అక్షరశ్రేణి_పొడవు (చూసిన_dev->me->me_mntroot)
                                           < అక్షరశ్రేణి_పొడవు (me->me_mntroot));
              ఒకవేళ (! print_grand_మొత్తం
                  && me->me_remote && చూసిన_dev->me->me_remote
                  && ! streq (చూసిన_dev->me->me_devname, me->me_devname))
                {
                  /* Don't discard remote entries with different locations,
                     as these are more likely to be explicitly mounted.
                     However avoid this when producing a total to give
                     a more accurate value in that case.  */
                }
              లేకపోతే ఒకవేళ ((strchr (me->me_devname, '/')
                       /* let "real" devices with '/' in the name win.  */
                        && ! strchr (చూసిన_dev->me->me_devname, '/'))
                       /* let points towards the root of the device win.  */
                       || (లక్ష్యం_nearer_మూలం && ! ఆధారం_క్రింద_మూలం)
                       /* let an entry overmounted on a new device win...  */
                       || (! streq (చూసిన_dev->me->me_devname, me->me_devname)
                           /* ... but only when matching an existing mnt point,
                              to avoid problematic replacement when given
                              inaccurate mount lists, seen with some chroot
                              environments for example.  */
                           && streq (me->me_mountdir,
                                     చూసిన_dev->me->me_mountdir)))
                {
                  /* Discard mount entry for existing device.  */
                  discard_me = చూసిన_dev->me;
                  చూసిన_dev->me = me;
                }
              లేకపోతే
                {
                  /* Discard mount entry currently being processed.  */
                  discard_me = me;
                }

            }
        }

      ఒకవేళ (discard_me)
        {
          me = me->me_తదుపరి;
          ఒకవేళ (! devices_only)
            free_mount_నమోదు (discard_me);
        }
      లేకపోతే
        {
          /* Add the device number to the device_table.  */
          నిర్మాణం devlist *devlist = xmalloc (పరిమాణం *devlist);
          devlist->me = me;
          devlist->dev_అంకె = buf.st_dev;
          devlist->తదుపరి_same_dev = చివరి_చూసిన_dev;
          devlist->తదుపరి = device_list;
          device_list = devlist;

          నిర్మాణం devlist *hash_నమోదు = hash_చొప్పించు (devlist_పట్టిక, devlist);
          ఒకవేళ (hash_నమోదు == ఖాళీ)
            xalloc_die ();
          /* Ensure lookups use this latest devlist.  */
          hash_నమోదు->చూసిన_చివరి = devlist;

          me = me->me_తదుపరి;
        }
    }

  /* Finally rebuild the mount_list from the devlist.  */
  ఒకవేళ (! devices_only)
    {
      mount_list = ఖాళీ;
      అయితే (device_list)
        {
          /* Add the mount entry.  */
          నిర్మాణం mount_నమోదు *me = device_list->me;
          me->me_తదుపరి = mount_list;
          mount_list = me;
          నిర్మాణం devlist *తదుపరి = device_list->తదుపరి;
          విడుదల (device_list);
          device_list = తదుపరి;
        }
      hash_free (devlist_పట్టిక);
      devlist_పట్టిక = ఖాళీ;
    }
}


/* Search a mount entry list for device id DEV.
   Return the corresponding mount entry if found or NULL if not.  */

ATTRIBUTE_శుద్ధ
స్థిర నిర్మాణం mount_నమోదు స్థిరరాశి *
me_for_dev (dev_t dev)
{
  నిర్మాణం devlist *dl = devlist_for_dev (dev);
  ఒకవేళ (dl)
        తిరిగి dl->me;

  తిరిగి ఖాళీ;
}

/* Return true if N is a known integer value.  On many file systems,
   UINTMAX_MAX represents an unknown value; on AIX, UINTMAX_MAX - 1
   represents unknown.  Use a rule that works on AIX file systems, and
   that almost-always works on other types.  */
స్థిర bool
తెలిసిన_విలువ (uintmax_t n)
{
  తిరిగి n < UINTMAX_MAX - 1;
}

/* Like human_readable (N, BUF, human_output_opts, INPUT_UNITS, OUTPUT_UNITS),
   except:

    - If NEGATIVE, then N represents a negative number,
      expressed in two's complement.
    - Otherwise, return "-" if N is unknown.  */

స్థిర అక్షరం స్థిరరాశి *
df_readable (bool negative, uintmax_t n, అక్షరం *buf,
             uintmax_t ఆదానం_units, uintmax_t output_units)
{
  ఒకవేళ (! తెలిసిన_విలువ (n) && !negative)
    తిరిగి "-";
  లేకపోతే
    {
      అక్షరం *p = human_readable (negative ? -n : n, buf + negative,
                                human_output_opts, ఆదానం_units, output_units);
      ఒకవేళ (negative)
        *--p = '-';
      తిరిగి p;
    }
}

/* Add integral value while using uintmax_t for value part and separate
   negation flag.  It adds value of SRC and SRC_NEG to DEST and DEST_NEG.
   The result will be in DEST and DEST_NEG.  See df_readable to understand
   how the negation flag is used.  */
స్థిర శూన్య
కూడు_uint_with_neg_flag (uintmax_t *dest, bool *dest_neg,
                        uintmax_t src, bool src_neg)
{
  ఒకవేళ (*dest_neg == src_neg)
    {
      *dest += src;
      తిరిగి;
    }

  ఒకవేళ (*dest_neg)
    *dest = -*dest;

  ఒకవేళ (src_neg)
    src = -src;

  ఒకవేళ (src < *dest)
    *dest -= src;
  లేకపోతే
    {
      *dest = src - *dest;
      *dest_neg = src_neg;
    }

  ఒకవేళ (*dest_neg)
    *dest = -*dest;
}

/* Return true if S ends in a string that may be a 36-byte UUID,
   i.e., of the form HHHHHHHH-HHHH-HHHH-HHHH-HHHHHHHHHHHH, where
   each H is an upper or lower case hexadecimal digit.  */
ATTRIBUTE_శుద్ధ
స్థిర bool
has_uuid_suffix (అక్షరం స్థిరరాశి *s)
{
  size_t len = అక్షరశ్రేణి_పొడవు (s);
  తిరిగి (36 < len
          && strspn (s + len - 36, "-0123456789abcdefABCDEF") == 36);
}

/* Obtain the block values BV and inode values IV
   from the file system usage FSU.  */
స్థిర శూన్య
get_క్షేత్రం_విలువలు (నిర్మాణం క్షేత్రం_విలువలు_t *bv,
                  నిర్మాణం క్షేత్రం_విలువలు_t *iv,
                  స్థిరరాశి నిర్మాణం fs_usage *fsu)
{
  /* Inode values.  */
  iv->ఆదానం_units = iv->output_units = 1;
  iv->మొత్తం = fsu->fsu_files;
  iv->అందుబాటు = iv->అందుబాటు_గమ్యం_మూలం = fsu->fsu_ffree;
  iv->negate_అందుబాటు = false;

  iv->వాడిన = UINTMAX_MAX;
  iv->negate_వాడిన = false;
  ఒకవేళ (తెలిసిన_విలువ (iv->మొత్తం) && తెలిసిన_విలువ (iv->అందుబాటు_గమ్యం_మూలం))
    {
      iv->వాడిన = iv->మొత్తం - iv->అందుబాటు_గమ్యం_మూలం;
      iv->negate_వాడిన = (iv->మొత్తం < iv->అందుబాటు_గమ్యం_మూలం);
    }

  /* Block values.  */
  bv->ఆదానం_units = fsu->fsu_blocksize;
  bv->output_units = output_ఖండం_కొలత;
  bv->మొత్తం = fsu->fsu_ఖండాలు;
  bv->అందుబాటు = fsu->fsu_bavail;
  bv->అందుబాటు_గమ్యం_మూలం = fsu->fsu_bfree;
  bv->negate_అందుబాటు = (fsu->fsu_bavail_శిఖరం_బిట్టు_సమితి
                         && తెలిసిన_విలువ (fsu->fsu_bavail));

  bv->వాడిన = UINTMAX_MAX;
  bv->negate_వాడిన = false;
  ఒకవేళ (తెలిసిన_విలువ (bv->మొత్తం) && తెలిసిన_విలువ (bv->అందుబాటు_గమ్యం_మూలం))
    {
      bv->వాడిన = bv->మొత్తం - bv->అందుబాటు_గమ్యం_మూలం;
      bv->negate_వాడిన = (bv->మొత్తం < bv->అందుబాటు_గమ్యం_మూలం);
    }
}

/* Add block and inode values to grand total.  */
స్థిర శూన్య
కూడు_గమ్యం_grand_మొత్తం (నిర్మాణం క్షేత్రం_విలువలు_t *bv, నిర్మాణం క్షేత్రం_విలువలు_t *iv)
{
  ఒకవేళ (తెలిసిన_విలువ (iv->మొత్తం))
    grand_fsu.fsu_files += iv->మొత్తం;
  ఒకవేళ (తెలిసిన_విలువ (iv->అందుబాటు))
    grand_fsu.fsu_ffree += iv->అందుబాటు;

  ఒకవేళ (తెలిసిన_విలువ (bv->మొత్తం))
    grand_fsu.fsu_ఖండాలు += bv->ఆదానం_units * bv->మొత్తం;
  ఒకవేళ (తెలిసిన_విలువ (bv->అందుబాటు_గమ్యం_మూలం))
    grand_fsu.fsu_bfree += bv->ఆదానం_units * bv->అందుబాటు_గమ్యం_మూలం;
  ఒకవేళ (తెలిసిన_విలువ (bv->అందుబాటు))
    కూడు_uint_with_neg_flag (&grand_fsu.fsu_bavail,
                            &grand_fsu.fsu_bavail_శిఖరం_బిట్టు_సమితి,
                            bv->ఆదానం_units * bv->అందుబాటు,
                            bv->negate_అందుబాటు);
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

స్థిర శూన్య
get_dev (అక్షరం స్థిరరాశి *device, అక్షరం స్థిరరాశి *mount_బిందువు, అక్షరం స్థిరరాశి *file,
         అక్షరం స్థిరరాశి *stat_file, అక్షరం స్థిరరాశి *fstype,
         bool me_dummy, bool me_remote,
         స్థిరరాశి నిర్మాణం fs_usage *force_fsu,
         bool process_all)
{
  ఒకవేళ (me_remote && show_local_fs)
    తిరిగి;

  ఒకవేళ (me_dummy && !show_all_fs && !show_listed_fs)
    తిరిగి;

  ఒకవేళ (!selected_fstype (fstype) || excluded_fstype (fstype))
    తిరిగి;

  /* Ignore relative MOUNT_POINTs, which are present for example
     in /proc/mounts on Linux with network namespaces.  */
  ఒకవేళ (!force_fsu && ! IS_ABSOLUTE_FILE_NAME (mount_బిందువు))
    తిరిగి;

  ఒకవేళ (!stat_file)
    stat_file = mount_బిందువు;

  నిర్మాణం fs_usage fsu;
  ఒకవేళ (force_fsu)
    fsu = *force_fsu;
  లేకపోతే ఒకవేళ (get_fs_usage (stat_file, device, &fsu))
    {
      /* If we can't access a system provided entry due
         to it not being present (now), or due to permissions,
         just output placeholder values rather than failing.  */
      ఒకవేళ (process_all && (errno == EACCES || errno == ENOENT))
        {
          ఒకవేళ (! show_all_fs)
            తిరిగి;

          fstype = "-";
          fsu.fsu_bavail_శిఖరం_బిట్టు_సమితి = false;
          fsu.fsu_blocksize = fsu.fsu_ఖండాలు = fsu.fsu_bfree =
          fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
        }
      లేకపోతే
        {
          error (0, errno, "%s", quotef (stat_file));
          exit_స్థితి = EXIT_FAILURE;
          తిరిగి;
        }
    }
  లేకపోతే ఒకవేళ (process_all && show_all_fs)
    {
      /* Ensure we don't output incorrect stats for over-mounted directories.
         Discard stats when the device name doesn't match.  Though don't
         discard when used and current mount entries are both remote due
         to the possibility of aliased host names or exports.  */
      నిర్మాణం stat sb;
      ఒకవేళ (stat (stat_file, &sb) == 0)
        {
          నిర్మాణం mount_నమోదు స్థిరరాశి * dev_me = me_for_dev (sb.st_dev);
          ఒకవేళ (dev_me && ! streq (dev_me->me_devname, device)
              && (! dev_me->me_remote || ! me_remote))
            {
              fstype = "-";
              fsu.fsu_bavail_శిఖరం_బిట్టు_సమితి = false;
              fsu.fsu_blocksize = fsu.fsu_ఖండాలు = fsu.fsu_bfree =
              fsu.fsu_bavail = fsu.fsu_files = fsu.fsu_ffree = UINTMAX_MAX;
            }
        }
    }

  ఒకవేళ (fsu.fsu_ఖండాలు == 0 && !show_all_fs && !show_listed_fs)
    తిరిగి;

  ఒకవేళ (! force_fsu)
    file_systems_processed = true;

  alloc_పట్టిక_అడ్డువరుస ();

  ఒకవేళ (! device)
    device = "-";		/* unknown */

  ఒకవేళ (! file)
    file = "-";			/* unspecified */

  అక్షరం *dev_name = xstrdup (device);
  అక్షరం *resolved_dev;

  /* On some systems, dev_name is a long-named symlink like
     /dev/disk/by-uuid/828fc648-9f30-43d8-a0b1-f7196a2edb66 pointing to a
     much shorter and more useful name like /dev/sda1.  It may also look
     like /dev/mapper/luks-828fc648-9f30-43d8-a0b1-f7196a2edb66 and point to
     /dev/dm-0.  When process_all is true and dev_name is a symlink whose
     name ends with a UUID use the resolved name instead.  */
  ఒకవేళ (process_all
      && has_uuid_suffix (dev_name)
      && (resolved_dev = canonicalize_filename_బహుళకం (dev_name, CAN_EXISTING)))
    {
      విడుదల (dev_name);
      dev_name = resolved_dev;
    }

  ఒకవేళ (! fstype)
    fstype = "-";		/* unknown */

  నిర్మాణం క్షేత్రం_విలువలు_t ఖండం_విలువలు;
  నిర్మాణం క్షేత్రం_విలువలు_t inode_విలువలు;
  get_క్షేత్రం_విలువలు (&ఖండం_విలువలు, &inode_విలువలు, &fsu);

  /* Add to grand total unless processing grand total line.  */
  ఒకవేళ (print_grand_మొత్తం && ! force_fsu)
    కూడు_గమ్యం_grand_మొత్తం (&ఖండం_విలువలు, &inode_విలువలు);

  కొరకు (idx_t నిలువు = 0; నిలువు < పట్టిక.ncolumns; నిలువు++)
    {
      అక్షరం buf[LONGEST_HUMAN_READABLE + 2];
      అక్షరం *cell;

      నిర్మాణం క్షేత్రం_విలువలు_t *v;
      ఎంపిక (పట్టిక.columns[నిలువు]->క్షేత్రం_type)
        {
        స్థితి ఖండం_FLD:
          v = &ఖండం_విలువలు;
          ఆపు;
        స్థితి INODE_FLD:
          v = &inode_విలువలు;
          ఆపు;
        స్థితి OTHER_FLD:
          v = ఖాళీ;
          ఆపు;
        అప్రమేయ:
          affirm (!"bad field_type");
        }

      ఎంపిక (పట్టిక.columns[నిలువు]->క్షేత్రం)
        {
        స్థితి ఆధారం_క్షేత్రం:
          cell = xstrdup (dev_name);
          ఆపు;

        స్థితి FSTYPE_క్షేత్రం:
          cell = xstrdup (fstype);
          ఆపు;

        స్థితి కొలత_క్షేత్రం:
        స్థితి ITOTAL_క్షేత్రం:
          cell = xstrdup (df_readable (false, v->మొత్తం, buf,
                                       v->ఆదానం_units, v->output_units));
          ఆపు;

        స్థితి వాడిన_క్షేత్రం:
        స్థితి IUSED_క్షేత్రం:
          cell = xstrdup (df_readable (v->negate_వాడిన, v->వాడిన, buf,
                                       v->ఆదానం_units, v->output_units));
          ఆపు;

        స్థితి AVAIL_క్షేత్రం:
        స్థితి IAVAIL_క్షేత్రం:
          cell = xstrdup (df_readable (v->negate_అందుబాటు, v->అందుబాటు, buf,
                                       v->ఆదానం_units, v->output_units));
          ఆపు;

        స్థితి PCENT_క్షేత్రం:
        స్థితి IPCENT_క్షేత్రం:
          {
            ద్విగుణ pct = -1;
            ఒకవేళ (! తెలిసిన_విలువ (v->వాడిన) || ! తెలిసిన_విలువ (v->అందుబాటు))
              ;
            లేకపోతే ఒకవేళ (!v->negate_వాడిన
                     && v->వాడిన <= UINTMAX_MAX / 100
                     && v->వాడిన + v->అందుబాటు != 0
                     && (v->వాడిన + v->అందుబాటు < v->వాడిన)
                     == v->negate_అందుబాటు)
              {
                uintmax_t u100 = v->వాడిన * 100;
                uintmax_t nonroot_మొత్తం = v->వాడిన + v->అందుబాటు;
                pct = u100 / nonroot_మొత్తం + (u100 % nonroot_మొత్తం != 0);
              }
            లేకపోతే
              {
                /* The calculation cannot be done easily with integer
                   arithmetic.  Fall back on floating point.  This can suffer
                   from minor rounding errors, but doing it exactly requires
                   multiple precision arithmetic, and it's not worth the
                   aggravation.  */
                ద్విగుణ u = v->negate_వాడిన ? - (ద్విగుణ) - v->వాడిన : v->వాడిన;
                ద్విగుణ a = v->negate_అందుబాటు
                           ? - (ద్విగుణ) - v->అందుబాటు : v->అందుబాటు;
                ద్విగుణ nonroot_మొత్తం = u + a;
                ఒకవేళ (nonroot_మొత్తం)
                  {
                    దీర్ఘ పూర్ణాంకం lipct = pct = u * 100 / nonroot_మొత్తం;
                    ద్విగుణ ipct = lipct;

                    /* Like 'pct = ceil (dpct);', but avoid ceil so that
                       the math library needn't be linked.  */
                    ఒకవేళ (ipct - 1 < pct && pct <= ipct + 1)
                      pct = ipct + (ipct < pct);
                  }
              }

            cell = pct < 0 ? xstrdup ("-") : xasprintf ("%.0f%%", pct);
            ఆపు;
          }

        స్థితి FILE_క్షేత్రం:
          cell = xstrdup (file);
          ఆపు;

        స్థితి లక్ష్యం_క్షేత్రం:
#నిర్వచితమైతే HIDE_AUTOMOUNT_ఉపసర్గ
          /* Don't print the first directory name in MOUNT_POINT if it's an
             artifact of an automounter.  This is a bit too aggressive to be
             the default.  */
          ఒకవేళ (STRNCMP_LIT (mount_బిందువు, "/auto/") == 0)
            mount_బిందువు += 5;
          లేకపోతే ఒకవేళ (STRNCMP_LIT (mount_బిందువు, "/tmp_mnt/") == 0)
            mount_బిందువు += 8;
#అయితే_ముగింపు
          cell = xstrdup (mount_బిందువు);
          ఆపు;

        స్థితి INVALID_క్షేత్రం:
        అప్రమేయ:
          affirm (!"unhandled field");
        }

      affirm (cell);

      replace_problematic_chars (cell);
      పూర్ణాంకం cell_వెడల్పు = mbswidth (cell, MBSWIDTH_FLAGS);
      పట్టిక.columns[నిలువు]->వెడల్పు = MAX (పట్టిక.columns[నిలువు]->వెడల్పు, cell_వెడల్పు);
      పట్టిక.అడ్డువరుసలు[పట్టిక.nrows - 1][నిలువు] = cell;
    }
  విడుదల (dev_name);
}

/* Scan the mount list returning the _last_ device found for MOUNT.
   NULL is returned if MOUNT not found.  The result is malloced.  */
స్థిర అక్షరం *
చివరి_device_for_mount (అక్షరం స్థిరరాశి *mount)
{
  నిర్మాణం mount_నమోదు స్థిరరాశి *le = ఖాళీ;

  కొరకు (నిర్మాణం mount_నమోదు స్థిరరాశి *me = mount_list; me; me = me->me_తదుపరి)
    {
      ఒకవేళ (streq (me->me_mountdir, mount))
        le = me;
    }

  ఒకవేళ (le)
    {
      అక్షరం *devname = le->me_devname;
      అక్షరం *canon_dev = canonicalize_file_name (devname);
      ఒకవేళ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        తిరిగి canon_dev;
      విడుదల (canon_dev);
      తిరిగి xstrdup (le->me_devname);
    }
  లేకపోతే
    తిరిగి ఖాళీ;
}

/* If DEVICE corresponds to a mount point, show its usage
   and return true.  Otherwise, return false.  */
స్థిర bool
get_device (అక్షరం స్థిరరాశి *device)
{
  నిర్మాణం mount_నమోదు స్థిరరాశి *ఉత్తమ_సరిపోలిక = ఖాళీ;
  bool ఉత్తమ_సరిపోలిక_accessible = false;
  bool eclipsed_device = false;
  అక్షరం స్థిరరాశి *file = device;

  అక్షరం *resolved = canonicalize_file_name (device);
  ఒకవేళ (resolved && IS_ABSOLUTE_FILE_NAME (resolved))
    device = resolved;

  size_t ఉత్తమ_సరిపోలిక_len = SIZE_MAX;
  కొరకు (నిర్మాణం mount_నమోదు స్థిరరాశి *me = mount_list; me; me = me->me_తదుపరి)
    {
      /* TODO: Should cache canon_dev in the mount_entry struct.  */
      అక్షరం *devname = me->me_devname;
      అక్షరం *canon_dev = canonicalize_file_name (me->me_devname);
      ఒకవేళ (canon_dev && IS_ABSOLUTE_FILE_NAME (canon_dev))
        devname = canon_dev;

      ఒకవేళ (streq (device, devname))
        {
          అక్షరం *చివరి_device = చివరి_device_for_mount (me->me_mountdir);
          eclipsed_device = చివరి_device && ! streq (చివరి_device, devname);
          size_t len = అక్షరశ్రేణి_పొడవు (me->me_mountdir);

          ఒకవేళ (! eclipsed_device
              && (! ఉత్తమ_సరిపోలిక_accessible || len < ఉత్తమ_సరిపోలిక_len))
            {
              నిర్మాణం stat device_stats;
              bool this_సరిపోలిక_accessible = false;

              ఒకవేళ (stat (me->me_mountdir, &device_stats) == 0)
                ఉత్తమ_సరిపోలిక_accessible = this_సరిపోలిక_accessible = true;

              ఒకవేళ (this_సరిపోలిక_accessible
                  || (! ఉత్తమ_సరిపోలిక_accessible && len < ఉత్తమ_సరిపోలిక_len))
                {
                  ఉత్తమ_సరిపోలిక = me;
                  ఒకవేళ (len == 1) /* Traditional root.  */
                    {
                      విడుదల (చివరి_device);
                      విడుదల (canon_dev);
                      ఆపు;
                    }
                  లేకపోతే
                    ఉత్తమ_సరిపోలిక_len = len;
                }
            }

          విడుదల (చివరి_device);
        }

      విడుదల (canon_dev);
    }

  విడుదల (resolved);

  ఒకవేళ (ఉత్తమ_సరిపోలిక)
    {
      get_dev (ఉత్తమ_సరిపోలిక->me_devname, ఉత్తమ_సరిపోలిక->me_mountdir, file, ఖాళీ,
               ఉత్తమ_సరిపోలిక->me_type, ఉత్తమ_సరిపోలిక->me_dummy,
               ఉత్తమ_సరిపోలిక->me_remote, ఖాళీ, false);
      తిరిగి true;
    }
  లేకపోతే ఒకవేళ (eclipsed_device)
    {
      error (0, 0, _("cannot access %s: over-mounted by another device"),
             quoteaf (file));
      exit_స్థితి = EXIT_FAILURE;
      తిరిగి true;
    }

  తిరిగి false;
}

/* Figure out which device file or directory POINT is mounted on
   and show its device usage.
   STATP must be the result of 'stat (POINT, STATP)'.  */
స్థిర శూన్య
get_బిందువు (అక్షరం స్థిరరాశి *బిందువు, స్థిరరాశి నిర్మాణం stat *statp)
{
  నిర్మాణం stat device_stats;
  నిర్మాణం mount_నమోదు స్థిరరాశి *ఉత్తమ_సరిపోలిక = ఖాళీ;

  /* Calculate the real absolute file name for POINT, and use that to find
     the mount point.  This avoids statting unavailable mount points,
     which can hang df.  */
  అక్షరం *resolved = canonicalize_file_name (బిందువు);
  ఒకవేళ (resolved && resolved[0] == '/')
    {
      size_t resolved_len = అక్షరశ్రేణి_పొడవు (resolved);
      size_t ఉత్తమ_సరిపోలిక_len = 0;

      కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me; me = me->me_తదుపరి)
        {
          ఒకవేళ (!streq (me->me_type, "lofs")
              && (!ఉత్తమ_సరిపోలిక || ఉత్తమ_సరిపోలిక->me_dummy || !me->me_dummy))
            {
              size_t len = అక్షరశ్రేణి_పొడవు (me->me_mountdir);
              ఒకవేళ (ఉత్తమ_సరిపోలిక_len <= len && len <= resolved_len
                  && (len == 1 /* root file system */
                      || ((len == resolved_len || resolved[len] == '/')
                          && STREQ_LEN (me->me_mountdir, resolved, len))))
                {
                  ఉత్తమ_సరిపోలిక = me;
                  ఉత్తమ_సరిపోలిక_len = len;
                }
            }
        }
    }
  విడుదల (resolved);
  ఒకవేళ (ఉత్తమ_సరిపోలిక
      && (stat (ఉత్తమ_సరిపోలిక->me_mountdir, &device_stats) != 0
          || device_stats.st_dev != statp->st_dev))
    ఉత్తమ_సరిపోలిక = ఖాళీ;

  ఒకవేళ (! ఉత్తమ_సరిపోలిక)
    కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me; me = me->me_తదుపరి)
      {
        ఒకవేళ (me->me_dev == (dev_t) -1)
          {
            ఒకవేళ (stat (me->me_mountdir, &device_stats) == 0)
              me->me_dev = device_stats.st_dev;
            లేకపోతే
              {
                /* Report only I/O errors.  Other errors might be
                   caused by shadowed mount points, which means POINT
                   can't possibly be on this file system.  */
                ఒకవేళ (errno == EIO)
                  {
                    error (0, errno, "%s", quotef (me->me_mountdir));
                    exit_స్థితి = EXIT_FAILURE;
                  }

                /* So we won't try and fail repeatedly.  */
                me->me_dev = (dev_t) -2;
              }
          }

        ఒకవేళ (statp->st_dev == me->me_dev
            && !streq (me->me_type, "lofs")
            && (!ఉత్తమ_సరిపోలిక || ఉత్తమ_సరిపోలిక->me_dummy || !me->me_dummy))
          {
            /* Skip bogus mtab entries.  */
            ఒకవేళ (stat (me->me_mountdir, &device_stats) != 0
                || device_stats.st_dev != me->me_dev)
              me->me_dev = (dev_t) -2;
            లేకపోతే
              ఉత్తమ_సరిపోలిక = me;
          }
      }

  ఒకవేళ (ఉత్తమ_సరిపోలిక)
    get_dev (ఉత్తమ_సరిపోలిక->me_devname, ఉత్తమ_సరిపోలిక->me_mountdir, బిందువు, బిందువు,
             ఉత్తమ_సరిపోలిక->me_type, ఉత్తమ_సరిపోలిక->me_dummy, ఉత్తమ_సరిపోలిక->me_remote,
             ఖాళీ, false);
  లేకపోతే
    {
      /* We couldn't find the mount entry corresponding to POINT.  Go ahead and
         print as much info as we can; methods that require the device to be
         present will fail at a later point.  */

      /* Find the actual mount point.  */
      అక్షరం *mp = కనుగొను_mount_బిందువు (బిందువు, statp);
      ఒకవేళ (mp)
        {
          get_dev (ఖాళీ, mp, బిందువు, ఖాళీ, ఖాళీ,
                   false, false, ఖాళీ, false);
          విడుదల (mp);
        }
    }
}

/* Determine what kind of node NAME is and show the device usage
   for it.  STATP is the results of 'stat' on NAME.  */

స్థిర శూన్య
get_నమోదు (అక్షరం స్థిరరాశి *name, నిర్మాణం stat స్థిరరాశి *statp)
{
  ఒకవేళ ((S_ISBLK (statp->st_బహుళకం) || S_ISCHR (statp->st_బహుళకం))
      && get_device (name))
    తిరిగి;

  get_బిందువు (name, statp);
}

/* Show all mounted file systems, except perhaps those that are of
   an unselected type or are empty.  */

స్థిర శూన్య
get_all_entries (శూన్య)
{
  filter_mount_list (show_all_fs);

  కొరకు (నిర్మాణం mount_నమోదు *me = mount_list; me; me = me->me_తదుపరి)
    get_dev (me->me_devname, me->me_mountdir, ఖాళీ, ఖాళీ, me->me_type,
             me->me_dummy, me->me_remote, ఖాళీ, true);
}

/* Add FSTYPE to the list of file system types to display.  */

స్థిర శూన్య
కూడు_fs_type (అక్షరం స్థిరరాశి *fstype)
{
  నిర్మాణం fs_type_list *fsp;

  fsp = xmalloc (పరిమాణం *fsp);
  fsp->fs_name = (అక్షరం *) fstype;
  fsp->fs_తదుపరి = fs_select_list;
  fs_select_list = fsp;
}

/* Add FSTYPE to the list of file system types to be omitted.  */

స్థిర శూన్య
కూడు_excluded_fs_type (అక్షరం స్థిరరాశి *fstype)
{
  నిర్మాణం fs_type_list *fsp;

  fsp = xmalloc (పరిమాణం *fsp);
  fsp->fs_name = (అక్షరం *) fstype;
  fsp->fs_తదుపరి = fs_exclude_list;
  fs_exclude_list = fsp;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]...\n"), కార్యక్రమం_name);
      fputs (_("\
Show information about the file system on which each FILE resides,\n\
or all file systems by అప్రమేయ.\n\
"), stdout);

      emit_mandatory_arg_note ();

      /* TRANSLATORS: The thousands and decimal separators are best
         adjusted to an appropriate default for your locale.  */
      oputs (_("\
  -a, --all\n\
         include pseudo, duplicate, inaccessible file systems\n\
"));
      oputs (_("\
  -B, --ఖండం-కొలత=కొలత_2\n\
         scale sizes by కొలత_2 before printing them; see కొలత_2 format క్రింద;\n\
         E.g., '-BM' prints sizes లోపల units యొక్క 1,048,576 బైట్లు\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes లోపల powers యొక్క 1024 (e.g., 1023M)\n\
"));
      oputs (_("\
  -H, --si\n\
         print sizes లోపల powers యొక్క 1000 (e.g., 1.1G)\n\
"));
      oputs (_("\
  -i, --inodes\n\
         list inode information instead యొక్క ఖండం usage\n\
"));
      oputs (_("\
  -k\n\
         like --ఖండం-కొలత=1K\n\
"));
      oputs (_("\
  -l, --local\n\
         హద్దు listing గమ్యం local file systems\n\
"));
      oputs (_("\
      --no-sync\n\
         చేయి not invoke sync before getting usage info (అప్రమేయ)\n\
"));
      oputs (_("\
      --output[=క్షేత్రం_LIST]\n\
         use the output format defined by క్షేత్రం_LIST,\n\
         or print all fields ఒకవేళ క్షేత్రం_LIST is omitted\n\
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
      --మొత్తం\n\
         elide all entries insignificant గమ్యం అందుబాటు space,\n\
         and produce a grand మొత్తం\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         హద్దు listing గమ్యం file systems యొక్క type TYPE\n\
"));
      oputs (_("\
  -T, --print-type\n\
         print file system type\n\
"));
      oputs (_("\
  -x, --exclude-type=TYPE\n\
         హద్దు listing గమ్యం file systems not యొక్క type TYPE\n\
"));
      oputs (_("\
  -v\n\
         (ignored)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DF");
      emit_కొలత_note ();
      fputs (_("\n\
క్షేత్రం_LIST is a comma-separated list యొక్క columns గమ్యం be included.  Valid\n\
క్షేత్రం పేర్లు are: 'source', 'fstype', 'itotal', 'iused', 'iavail', 'ipcent',\n\
'size', 'used', 'avail', 'pcent', 'file' and 'target' (see info పుట).\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  నిర్మాణం stat *stats = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* If true, use the POSIX output format.  */
  bool posix_format = false;

  అక్షరం స్థిరరాశి *సందేశం_mut_excl = _("options %s and %s are mutually exclusive");

  అయితే (true)
    {
      పూర్ణాంకం oi = -1;
      పూర్ణాంకం c = getopt_long (argc, argv, "aB:iF:hHklmPTt:vx:", long_options,
                           &oi);
      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 'a':
          show_all_fs = true;
          ఆపు;
        స్థితి 'B':
          {
            గణన strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ఖండం_కొలత);
            ఒకవేళ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ఆపు;
        స్థితి 'i':
          ఒకవేళ (శీర్షిక_బహుళకం == OUTPUT_బహుళకం)
            {
              error (0, 0, సందేశం_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          శీర్షిక_బహుళకం = INODES_బహుళకం;
          ఆపు;
        స్థితి 'h':
          human_output_opts = human_autoscale | human_SI | human_పునాది_1024;
          output_ఖండం_కొలత = 1;
          ఆపు;
        స్థితి 'H':
          human_output_opts = human_autoscale | human_SI;
          output_ఖండం_కొలత = 1;
          ఆపు;
        స్థితి 'k':
          human_output_opts = 0;
          output_ఖండం_కొలత = 1024;
          ఆపు;
        స్థితి 'l':
          show_local_fs = true;
          ఆపు;
        స్థితి 'm': /* obsolescent, exists for BSD compatibility */
          human_output_opts = 0;
          output_ఖండం_కొలత = 1024 * 1024;
          ఆపు;
        స్థితి 'T':
          ఒకవేళ (శీర్షిక_బహుళకం == OUTPUT_బహుళకం)
            {
              error (0, 0, సందేశం_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          print_type = true;
          ఆపు;
        స్థితి 'P':
          ఒకవేళ (శీర్షిక_బహుళకం == OUTPUT_బహుళకం)
            {
              error (0, 0, సందేశం_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          posix_format = true;
          ఆపు;
        స్థితి SYNC_OPTION:
          require_sync = true;
          ఆపు;
        స్థితి NO_SYNC_OPTION:
          require_sync = false;
          ఆపు;

        స్థితి 'F':
          /* Accept -F as a synonym for -t for compatibility with Solaris.  */
        స్థితి 't':
          కూడు_fs_type (optarg);
          ఆపు;

        స్థితి 'v':		/* For SysV compatibility.  */
          /* ignore */
          ఆపు;
        స్థితి 'x':
          కూడు_excluded_fs_type (optarg);
          ఆపు;

        స్థితి OUTPUT_OPTION:
          ఒకవేళ (శీర్షిక_బహుళకం == INODES_బహుళకం)
            {
              error (0, 0, సందేశం_mut_excl, "-i", "--output");
              usage (EXIT_FAILURE);
            }
          ఒకవేళ (posix_format && శీర్షిక_బహుళకం == DEFAULT_బహుళకం)
            {
              error (0, 0, సందేశం_mut_excl, "-P", "--output");
              usage (EXIT_FAILURE);
            }
          ఒకవేళ (print_type)
            {
              error (0, 0, సందేశం_mut_excl, "-T", "--output");
              usage (EXIT_FAILURE);
            }
          శీర్షిక_బహుళకం = OUTPUT_బహుళకం;
          ఒకవేళ (optarg)
            decode_output_arg (optarg);
          ఆపు;

        స్థితి మొత్తం_OPTION:
          print_grand_మొత్తం = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (human_output_opts == -1)
    {
      ఒకవేళ (posix_format)
        {
          human_output_opts = 0;
          output_ఖండం_కొలత = (getenv ("POSIXLY_CORRECT") ? 512 : 1024);
        }
      లేకపోతే
        human_options (getenv ("DF_BLOCK_SIZE"),
                       &human_output_opts, &output_ఖండం_కొలత);
    }

  ఒకవేళ (శీర్షిక_బహుళకం == INODES_బహుళకం || శీర్షిక_బహుళకం == OUTPUT_బహుళకం)
    ;
  లేకపోతే ఒకవేళ (human_output_opts & human_autoscale)
    శీర్షిక_బహుళకం = HUMAN_బహుళకం;
  లేకపోతే ఒకవేళ (posix_format)
    శీర్షిక_బహుళకం = POSIX_బహుళకం;

  /* Fail if the same file system type was both selected and excluded.  */
  {
    bool సరిపోలిక = false;
    కొరకు (నిర్మాణం fs_type_list *fs_incl = fs_select_list; fs_incl;
         fs_incl = fs_incl->fs_తదుపరి)
      {
        కొరకు (నిర్మాణం fs_type_list *fs_excl = fs_exclude_list; fs_excl;
             fs_excl = fs_excl->fs_తదుపరి)
          {
            ఒకవేళ (streq (fs_incl->fs_name, fs_excl->fs_name))
              {
                error (0, 0,
                       _("file system type %s both selected and excluded"),
                       quote (fs_incl->fs_name));
                సరిపోలిక = true;
                ఆపు;
              }
          }
      }
    ఒకవేళ (సరిపోలిక)
      తిరిగి EXIT_FAILURE;
  }

  ఒకవేళ (optind < argc)
    {
      /* stat each of the given entries to make sure any corresponding
         partition is automounted.  This must be done before reading the
         file system table.  */
      stats = xnmalloc (argc - optind, పరిమాణం *stats);
      కొరకు (పూర్ణాంకం i = optind; i < argc; ++i)
        {
          పూర్ణాంకం err = automount_stat_err (argv[i], &stats[i - optind]);
          ఒకవేళ (err != 0)
            {
              error (0, err, "%s", quotef (argv[i]));
              exit_స్థితి = EXIT_FAILURE;
              argv[i] = ఖాళీ;
            }
        }
    }

  mount_list =
    read_file_system_list ((fs_select_list != ఖాళీ
                            || fs_exclude_list != ఖాళీ
                            || print_type
                            || క్షేత్రం_దత్తాంశం_2[FSTYPE_క్షేత్రం].వాడిన
                            || show_local_fs));

  ఒకవేళ (mount_list == ఖాళీ)
    {
      /* Couldn't read the table of mounted file systems.
         Fail if df was invoked with no file name arguments,
         or when either of -a, -l, -t or -x is used with file name
         arguments.  Otherwise, merely give a warning and proceed.  */
      పూర్ణాంకం స్థితి_2 = 0;
      ఒకవేళ ( ! (optind < argc)
           || (show_all_fs
               || show_local_fs
               || fs_select_list != ఖాళీ
               || fs_exclude_list != ఖాళీ))
        {
          స్థితి_2 = EXIT_FAILURE;
        }
      అక్షరం స్థిరరాశి *warning = (స్థితి_2 == 0 ? _("Warning: ") : "");
      error (స్థితి_2, errno, "%s%s", warning,
             _("cannot read table of mounted file systems"));
    }

  ఒకవేళ (require_sync)
    sync ();

  get_క్షేత్రం_list ();
  get_శీర్షిక ();

  ఒకవేళ (stats)
    {
      /* Display explicitly requested empty file systems.  */
      show_listed_fs = true;

      కొరకు (పూర్ణాంకం i = optind; i < argc; ++i)
        ఒకవేళ (argv[i])
          get_నమోదు (argv[i], &stats[i - optind]);
    }
  లేకపోతే
    get_all_entries ();

  ఒకవేళ (file_systems_processed)
    {
      ఒకవేళ (print_grand_మొత్తం)
        get_dev ("total",
                 (క్షేత్రం_దత్తాంశం_2[ఆధారం_క్షేత్రం].వాడిన ? "-" : "total"),
                 ఖాళీ, ఖాళీ, ఖాళీ, false, false, &grand_fsu, false);

      print_పట్టిక ();
    }
  లేకపోతే
    {
      /* Print the "no FS processed" diagnostic only if there was no preceding
         diagnostic, e.g., if all have been excluded.  */
      ఒకవేళ (exit_స్థితి == EXIT_SUCCESS)
        error (EXIT_FAILURE, 0, _("no file systems processed"));
    }

  main_exit (exit_స్థితి);
}
