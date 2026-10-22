/* dd -- convert a file while copying it.
   Copyright (C) 1985-2026 Free Software Foundation, Inc.

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

/* Written by Paul Rubin, David MacKenzie, and Stuart Kemp. */

#شامل <config.h>

#شامل <sys/types.h>
#شامل <signal.h>

#شامل "system.h"
#شامل "alignalloc.h"
#شامل "close-stream.h"
#شامل "fd-reopen.h"
#شامل "gethrxtime.h"
#شامل "human.h"
#شامل "ioblksize.h"
#شامل "long-options.h"
#شامل "quote.h"
#شامل "xstrtol.h"
#شامل "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "dd"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#اگر_غیر_تعریف_شدہ SA_RESETHAND
# define SA_RESETHAND 0
#اگر_ختم

#اگر_غیر_تعریف_شدہ SIGINFO
# define SIGINFO SIGUSR1
#اگر_ختم

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#اگر_غیر_تعریف_شدہ O_CIO
# define O_CIO 0
#اگر_ختم

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#تعریف_منسوخ O_NOCACHE

#تعریف_کرو output_char(c)				\
  کرو						\
    {						\
      obuf[oc++] = (c);				\
      اگر (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  جب (0)

/* Default input and output blocksize. */
#تعریف_کرو DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
شمار
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_بلاک = 010,
    C_UNBLOCK = 020,
    C_LCASE = 040,
    C_UCASE = 0100,
    C_SWAB = 0200,
    C_NOERROR = 0400,
    C_NOTRUNC = 01000,
    C_SYNC = 02000,

    /* Use separate input and output buffers, and combine partial
       input blocks. */
    C_TWOBUFS = 04000,

    C_NOCREAT = 010000,
    C_EXCL = 020000,
    C_FDATASYNC = 040000,
    C_FSYNC = 0100000,

    C_SPARSE = 0200000
  };

/* Status levels.  */
شمار
  {
    حالت_NONE = 1,
    حالت_NOXFER = 2,
    حالت_DEFAULT = 3,
    حالت_پیش_رفت = 4
  };

/* The name of the input file, or NULL for the standard input. */
ساکن حرف ثابت *داخلہ_file = تہی;

/* The name of the output file, or NULL for the standard output. */
ساکن حرف ثابت *output_file = تہی;

/* The page size on this host.  */
ساکن idx_t صفحہ_سائز;

/* The number of bytes in which atomic reads are done. */
ساکن idx_t داخلہ_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ساکن idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ساکن idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ساکن intmax_t skip_اندراجات = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ساکن idx_t skip_بائٹیں = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ساکن intmax_t seek_اندراجات = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ساکن intmax_t seek_بائٹیں = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ساکن bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ساکن intmax_t max_اندراجات = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ساکن idx_t max_بائٹیں = 0;

/* Bit vector of conversions to apply. */
ساکن صحیح conversions_نقاب = 0;

/* Open flags for the input and output files.  */
ساکن صحیح داخلہ_flags = 0;
ساکن صحیح output_flags = 0;

/* Status flags for what is printed to stderr.  */
ساکن صحیح حالت_level = حالت_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ساکن bool translation_needed = false;

/* Number of partial blocks written. */
ساکن intmax_t w_partial = 0;

/* Number of full blocks written. */
ساکن intmax_t w_بھرا = 0;

/* Number of partial blocks read. */
ساکن intmax_t r_partial = 0;

/* Number of full blocks read. */
ساکن intmax_t r_بھرا = 0;

/* Number of bytes written.  */
ساکن intmax_t w_بائٹیں = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ساکن intmax_t reported_w_بائٹیں = -1;

/* Time that dd started.  */
ساکن xtime_t آغاز_وقت;

/* Next time to report periodic progress.  */
ساکن xtime_t اگلا_وقت;

/* If positive, the number of bytes output in the current progress line.  */
ساکن صحیح پیش_رفت_len;

/* True if input is seekable.  */
ساکن bool داخلہ_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ساکن صحیح داخلہ_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ساکن off_t داخلہ_انحراف;

/* True if a partial read should be diagnosed.  */
ساکن bool warn_partial_read;

/* Records truncated by conv=block. */
ساکن intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
ساکن حرف newline_character = '\n';
ساکن حرف space_character = ' ';

/* I/O buffers.  */
ساکن حرف *ibuf;
ساکن حرف *obuf;

/* Current index into 'obuf'. */
ساکن idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
ساکن idx_t ستون = 0;

/* The set of signals that are caught.  */
ساکن sigset_t caught_اشارات;

/* If nonzero, the value of the pending fatal signal.  */
ساکن sig_atomic_t متلون مداخلت_signal;

/* A count of the number of pending info signals that have been received.  */
ساکن sig_atomic_t متلون info_signal_گنتی;

/* Whether to discard cache for input or output.  */
ساکن bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ساکن bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ساکن ssize_t (*iread_fnc) (صحیح fd, حرف *buf, idx_t سائز);

/* A longest symbol in the struct symbol_values tables below.  */
#تعریف_کرو LONGEST_علامت "count_bytes"

/* A symbol and the corresponding integer value.  */
ساخت علامت_قدر
{
  حرف علامت[حجم LONGEST_علامت];
  صحیح قدر;
};

/* Conversion symbols, for conv="...".  */
ساکن ساخت علامت_قدر ثابت conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_بلاک | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_بلاک | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_بلاک | C_TWOBUFS},	/* Variable to fixed length records. */
  {"unblock", C_UNBLOCK | C_TWOBUFS},	/* Fixed to variable length records. */
  {"lcase", C_LCASE | C_TWOBUFS},	/* Translate upper to lower case. */
  {"ucase", C_UCASE | C_TWOBUFS},	/* Translate lower to upper case. */
  {"sparse", C_SPARSE},		/* Try to sparsely write output. */
  {"swab", C_SWAB | C_TWOBUFS},	/* Swap bytes of input. */
  {"noerror", C_NOERROR},	/* Ignore i/o errors. */
  {"nocreat", C_NOCREAT},	/* Do not create output file.  */
  {"excl", C_EXCL},		/* Fail if the output file already exists.  */
  {"notrunc", C_NOTRUNC},	/* Do not truncate output file. */
  {"sync", C_SYNC},		/* Pad input records to ibs with NULs. */
  {"fdatasync", C_FDATASYNC},	/* Synchronize output data before finishing.  */
  {"fsync", C_FSYNC},		/* Also synchronize output metadata.  */
  {"", 0}
};

#تعریف_کرو FFS_نقاب(x) ((x) ^ ((x) & ((x) - 1)))
شمار
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ثنائی
          | O_CIO
          | O_براہ_راست
          | O_DIRECTORY
          | O_DSYNC
          | O_EXCL
          | O_NOATIME
          | O_NOCTTY
          | O_NOFOLLOW
          | O_NOLINKS
          | O_NONBLOCK
          | O_SYNC
          | O_متن
          ),

    /* Use its lowest bits for private flags.  */
    O_FULLBLOCK = FFS_نقاب (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_نقاب (v2),
    v3 = v2 ^ O_NOCACHE,

    O_گنتی_بائٹیں = FFS_نقاب (v3),
    v4 = v3 ^ O_گنتی_بائٹیں,

    O_SKIP_بائٹیں = FFS_نقاب (v4),
    v5 = v4 ^ O_SKIP_بائٹیں,

    O_SEEK_بائٹیں = FFS_نقاب (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_گنتی_بائٹیں != 0);
static_assert (O_SKIP_بائٹیں != 0);
static_assert (O_SEEK_بائٹیں != 0);

#تعریف_کرو مضرب_بٹ_سیٹ(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! مضرب_بٹ_سیٹ (O_FULLBLOCK));
static_assert ( ! مضرب_بٹ_سیٹ (O_NOCACHE));
static_assert ( ! مضرب_بٹ_سیٹ (O_گنتی_بائٹیں));
static_assert ( ! مضرب_بٹ_سیٹ (O_SKIP_بائٹیں));
static_assert ( ! مضرب_بٹ_سیٹ (O_SEEK_بائٹیں));

/* Flags, for iflag="..." and oflag="...".  */
ساکن ساخت علامت_قدر ثابت flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ثنائی},
  {"cio",	  O_CIO},
  {"direct",	  O_براہ_راست},
  {"directory",   O_DIRECTORY},
  {"dsync",	  O_DSYNC},
  {"noatime",	  O_NOATIME},
  {"nocache",	  O_NOCACHE},   /* Discard cache.  */
  {"noctty",	  O_NOCTTY},
  {"nofollow",	  HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0},
  {"nolinks",	  O_NOLINKS},
  {"nonblock",	  O_NONBLOCK},
  {"sync",	  O_SYNC},
  {"text",	  O_متن},
  {"fullblock",   O_FULLBLOCK}, /* Accumulate full blocks from input.  */
  {"count_bytes", O_گنتی_بائٹیں},
  {"skip_bytes",  O_SKIP_بائٹیں},
  {"seek_bytes",  O_SEEK_بائٹیں},
  {"",		0}
};

/* Status, for status="...".  */
ساکن ساخت علامت_قدر ثابت statuses[] =
{
  {"none",	حالت_NONE},
  {"noxfer",	حالت_NOXFER},
  {"progress",	حالت_پیش_رفت},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ساکن غیرعلامتی حرف trans_جدول[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ساکن حرف ثابت ascii_مقصد_ebcdic[] =
{
  '\000', '\001', '\002', '\003', '\067', '\055', '\056', '\057',
  '\026', '\005', '\045', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\074', '\075', '\062', '\046',
  '\030', '\031', '\077', '\047', '\034', '\035', '\036', '\037',
  '\100', '\132', '\177', '\173', '\133', '\154', '\120', '\175',
  '\115', '\135', '\134', '\116', '\153', '\140', '\113', '\141',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\172', '\136', '\114', '\176', '\156', '\157',
  '\174', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\321', '\322', '\323', '\324', '\325', '\326',
  '\327', '\330', '\331', '\342', '\343', '\344', '\345', '\346',
  '\347', '\350', '\351', '\255', '\340', '\275', '\232', '\155',
  '\171', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\221', '\222', '\223', '\224', '\225', '\226',
  '\227', '\230', '\231', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\251', '\300', '\117', '\320', '\137', '\007',
  '\040', '\041', '\042', '\043', '\044', '\025', '\006', '\027',
  '\050', '\051', '\052', '\053', '\054', '\011', '\012', '\033',
  '\060', '\061', '\032', '\063', '\064', '\065', '\066', '\010',
  '\070', '\071', '\072', '\073', '\004', '\024', '\076', '\341',
  '\101', '\102', '\103', '\104', '\105', '\106', '\107', '\110',
  '\111', '\121', '\122', '\123', '\124', '\125', '\126', '\127',
  '\130', '\131', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\160', '\161', '\162', '\163', '\164', '\165',
  '\166', '\167', '\170', '\200', '\212', '\213', '\214', '\215',
  '\216', '\217', '\220', '\152', '\233', '\234', '\235', '\236',
  '\237', '\240', '\252', '\253', '\254', '\112', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\241', '\276', '\277',
  '\312', '\313', '\314', '\315', '\316', '\317', '\332', '\333',
  '\334', '\335', '\336', '\337', '\352', '\353', '\354', '\355',
  '\356', '\357', '\372', '\373', '\374', '\375', '\376', '\377'
};

ساکن حرف ثابت ascii_مقصد_ibm[] =
{
  '\000', '\001', '\002', '\003', '\067', '\055', '\056', '\057',
  '\026', '\005', '\045', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\074', '\075', '\062', '\046',
  '\030', '\031', '\077', '\047', '\034', '\035', '\036', '\037',
  '\100', '\132', '\177', '\173', '\133', '\154', '\120', '\175',
  '\115', '\135', '\134', '\116', '\153', '\140', '\113', '\141',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\172', '\136', '\114', '\176', '\156', '\157',
  '\174', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\321', '\322', '\323', '\324', '\325', '\326',
  '\327', '\330', '\331', '\342', '\343', '\344', '\345', '\346',
  '\347', '\350', '\351', '\255', '\340', '\275', '\137', '\155',
  '\171', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\221', '\222', '\223', '\224', '\225', '\226',
  '\227', '\230', '\231', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\251', '\300', '\117', '\320', '\241', '\007',
  '\040', '\041', '\042', '\043', '\044', '\025', '\006', '\027',
  '\050', '\051', '\052', '\053', '\054', '\011', '\012', '\033',
  '\060', '\061', '\032', '\063', '\064', '\065', '\066', '\010',
  '\070', '\071', '\072', '\073', '\004', '\024', '\076', '\341',
  '\101', '\102', '\103', '\104', '\105', '\106', '\107', '\110',
  '\111', '\121', '\122', '\123', '\124', '\125', '\126', '\127',
  '\130', '\131', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\160', '\161', '\162', '\163', '\164', '\165',
  '\166', '\167', '\170', '\200', '\212', '\213', '\214', '\215',
  '\216', '\217', '\220', '\232', '\233', '\234', '\235', '\236',
  '\237', '\240', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
  '\312', '\313', '\314', '\315', '\316', '\317', '\332', '\333',
  '\334', '\335', '\336', '\337', '\352', '\353', '\354', '\355',
  '\356', '\357', '\372', '\373', '\374', '\375', '\376', '\377'
};

ساکن حرف ثابت ebcdic_مقصد_ascii[] =
{
  '\000', '\001', '\002', '\003', '\234', '\011', '\206', '\177',
  '\227', '\215', '\216', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\235', '\205', '\010', '\207',
  '\030', '\031', '\222', '\217', '\034', '\035', '\036', '\037',
  '\200', '\201', '\202', '\203', '\204', '\012', '\027', '\033',
  '\210', '\211', '\212', '\213', '\214', '\005', '\006', '\007',
  '\220', '\221', '\026', '\223', '\224', '\225', '\226', '\004',
  '\230', '\231', '\232', '\233', '\024', '\025', '\236', '\032',
  '\040', '\240', '\241', '\242', '\243', '\244', '\245', '\246',
  '\247', '\250', '\325', '\056', '\074', '\050', '\053', '\174',
  '\046', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\041', '\044', '\052', '\051', '\073', '\176',
  '\055', '\057', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\313', '\054', '\045', '\137', '\076', '\077',
  '\272', '\273', '\274', '\275', '\276', '\277', '\300', '\301',
  '\302', '\140', '\072', '\043', '\100', '\047', '\075', '\042',
  '\303', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\304', '\305', '\306', '\307', '\310', '\311',
  '\312', '\152', '\153', '\154', '\155', '\156', '\157', '\160',
  '\161', '\162', '\136', '\314', '\315', '\316', '\317', '\320',
  '\321', '\345', '\163', '\164', '\165', '\166', '\167', '\170',
  '\171', '\172', '\322', '\323', '\324', '\133', '\326', '\327',
  '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
  '\340', '\341', '\342', '\343', '\344', '\135', '\346', '\347',
  '\173', '\101', '\102', '\103', '\104', '\105', '\106', '\107',
  '\110', '\111', '\350', '\351', '\352', '\353', '\354', '\355',
  '\175', '\112', '\113', '\114', '\115', '\116', '\117', '\120',
  '\121', '\122', '\356', '\357', '\360', '\361', '\362', '\363',
  '\134', '\237', '\123', '\124', '\125', '\126', '\127', '\130',
  '\131', '\132', '\364', '\365', '\366', '\367', '\370', '\371',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\372', '\373', '\374', '\375', '\376', '\377'
};

/* True if we need to close the standard output *stream*.  */
ساکن bool close_stdout_required = true;

/* The only reason to close the standard output *stream* is if
   parse_long_options fails (as it does for --help or --version).
   In any other case, dd uses only the STDOUT_FILENO file descriptor,
   and the "cleanup" function calls "close (STDOUT_FILENO)".
   Closing the file descriptor and then letting the usual atexit-run
   close_stdout function call "fclose (stdout)" would result in a
   harmless failure of the close syscall (with errno EBADF).
   This function serves solely to avoid the unnecessary close_stdout
   call, once parse_long_options has succeeded.
   Meanwhile, we guarantee that the standard error stream is flushed,
   by inlining the last half of close_stdout as needed.  */
ساکن خالی
maybe_close_stdout (خالی)
{
  اگر (close_stdout_required)
    close_stdout ();
  ورنہ اگر (close_دھارا (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ساکن خالی
diagnose (صحیح errnum, حرف ثابت *fmt, ...)
{
  اگر (0 < پیش_رفت_len)
    {
      fputc ('\n', stderr);
      پیش_رفت_len = 0;
    }

  va_list ap;
  va_آغاز (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_انتہا (ap);
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [عملیہ]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Copy a file, converting and formatting according مقصد the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=بائٹیں_2        اقرأ and اكتب اوپر مقصد بائٹیں_2 بائٹیں at a وقت (طےشدہ: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=بائٹیں_2       convert بائٹیں_2 بائٹیں at a وقت\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as فی the comma separated علامت list\n\
"));
      oputs (_("\
  گنتی=N         copy only N داخلہ بلاکیں\n\
"));
      oputs (_("\
  ibs=بائٹیں_2       اقرأ اوپر مقصد بائٹیں_2 بائٹیں at a وقت (طےشدہ: 512)\n\
"));
      oputs (_("\
  اگر=FILE         اقرأ from FILE instead کا standard داخلہ\n\
"));
      oputs (_("\
  iflag=FLAGS     اقرأ as فی the comma separated علامت list\n\
"));
      oputs (_("\
  obs=بائٹیں_2       اكتب بائٹیں_2 بائٹیں at a وقت (طےشدہ: 512)\n\
"));
      oputs (_("\
  کا=FILE         اكتب مقصد FILE instead کا standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     اكتب as فی the comma separated علامت list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output بلاکیں\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized داخلہ بلاکیں\n\
"));
      oputs (_("\
  حالت_2=LEVEL    The LEVEL کا information مقصد print مقصد standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and بائٹیں_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on برائے T, P, E, Z, Y, R, Q.\n\
ثنائی_2 prefixes can be مستعمل, too: KiB=K, MiB=M, and so on.\n\
If N سرے اندر 'B', it counts بائٹیں not بلاکیں.\n\
\n\
Each CONV علامت may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC مقصد ASCII\n\
  ebcdic    from ASCII مقصد EBCDIC\n\
  ibm       from ASCII مقصد alternate EBCDIC\n\
  بلاک     pad newline-terminated اندراجات with spaces مقصد cbs-سائز\n\
  unblock   replace trailing spaces اندر cbs-سائز اندراجات with newline\n\
  lcase     change بالائی حالت مقصد زیریں حالت\n\
  ucase     change زیریں حالت مقصد بالائی حالت\n\
  sparse    try مقصد seek rather than اكتب all-NUL output بلاکیں\n\
  swab      تبادلہ every pair کا داخلہ بائٹیں\n\
  sync      pad every داخلہ بلاک with NULs مقصد ibs-سائز; when مستعمل\n\
            with بلاک or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail اگر the output file already exists\n\
  nocreat   کرو not create the output file\n\
  notrunc   کرو not truncate the output file\n\
  noerror   جاری after اقرأ errors\n\
  fdatasync  physically اكتب output file ڈیٹا before finishing\n\
  fsync     likewise, but also اكتب metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG علامت may be:\n\
\n\
  append    append منوال_2 (makes sense only برائے output; conv=notrunc suggested)\n\
"), stdout);
      اگر (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      اگر (O_براہ_راست)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      اگر (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      اگر (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      اگر (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      اگر (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#اگر HAVE_POSIX_FADVISE
      اگر (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#اگر_ختم
      اگر (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      اگر (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      اگر (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      اگر (O_ثنائی)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      اگر (O_متن)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        لکھو (_("\
\n\
Sending a %s اشارہ مقصد a running 'dd' process makes it\n\
print I/O statistics مقصد standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Common options to use when displaying sizes and rates.  */

شمار { human_opts = (human_autoscale | human_باری_مقصد_قریب_ترین
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ساکن خالی
alloc_ibuf (خالی)
{
  اگر (ibuf)
    واپس;

  bool extra_byte_for_swab = !!(conversions_نقاب & C_SWAB);
  ibuf = alignalloc (صفحہ_سائز, داخلہ_blocksize + extra_byte_for_swab);
  اگر (!ibuf)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             داخلہ_blocksize,
             human_readable (داخلہ_blocksize, hbuf,
                             human_opts | human_بنیاد_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ساکن خالی
alloc_obuf (خالی)
{
  اگر (obuf)
    واپس;

  اگر (conversions_نقاب & C_TWOBUFS)
    {
      obuf = alignalloc (صفحہ_سائز, output_blocksize);
      اگر (!obuf)
        {
          حرف hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_بنیاد_1024, 1, 1));
        }
    }
  ورنہ
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

ساکن خالی
translate_charset (حرف ثابت *new_trans)
{
  برائے (صحیح i = 0; i < 256; i++)
    trans_جدول[i] = new_trans[trans_جدول[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

ساکن اندرونی bool
مضرب_بٹ_سیٹ_2 (صحیح i)
{
  واپس مضرب_بٹ_سیٹ (i);
}

ساکن bool
abbreviation_lacks_سابقہ (حرف ثابت *پیغام_2)
{
  واپس پیغام_2[لڑی_لمبائی (پیغام_2) - 2] == ' ';
}

/* Print transfer statistics.  */

ساکن خالی
print_xfer_stats (xtime_t پیش_رفت_وقت)
{
  xtime_t now = پیش_رفت_وقت ? پیش_رفت_وقت : gethrxtime ();
  ساکن حرف ثابت slash_s[] = "/s";
  حرف hbuf[3][LONGEST_HUMAN_READABLE + حجم slash_s];
  دوہرا delta_s;
  حرف ثابت *بائٹیں_فی_دوسرا;
  حرف ثابت *si = human_readable (w_بائٹیں, hbuf[0], human_opts, 1, 1);
  حرف ثابت *iec = human_readable (w_بائٹیں, hbuf[1],
                                    human_opts | human_بنیاد_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  حرف *bpsbuf = hbuf[2];
  صحیح bpsbufsize = حجم hbuf[2];
  اگر (آغاز_وقت < now)
    {
      دوہرا XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - آغاز_وقت;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      بائٹیں_فی_دوسرا = human_readable (w_بائٹیں, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (بائٹیں_فی_دوسرا - bpsbuf + bpsbuf, slash_s);
    }
  ورنہ
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      بائٹیں_فی_دوسرا = bpsbuf;
    }

  اگر (پیش_رفت_وقت)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  حرف delta_s_buf[24];
  snprintf (delta_s_buf, حجم delta_s_buf,
            پیش_رفت_وقت ? "%.0f s" : "%g s", delta_s);

  صحیح stats_len
    = (abbreviation_lacks_سابقہ (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_بائٹیں)),
                  w_بائٹیں, delta_s_buf, بائٹیں_فی_دوسرا)
       : abbreviation_lacks_سابقہ (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_بائٹیں, si, delta_s_buf, بائٹیں_فی_دوسرا)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_بائٹیں, si, iec, delta_s_buf, بائٹیں_فی_دوسرا));

  اگر (پیش_رفت_وقت)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      اگر (0 <= stats_len && stats_len < پیش_رفت_len)
        fprintf (stderr, "%*s", پیش_رفت_len - stats_len, "");
      پیش_رفت_len = stats_len;
    }
  ورنہ
    fputc ('\n', stderr);

  reported_w_بائٹیں = w_بائٹیں;
}

ساکن خالی
print_stats (خالی)
{
  اگر (حالت_level == حالت_NONE)
    واپس;

  اگر (0 < پیش_رفت_len)
    {
      fputc ('\n', stderr);
      پیش_رفت_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_بھرا, r_partial, w_بھرا, w_partial);

  اگر (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  اگر (حالت_level == حالت_NOXFER)
    واپس;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساکن خالی
مداخلت_handler (صحیح sig)
{
  اگر (! SA_RESETHAND)
    اشارہ (sig, SIG_DFL);
  مداخلت_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ساکن خالی
siginfo_handler (MAYBE_غیر_مستعمل صحیح sig)
{
  info_signal_گنتی++;
}

/* Install the signal handlers.  */

ساکن خالی
install_signal_handlers (خالی)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  ساخت sigaction act;
  sigemptyset (&caught_اشارات);
  اگر (catch_siginfo)
    sigaddset (&caught_اشارات, SIGINFO);
  sigaction (SIGINT, تہی, &act);
  اگر (act.sa_handler != SIG_IGN)
    sigaddset (&caught_اشارات, SIGINT);
  act.sa_mask = caught_اشارات;

  اگر (sigismember (&caught_اشارات, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, تہی);
    }

  اگر (sigismember (&caught_اشارات, SIGINT))
    {
      act.sa_handler = مداخلت_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, تہی);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
ساکن صحیح
iclose (صحیح fd)
{
  اگر (close (fd) != 0)
    کرو
      اگر (errno != EINTR)
        واپس -1;
    جب (close (fd) != 0 && errno != EBADF);

  واپس 0;
}

ساکن صحیح synchronize_output (خالی);

ساکن خالی
cleanup (خالی)
{
  اگر (!مداخلت_signal)
    {
      صحیح sync_حالت = synchronize_output ();
      اگر (sync_حالت)
        باہر_نکلو (sync_حالت);
    }

  اگر (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (داخلہ_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  اگر (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

ساکن خالی
process_اشارات (خالی)
{
  جب (مداخلت_signal || info_signal_گنتی)
    {
      صحیح مداخلت;
      صحیح infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_اشارات, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      مداخلت = مداخلت_signal;
      infos = info_signal_گنتی;

      اگر (infos)
        info_signal_گنتی = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, تہی);

      اگر (مداخلت)
        cleanup ();
      print_stats ();
      اگر (مداخلت)
        اٹھاؤ (مداخلت);
    }
}

ساکن خالی
اختتام_اوپر (خالی)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_اشارات ();
  cleanup ();
  print_stats ();
}

ساکن خالی
quit (صحیح رمز)
{
  اختتام_اوپر ();
  باہر_نکلو (رمز);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ساکن off_t
cache_باری (صحیح fd, off_t len)
{
  ساکن off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  اگر (len)
    {
      intmax_t c_pending;
      اگر (ckd_جوڑو (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      اگر (c_pending > *pending)
        len = c_pending - *pending;
      ورنہ
        len = 0;
    }
  ورنہ
    len = *pending;

  واپس len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

ساکن bool
invalidate_cache (صحیح fd, off_t len)
{
  صحیح adv_ret = -1;
  off_t انحراف;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_باری (fd, len);
  اگر (len && !clen)
    واپس true; /* Don't advise this time.  */
  ورنہ اگر (! len && ! clen && ! nocache_eof)
    واپس true;
  off_t pending = len ? cache_باری (fd, 0) : 0;

  اگر (fd == STDIN_FILENO)
    {
      اگر (داخلہ_seekable)
        انحراف = داخلہ_انحراف;
      ورنہ
        {
          انحراف = -1;
          errno = ESPIPE;
        }
    }
  ورنہ
    {
      ساکن off_t output_انحراف = -2;

      اگر (output_انحراف != -1)
        {
          اگر (output_انحراف < 0)
            output_انحراف = lseek (fd, 0, SEEK_CUR);
          ورنہ اگر (len)
            output_انحراف += clen + pending;
        }

      انحراف = output_انحراف;
    }

  اگر (0 <= انحراف)
   {
     اگر (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#اگر HAVE_POSIX_FADVISE
     انحراف = انحراف - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     اگر (clen == 0)
       انحراف -= انحراف % صفحہ_سائز;
     adv_ret = posix_fadvise (fd, انحراف, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#ورنہ
     errno = ENOTSUP;
#اگر_ختم
   }

  واپس adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ساکن ssize_t
iread (صحیح fd, حرف *buf, idx_t سائز)
{
  ssize_t nread;
  ساکن ssize_t پچھلا_nread;

  کرو
    {
      process_اشارات ();
      nread = اقرأ (fd, buf, سائز);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      اگر (nread == -1 && errno == EINVAL
          && 0 < پچھلا_nread && پچھلا_nread < سائز
          && (داخلہ_flags & O_براہ_راست))
        {
          errno = 0;
          nread = 0;
        }
    }
  جب (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  اگر (0 < nread && nread < سائز)
    process_اشارات ();

  اگر (0 < nread && warn_partial_read)
    {
      اگر (0 < پچھلا_nread && پچھلا_nread < سائز)
        {
          idx_t پچھلا = پچھلا_nread;
          اگر (حالت_level != حالت_NONE)
            diagnose (0, ngettext (("warning: partial read (%td byte); "
                                    "suggest iflag=fullblock"),
                                   ("warning: partial read (%td bytes); "
                                    "suggest iflag=fullblock"),
                                   select_plural (پچھلا)),
                      پچھلا);
          warn_partial_read = false;
        }
    }

  پچھلا_nread = nread;
  واپس nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
ساکن ssize_t
iread_fullblock (صحیح fd, حرف *buf, idx_t سائز)
{
  ssize_t nread = 0;

  جب (0 < سائز)
    {
      ssize_t ncurr = iread (fd, buf, سائز);
      اگر (ncurr < 0)
        واپس ncurr;
      اگر (ncurr == 0)
        توڑو;
      nread += ncurr;
      buf   += ncurr;
      سائز  -= ncurr;
    }

  واپس nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ساکن idx_t
iwrite (صحیح fd, حرف ثابت *buf, idx_t سائز)
{
  idx_t کل_written = 0;

  اگر ((output_flags & O_براہ_راست) && سائز < output_blocksize)
    {
      صحیح old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      اگر (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_براہ_راست) != 0
          && حالت_level != حالت_NONE)
        diagnose (errno, _("failed to turn off O_DIRECT: %s"),
                  quotef (output_file));

      /* Since we have just turned off O_DIRECT for the final write,
         we try to preserve some of its semantics.  */

      /* Call invalidate_cache to setup the appropriate offsets
         for subsequent calls.  */
      o_nocache_eof = true;
      invalidate_cache (STDOUT_FILENO, 0);

      /* Attempt to ensure that the final block is committed
         to stable storage as quickly as possible.  */
      conversions_نقاب |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  جب (کل_written < سائز)
    {
      ssize_t nwritten = 0;
      process_اشارات ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      اگر ((conversions_نقاب & C_SPARSE) && is_nul (buf, سائز))
        {
          اگر (lseek (fd, سائز, SEEK_CUR) < 0)
            {
              conversions_نقاب &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          ورنہ
            {
              final_op_was_seek = true;
              nwritten = سائز;
            }
        }

      اگر (!nwritten)
        nwritten = اكتب (fd, buf + کل_written, سائز - کل_written);

      اگر (nwritten < 0)
        {
          اگر (errno != EINTR)
            توڑو;
        }
      ورنہ اگر (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          توڑو;
        }
      ورنہ
        کل_written += nwritten;
    }

  اگر (o_nocache && کل_written)
    invalidate_cache (fd, کل_written);

  واپس کل_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ساکن خالی
write_output (خالی)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_بائٹیں += nwritten;
  اگر (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      اگر (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  ورنہ
    w_بھرا++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ساکن صحیح
ifdatasync (صحیح fd)
{
  صحیح ret;

  کرو
    {
      process_اشارات ();
      ret = fdatasync (fd);
    }
  جب (ret < 0 && errno == EINTR);

  واپس ret;
}

/* Restart on EINTR from fd_reopen.  */

ساکن صحیح
ifd_reopen (صحیح desired_fd, حرف ثابت *file, صحیح flag, mode_t منوال_2)
{
  صحیح ret;

  کرو
    {
      process_اشارات ();
      ret = fd_reopen (desired_fd, file, flag, منوال_2);
    }
  جب (ret < 0 && errno == EINTR);

  واپس ret;
}

/* Restart on EINTR from fstat.  */

ساکن صحیح
ifstat (صحیح fd, ساخت stat *st)
{
  صحیح ret;

  کرو
    {
      process_اشارات ();
      ret = fstat (fd, st);
    }
  جب (ret < 0 && errno == EINTR);

  واپس ret;
}

/* Restart on EINTR from fsync.  */

ساکن صحیح
ifsync (صحیح fd)
{
  صحیح ret;

  کرو
    {
      process_اشارات ();
      ret = fsync (fd);
    }
  جب (ret < 0 && errno == EINTR);

  واپس ret;
}

/* Restart on EINTR from ftruncate.  */

ساکن صحیح
iftruncate (صحیح fd, off_t لمبائی)
{
  صحیح ret;

  کرو
    {
      process_اشارات ();
      ret = ftruncate (fd, لمبائی);
    }
  جب (ret < 0 && errno == EINTR);

  واپس ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_خالص
ساکن bool
عملیہ_matches (حرف ثابت *str, حرف ثابت *نمونہ_2, حرف delim)
{
  جب (*نمونہ_2)
    اگر (*str++ != *نمونہ_2++)
      واپس false;
  واپس !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ساکن صحیح
parse_symbols (حرف ثابت *str, ساخت علامت_قدر ثابت *جدول,
               bool exclusive, حرف ثابت *error_msgid)
{
  صحیح قدر = 0;

  جب (true)
    {
      حرف ثابت *strcomma = strchr (str, ',');
      ساخت علامت_قدر ثابت *اندراج_2;

      برائے (اندراج_2 = جدول;
           ! (عملیہ_matches (str, اندراج_2->علامت, ',') && اندراج_2->قدر);
           اندراج_2++)
        {
          اگر (! اندراج_2->علامت[0])
            {
              idx_t slen = strcomma ? strcomma - str : لڑی_لمبائی (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      اگر (exclusive)
        قدر = اندراج_2->قدر;
      ورنہ
        قدر |= اندراج_2->قدر;
      اگر (!strcomma)
        توڑو;
      str = strcomma + 1;
    }

  واپس قدر;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

ساکن intmax_t
parse_integer (حرف ثابت *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  صحیح indeterminate = 0;
  ساکن حرف ثابت suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t نتیجہ = 1;
  bool حد_سے_تجاوز = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  جب (true)
    {
      uintmax_t n = indeterminate;
      حرف *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      اگر ((e & ~LONGINT_حد_سے_تجاوز) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          اگر (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_حد_سے_تجاوز) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      اگر (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      اگر ((e & ~LONGINT_حد_سے_تجاوز) != LONGINT_OK)
        {
          اگر (! (e & LONGINT_حد_سے_تجاوز) && n <= INTMAX_MAX)
            {
              *invalid = e;
              واپس indeterminate;
            }
          e = LONGINT_حد_سے_تجاوز;
        }

      اگر (n == 0)
        نتیجہ = 0;
      ورنہ اگر (نتیجہ != 0)
        {
          intmax_t حاصل_ضرب;
          اگر ((e & LONGINT_حد_سے_تجاوز)
              || حد_سے_تجاوز
              || ckd_mul (&حاصل_ضرب, نتیجہ, n))
            حد_سے_تجاوز = true;
          ورنہ
            نتیجہ = حاصل_ضرب;
        }

      اگر (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      اگر (! multiply)
        توڑو;
      str = suffix + 1;
    }

  اگر (نتیجہ == 0)
    {
      اگر (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  ورنہ اگر (حد_سے_تجاوز)
    {
      e = LONGINT_حد_سے_تجاوز;
      نتیجہ = INTMAX_MAX;
    }

  *invalid = e;
  واپس نتیجہ;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_خالص
ساکن bool
عملیہ_is (حرف ثابت *عملیہ_2, حرف ثابت *name)
{
  واپس عملیہ_matches (عملیہ_2, name, '=');
}

ساکن خالی
scanargs (صحیح argc, حرف *ثابت *argv)
{
  idx_t blocksize = 0;
  intmax_t گنتی = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool گنتی_B = false, skip_B = false, seek_B = false;

  برائے (صحیح i = optind; i < argc; i++)
    {
      حرف ثابت *name = argv[i];
      حرف ثابت *val = strchr (name, '=');

      اگر (val == تہی)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      اگر (عملیہ_is (name, "if"))
        داخلہ_file = val;
      ورنہ اگر (عملیہ_is (name, "of"))
        output_file = val;
      ورنہ اگر (عملیہ_is (name, "conv"))
        conversions_نقاب |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      ورنہ اگر (عملیہ_is (name, "iflag"))
        داخلہ_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      ورنہ اگر (عملیہ_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      ورنہ اگر (عملیہ_is (name, "status"))
        حالت_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      ورنہ
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = تہی;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          اگر (عملیہ_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &داخلہ_blocksize;
            }
          ورنہ اگر (عملیہ_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          ورنہ اگر (عملیہ_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          ورنہ اگر (عملیہ_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          ورنہ اگر (عملیہ_is (name, "skip") || عملیہ_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          ورنہ اگر (عملیہ_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          ورنہ اگر (عملیہ_is (name, "count"))
            {
              گنتی = n;
              گنتی_B = has_B;
            }
          ورنہ
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          اگر (n < n_min)
            invalid = LONGINT_INVALID;
          ورنہ اگر (n_max < n)
            invalid = LONGINT_حد_سے_تجاوز;

          اگر (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_حد_سے_تجاوز ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          ورنہ اگر (converted_idx)
            *converted_idx = n;
        }
    }

  اگر (blocksize)
    داخلہ_blocksize = output_blocksize = blocksize;
  ورنہ
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_نقاب |= C_TWOBUFS;
    }

  اگر (داخلہ_blocksize == 0)
    داخلہ_blocksize = DEFAULT_BLOCKSIZE;
  اگر (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  اگر (conversion_blocksize == 0)
    conversions_نقاب &= ~(C_بلاک | C_UNBLOCK);

  اگر (داخلہ_flags & (O_DSYNC | O_SYNC))
    داخلہ_flags |= O_RSYNC;

  اگر (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  اگر (skip_B)
    داخلہ_flags |= O_SKIP_بائٹیں;
  اگر (داخلہ_flags & O_SKIP_بائٹیں && skip != 0)
    {
      skip_اندراجات = skip / داخلہ_blocksize;
      skip_بائٹیں = skip % داخلہ_blocksize;
    }
  ورنہ اگر (skip != 0)
    skip_اندراجات = skip;

  اگر (گنتی_B)
    داخلہ_flags |= O_گنتی_بائٹیں;
  اگر (داخلہ_flags & O_گنتی_بائٹیں && گنتی != INTMAX_MAX)
    {
      max_اندراجات = گنتی / داخلہ_blocksize;
      max_بائٹیں = گنتی % داخلہ_blocksize;
    }
  ورنہ اگر (گنتی != INTMAX_MAX)
    max_اندراجات = گنتی;

  اگر (seek_B)
    output_flags |= O_SEEK_بائٹیں;
  اگر (output_flags & O_SEEK_بائٹیں && seek != 0)
    {
      seek_اندراجات = seek / output_blocksize;
      seek_بائٹیں = seek % output_blocksize;
    }
  ورنہ اگر (seek != 0)
    seek_اندراجات = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_نقاب & C_TWOBUFS) && ! (داخلہ_flags & O_FULLBLOCK)
     && (skip_اندراجات
         || (0 < max_اندراجات && max_اندراجات < INTMAX_MAX)
         || (داخلہ_flags | output_flags) & O_براہ_راست));

  iread_fnc = ((داخلہ_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  داخلہ_flags &= ~O_FULLBLOCK;

  اگر (مضرب_بٹ_سیٹ_2 (conversions_نقاب & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  اگر (مضرب_بٹ_سیٹ_2 (conversions_نقاب & (C_بلاک | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  اگر (مضرب_بٹ_سیٹ_2 (conversions_نقاب & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  اگر (مضرب_بٹ_سیٹ_2 (conversions_نقاب & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  اگر (مضرب_بٹ_سیٹ_2 (داخلہ_flags & (O_براہ_راست | O_NOCACHE))
      || مضرب_بٹ_سیٹ_2 (output_flags & (O_براہ_راست | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  اگر (داخلہ_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_اندراجات == 0 && max_بائٹیں == 0);
      داخلہ_flags &= ~O_NOCACHE;
    }
  اگر (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_اندراجات == 0 && max_بائٹیں == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ساکن خالی
apply_translations (خالی)
{
  اگر (conversions_نقاب & C_ASCII)
    translate_charset (ebcdic_مقصد_ascii);

  اگر (conversions_نقاب & C_UCASE)
    {
      برائے (صحیح i = 0; i < 256; i++)
        trans_جدول[i] = بڑا_حرف (trans_جدول[i]);
      translation_needed = true;
    }
  ورنہ اگر (conversions_نقاب & C_LCASE)
    {
      برائے (صحیح i = 0; i < 256; i++)
        trans_جدول[i] = tolower (trans_جدول[i]);
      translation_needed = true;
    }

  اگر (conversions_نقاب & C_EBCDIC)
    {
      translate_charset (ascii_مقصد_ebcdic);
      newline_character = ascii_مقصد_ebcdic['\n'];
      space_character = ascii_مقصد_ebcdic[' '];
    }
  ورنہ اگر (conversions_نقاب & C_IBM)
    {
      translate_charset (ascii_مقصد_ibm);
      newline_character = ascii_مقصد_ibm['\n'];
      space_character = ascii_مقصد_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ساکن خالی
translate_بفر (حرف *buf, idx_t nread)
{
  حرف *cp = buf;
  برائے (idx_t i = nread; i; i--, cp++)
    *cp = trans_جدول[مقصد_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ساکن حرف *
swab_بفر (حرف *buf, idx_t *nread, صحیح *saved_byte)
{
  اگر (*nread == 0)
    واپس buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  صحیح پچھلا_saved = *saved_byte;
  اگر ((پچھلا_saved < 0) == (*nread & 1))
    {
      غیرعلامتی حرف c = buf[--*nread];
      *saved_byte = c;
    }
  ورنہ
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  برائے (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  اگر (پچھلا_saved < 0)
    واپس buf + 1;

  buf[1] = پچھلا_saved;
  ++*nread;
  واپس buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

ساکن خالی
advance_داخلہ_انحراف (intmax_t انحراف)
{
  اگر (0 <= داخلہ_انحراف && ckd_جوڑو (&داخلہ_انحراف, داخلہ_انحراف, انحراف))
    داخلہ_انحراف = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

ساکن intmax_t
skip (صحیح fdesc, حرف ثابت *file, intmax_t اندراجات, idx_t blocksize,
      idx_t *بائٹیں)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t انحراف;
  اگر (! ckd_mul (&انحراف, اندراجات, blocksize)
      && ! ckd_جوڑو (&انحراف, انحراف, *بائٹیں)
      && 0 <= lseek (fdesc, انحراف, SEEK_CUR))
    {
      اگر (fdesc == STDIN_FILENO)
        {
           ساخت stat st;
           اگر (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           اگر (usable_st_سائز (&st) && 0 < st.st_سائز && 0 <= داخلہ_انحراف
               && st.st_سائز - داخلہ_انحراف < انحراف)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               اندراجات = ( انحراف - st.st_سائز ) / blocksize;
               انحراف = st.st_سائز - داخلہ_انحراف;
             }
           ورنہ
             اندراجات = 0;
           advance_داخلہ_انحراف (انحراف);
        }
      ورنہ
        {
          اندراجات = 0;
          *بائٹیں = 0;
        }
      واپس اندراجات;
    }
  ورنہ
    {
      صحیح lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      اگر (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          اگر (!lseek_errno)
            {
              /* The original seek was not attempted as offset > OFF_T_MAX.
                 We should error for write as can't get to the desired
                 location, even if OFF_T_MAX < max file size.
                 For read we're not going to read any data anyway,
                 so we should error for consistency.
                 It would be nice to not error for /dev/{zero,null}
                 for any offset, but that's not a significant issue.  */
              lseek_errno = EOVERFLOW;
            }

          diagnose (lseek_errno,
                    gettext (fdesc == STDIN_FILENO
                             ? N_("%s: cannot skip")
                             : N_("%s: cannot seek")),
                    quotef (file));
          /* If the file has a specific size and we've asked
             to skip/seek beyond the max allowable, then quit.  */
          quit (EXIT_FAILURE);
        }
      /* else file_size && offset > OFF_T_MAX or file ! seekable */

      حرف *buf;
      اگر (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      ورنہ
        {
          alloc_obuf ();
          buf = obuf;
        }

      کرو
        {
          ssize_t nread = iread_fnc (fdesc, buf, اندراجات ? blocksize : *بائٹیں);
          اگر (nread < 0)
            {
              اگر (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  اگر (conversions_نقاب & C_NOERROR)
                    print_stats ();
                }
              ورنہ
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          ورنہ اگر (nread == 0)
            توڑو;
          ورنہ اگر (fdesc == STDIN_FILENO)
            advance_داخلہ_انحراف (nread);

          اگر (اندراجات != 0)
            اندراجات--;
          ورنہ
            *بائٹیں = 0;
        }
      جب (اندراجات || *بائٹیں);

      واپس اندراجات;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ساکن bool
advance_داخلہ_after_read_error (idx_t nbytes)
{
  اگر (! داخلہ_seekable)
    {
      اگر (داخلہ_seek_errno == ESPIPE)
        واپس true;
      errno = داخلہ_seek_errno;
    }
  ورنہ
    {
      off_t انحراف;
      advance_داخلہ_انحراف (nbytes);
      اگر (داخلہ_انحراف < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (داخلہ_file));
          واپس false;
        }
      انحراف = lseek (STDIN_FILENO, 0, SEEK_CUR);
      اگر (0 <= انحراف)
        {
          off_t diff;
          اگر (انحراف == داخلہ_انحراف)
            واپس true;
          diff = داخلہ_انحراف - انحراف;
          اگر (! (0 <= diff && diff <= nbytes) && حالت_level != حالت_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          اگر (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            واپس true;
          اگر (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (داخلہ_file));
  واپس false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ساکن خالی
copy_simple (حرف ثابت *buf, idx_t nread)
{
  حرف ثابت *آغاز = buf;	/* First uncopied char in BUF.  */

  کرو
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      حافظہ_نقل (obuf + oc, آغاز, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      آغاز += nfree;
      oc += nfree;
      اگر (oc >= output_blocksize)
        write_output ();
    }
  جب (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

ساکن خالی
copy_with_بلاک (حرف ثابت *buf, idx_t nread)
{
  برائے (idx_t i = nread; i; i--, buf++)
    {
      اگر (*buf == newline_character)
        {
          اگر (ستون < conversion_blocksize)
            {
              برائے (idx_t j = ستون; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          ستون = 0;
        }
      ورنہ
        {
          اگر (ستون == conversion_blocksize)
            r_truncate++;
          ورنہ اگر (ستون < conversion_blocksize)
            output_char (*buf);
          ستون++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

ساکن خالی
copy_with_unblock (حرف ثابت *buf, idx_t nread)
{
  ساکن idx_t pending_spaces = 0;

  برائے (idx_t i = 0; i < nread; i++)
    {
      حرف c = buf[i];

      اگر (ستون++ >= conversion_blocksize)
        {
          ستون = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      ورنہ اگر (c == space_character)
        pending_spaces++;
      ورنہ
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          جب (pending_spaces)
            {
              output_char (space_character);
              --pending_spaces;
            }
          output_char (c);
        }
    }
}

/* Set the file descriptor flags for FD that correspond to the nonzero bits
   in ADD_FLAGS.  The file's name is NAME.  */

ساکن خالی
سیٹ_fd_flags (صحیح fd, صحیح جوڑو_flags, حرف ثابت *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  جوڑو_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  اگر (جوڑو_flags)
    {
      صحیح old_flags = fcntl (fd, F_GETFL);
      صحیح new_flags = old_flags | جوڑو_flags;
      bool ok = true;
      اگر (old_flags < 0)
        ok = false;
      ورنہ اگر (old_flags != new_flags)
        {
          اگر (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              ساخت stat st;
              اگر (ifstat (fd, &st) != 0)
                ok = false;
              ورنہ اگر ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_منوال))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              ورنہ اگر ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          اگر (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      اگر (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

ساکن صحیح
dd_copy (خالی)
{
  حرف *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  صحیح exit_حالت = EXIT_SUCCESS;
  idx_t n_بائٹیں_read;

  اگر (skip_اندراجات != 0 || skip_بائٹیں != 0)
    {
      intmax_t us_بائٹیں;
      bool us_بائٹیں_حد_سے_تجاوز =
        (ckd_mul (&us_بائٹیں, skip_اندراجات, داخلہ_blocksize)
         || ckd_جوڑو (&us_بائٹیں, skip_بائٹیں, us_بائٹیں));
      off_t داخلہ_offset0 = داخلہ_انحراف;
      intmax_t us_بلاکیں = skip (STDIN_FILENO, داخلہ_file,
                                 skip_اندراجات, داخلہ_blocksize, &skip_بائٹیں);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      اگر ((us_بلاکیں
           || (0 <= داخلہ_انحراف
               && (us_بائٹیں_حد_سے_تجاوز
                   || us_بائٹیں != داخلہ_انحراف - داخلہ_offset0)))
          && حالت_level != حالت_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (داخلہ_file));
        }
    }

  اگر (seek_اندراجات != 0 || seek_بائٹیں != 0)
    {
      idx_t بائٹیں = seek_بائٹیں;
      intmax_t write_اندراجات = skip (STDOUT_FILENO, output_file,
                                      seek_اندراجات, output_blocksize, &بائٹیں);

      اگر (write_اندراجات != 0 || بائٹیں != 0)
        {
          memset (obuf, 0, write_اندراجات ? output_blocksize : بائٹیں);

          کرو
            {
              idx_t سائز = write_اندراجات ? output_blocksize : بائٹیں;
              اگر (iwrite (STDOUT_FILENO, obuf, سائز) != سائز)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              اگر (write_اندراجات != 0)
                write_اندراجات--;
              ورنہ
                بائٹیں = 0;
            }
          جب (write_اندراجات || بائٹیں);
        }
    }

  اگر (max_اندراجات == 0 && max_بائٹیں == 0)
    واپس exit_حالت;

  alloc_ibuf ();
  alloc_obuf ();
  صحیح saved_byte = -1;

  جب (true)
    {
      اگر (حالت_level == حالت_پیش_رفت)
        {
          xtime_t پیش_رفت_وقت = gethrxtime ();
          اگر (اگلا_وقت <= پیش_رفت_وقت)
            {
              print_xfer_stats (پیش_رفت_وقت);
              اگلا_وقت += XTIME_PRECISION;
            }
        }

      اگر (r_partial + r_بھرا >= max_اندراجات + !!max_بائٹیں)
        توڑو;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      اگر ((conversions_نقاب & C_SYNC) && (conversions_نقاب & C_NOERROR))
        memset (ibuf,
                (conversions_نقاب & (C_بلاک | C_UNBLOCK)) ? ' ' : '\0',
                داخلہ_blocksize);

      اگر (r_partial + r_بھرا >= max_اندراجات)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_بائٹیں);
      ورنہ
        nread = iread_fnc (STDIN_FILENO, ibuf, داخلہ_blocksize);

      اگر (nread > 0)
        {
          advance_داخلہ_انحراف (nread);
          اگر (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      ورنہ اگر (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_نقاب & C_NOTRUNC);
          توڑو;			/* EOF.  */
        }
      ورنہ
        {
          اگر (!(conversions_نقاب & C_NOERROR) || حالت_level != حالت_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (داخلہ_file));

          اگر (conversions_نقاب & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = داخلہ_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              اگر (!advance_داخلہ_after_read_error (bad_portion))
                {
                  exit_حالت = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  داخلہ_seekable = false;
                  داخلہ_seek_errno = ESPIPE;
                }
              اگر ((conversions_نقاب & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              ورنہ
                جاری;
            }
          ورنہ
            {
              /* Write any partial block. */
              exit_حالت = EXIT_FAILURE;
              توڑو;
            }
        }

      n_بائٹیں_read = nread;

      اگر (n_بائٹیں_read < داخلہ_blocksize)
        {
          r_partial++;
          partread = n_بائٹیں_read;
          اگر (conversions_نقاب & C_SYNC)
            {
              اگر (!(conversions_نقاب & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_بائٹیں_read,
                        (conversions_نقاب & (C_بلاک | C_UNBLOCK)) ? ' ' : '\0',
                        داخلہ_blocksize - n_بائٹیں_read);
              n_بائٹیں_read = داخلہ_blocksize;
            }
        }
      ورنہ
        {
          r_بھرا++;
          partread = 0;
        }

      اگر (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_بائٹیں_read);
          w_بائٹیں += nwritten;
          اگر (nwritten != n_بائٹیں_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              اگر (nwritten != 0)
                w_partial++;
              واپس EXIT_FAILURE;
            }
          ورنہ اگر (n_بائٹیں_read == داخلہ_blocksize)
            w_بھرا++;
          ورنہ
            w_partial++;
          جاری;
        }

      /* Do any translations on the whole buffer at once.  */

      اگر (translation_needed)
        translate_بفر (ibuf, n_بائٹیں_read);

      اگر (conversions_نقاب & C_SWAB)
        bufstart = swab_بفر (ibuf, &n_بائٹیں_read, &saved_byte);
      ورنہ
        bufstart = ibuf;

      اگر (conversions_نقاب & C_بلاک)
        copy_with_بلاک (bufstart, n_بائٹیں_read);
      ورنہ اگر (conversions_نقاب & C_UNBLOCK)
        copy_with_unblock (bufstart, n_بائٹیں_read);
      ورنہ
        copy_simple (bufstart, n_بائٹیں_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  اگر (0 <= saved_byte)
    {
      حرف saved_char = saved_byte;
      اگر (conversions_نقاب & C_بلاک)
        copy_with_بلاک (&saved_char, 1);
      ورنہ اگر (conversions_نقاب & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      ورنہ
        output_char (saved_char);
    }

  اگر ((conversions_نقاب & C_بلاک) && ستون > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      برائے (idx_t i = ستون; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  اگر (ستون && (conversions_نقاب & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  اگر (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_بائٹیں += nwritten;
      اگر (nwritten != 0)
        w_partial++;
      اگر (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          واپس EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  اگر (final_op_was_seek)
    {
      ساخت stat stdout_stat;
      اگر (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          واپس EXIT_FAILURE;
        }
      اگر (S_ISREG (stdout_stat.st_منوال) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_انحراف = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          اگر (0 <= output_انحراف && stdout_stat.st_سائز < output_انحراف)
            {
              اگر (iftruncate (STDOUT_FILENO, output_انحراف) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_انحراف}, quoteaf (output_file));
                  واپس EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  اگر (conversions_نقاب & (C_FDATASYNC | C_FSYNC)
      && حالت_level == حالت_پیش_رفت
      && 0 <= reported_w_بائٹیں && reported_w_بائٹیں < w_بائٹیں)
    print_xfer_stats (0);

  واپس exit_حالت;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ساکن صحیح
synchronize_output (خالی)
{
  صحیح exit_حالت = 0;
  صحیح نقاب = conversions_نقاب;
  conversions_نقاب &= ~ (C_FDATASYNC | C_FSYNC);

  اگر ((نقاب & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      اگر (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_حالت = EXIT_FAILURE;
        }
      نقاب |= C_FSYNC;
    }

  اگر ((نقاب & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      واپس EXIT_FAILURE;
    }

  واپس exit_حالت;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  off_t انحراف;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  صفحہ_سائز = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE, نسخہ,
                                   true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  برائے (صحیح i = 0; i < 256; i++)
    trans_جدول[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  اگر (داخلہ_file == تہی)
    {
      داخلہ_file = _("standard input");
      سیٹ_fd_flags (STDIN_FILENO, داخلہ_flags, داخلہ_file);
    }
  ورنہ
    {
      اگر (ifd_reopen (STDIN_FILENO, داخلہ_file, O_RDONLY | داخلہ_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (داخلہ_file));
    }

  انحراف = lseek (STDIN_FILENO, 0, SEEK_CUR);
  داخلہ_seekable = (0 <= انحراف);
  داخلہ_انحراف = MAX (0, انحراف);
  داخلہ_seek_errno = errno;

  اگر (output_file == تہی)
    {
      output_file = _("standard output");
      سیٹ_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  ورنہ
    {
      mode_t perms = منوال_RW_UGO;
      صحیح opts
        = (output_flags
           | (conversions_نقاب & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_نقاب & C_EXCL ? O_EXCL : 0)
           | (seek_اندراجات || (conversions_نقاب & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t سائز;
      اگر ((ckd_mul (&سائز, seek_اندراجات, output_blocksize)
           || ckd_جوڑو (&سائز, seek_بائٹیں, سائز))
          && !(conversions_نقاب & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_اندراجات, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      اگر ((! seek_اندراجات
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      اگر (seek_اندراجات != 0 && !(conversions_نقاب & C_NOTRUNC))
        {
          اگر (iftruncate (STDOUT_FILENO, سائز) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              صحیح ftruncate_errno = errno;
              ساخت stat stdout_stat;
              اگر (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              ورنہ اگر (S_ISREG (stdout_stat.st_منوال)
                       || S_ISDIR (stdout_stat.st_منوال)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = سائز;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  آغاز_وقت = gethrxtime ();
  اگلا_وقت = آغاز_وقت + XTIME_PRECISION;

  صحیح copy_حالت = dd_copy ();
  صحیح sync_حالت = synchronize_output ();
  صحیح exit_حالت = copy_حالت | sync_حالت;

  اگر (max_اندراجات == 0 && max_بائٹیں == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      اگر (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (داخلہ_file));
          exit_حالت = EXIT_FAILURE;
        }
      اگر (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_حالت = EXIT_FAILURE;
        }
    }
  ورنہ
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      اگر (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      اگر (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  اختتام_اوپر ();
  main_exit (exit_حالت);
}
