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

#ضمن <config.h>

#ضمن <sys/types.h>
#ضمن <signal.h>

#ضمن "system.h"
#ضمن "alignalloc.h"
#ضمن "close-stream.h"
#ضمن "fd-reopen.h"
#ضمن "gethrxtime.h"
#ضمن "human.h"
#ضمن "ioblksize.h"
#ضمن "long-options.h"
#ضمن "quote.h"
#ضمن "xstrtol.h"
#ضمن "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "dd"

#تعریف_کن AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#اگر_تعریف_نشده SA_RESETHAND
# define SA_RESETHAND 0
#پایان_اگر

#اگر_تعریف_نشده SIGINFO
# define SIGINFO SIGUSR1
#پایان_اگر

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#اگر_تعریف_نشده O_CIO
# define O_CIO 0
#پایان_اگر

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#لغو_تعریف O_NOCACHE

#تعریف_کن output_char(c)				\
  افعل						\
    {						\
      obuf[oc++] = (c);				\
      إذا (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  طالما (0)

/* Default input and output blocksize. */
#تعریف_کن DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
تعداد
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_بلوک = 010,
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
تعداد
  {
    وضعیت_NONE = 1,
    وضعیت_NOXFER = 2,
    وضعیت_DEFAULT = 3,
    وضعیت_پیشرفت = 4
  };

/* The name of the input file, or NULL for the standard input. */
ساكن حرف ثابت *درون_داد_file = عدم;

/* The name of the output file, or NULL for the standard output. */
ساكن حرف ثابت *output_file = عدم;

/* The page size on this host.  */
ساكن idx_t صفحه_اندازه;

/* The number of bytes in which atomic reads are done. */
ساكن idx_t درون_داد_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ساكن idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ساكن idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ساكن intmax_t skip_رکوردها = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ساكن idx_t skip_بایتها = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ساكن intmax_t seek_رکوردها = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ساكن intmax_t seek_بایتها = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ساكن bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ساكن intmax_t max_رکوردها = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ساكن idx_t max_بایتها = 0;

/* Bit vector of conversions to apply. */
ساكن صحيح conversions_ماسک = 0;

/* Open flags for the input and output files.  */
ساكن صحيح درون_داد_flags = 0;
ساكن صحيح output_flags = 0;

/* Status flags for what is printed to stderr.  */
ساكن صحيح وضعیت_level = وضعیت_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ساكن bool translation_needed = false;

/* Number of partial blocks written. */
ساكن intmax_t w_partial = 0;

/* Number of full blocks written. */
ساكن intmax_t w_پر = 0;

/* Number of partial blocks read. */
ساكن intmax_t r_partial = 0;

/* Number of full blocks read. */
ساكن intmax_t r_پر = 0;

/* Number of bytes written.  */
ساكن intmax_t w_بایتها = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ساكن intmax_t reported_w_بایتها = -1;

/* Time that dd started.  */
ساكن xtime_t شروع_زمان;

/* Next time to report periodic progress.  */
ساكن xtime_t بعدی_زمان;

/* If positive, the number of bytes output in the current progress line.  */
ساكن صحيح پیشرفت_len;

/* True if input is seekable.  */
ساكن bool درون_داد_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ساكن صحيح درون_داد_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ساكن off_t درون_داد_جابه_جایی;

/* True if a partial read should be diagnosed.  */
ساكن bool warn_partial_read;

/* Records truncated by conv=block. */
ساكن intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
ساكن حرف newline_character = '\n';
ساكن حرف space_character = ' ';

/* I/O buffers.  */
ساكن حرف *ibuf;
ساكن حرف *obuf;

/* Current index into 'obuf'. */
ساكن idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
ساكن idx_t ستون = 0;

/* The set of signals that are caught.  */
ساكن sigset_t caught_سیگنالها;

/* If nonzero, the value of the pending fatal signal.  */
ساكن sig_atomic_t متطاير وقفه_signal;

/* A count of the number of pending info signals that have been received.  */
ساكن sig_atomic_t متطاير info_signal_شمار;

/* Whether to discard cache for input or output.  */
ساكن bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ساكن bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ساكن ssize_t (*iread_fnc) (صحيح fd, حرف *buf, idx_t اندازه);

/* A longest symbol in the struct symbol_values tables below.  */
#تعریف_کن LONGEST_نماد "count_bytes"

/* A symbol and the corresponding integer value.  */
بنية نماد_مقدار
{
  حرف نماد[حجم LONGEST_نماد];
  صحيح مقدار;
};

/* Conversion symbols, for conv="...".  */
ساكن بنية نماد_مقدار ثابت conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_بلوک | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_بلوک | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_بلوک | C_TWOBUFS},	/* Variable to fixed length records. */
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

#تعریف_کن FFS_ماسک(x) ((x) ^ ((x) & ((x) - 1)))
تعداد
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_دودویی
          | O_CIO
          | O_مستقیم
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
    O_FULLBLOCK = FFS_ماسک (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_ماسک (v2),
    v3 = v2 ^ O_NOCACHE,

    O_شمار_بایتها = FFS_ماسک (v3),
    v4 = v3 ^ O_شمار_بایتها,

    O_SKIP_بایتها = FFS_ماسک (v4),
    v5 = v4 ^ O_SKIP_بایتها,

    O_SEEK_بایتها = FFS_ماسک (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_شمار_بایتها != 0);
static_assert (O_SKIP_بایتها != 0);
static_assert (O_SEEK_بایتها != 0);

#تعریف_کن مضرب_بیتها_مجموعه(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! مضرب_بیتها_مجموعه (O_FULLBLOCK));
static_assert ( ! مضرب_بیتها_مجموعه (O_NOCACHE));
static_assert ( ! مضرب_بیتها_مجموعه (O_شمار_بایتها));
static_assert ( ! مضرب_بیتها_مجموعه (O_SKIP_بایتها));
static_assert ( ! مضرب_بیتها_مجموعه (O_SEEK_بایتها));

/* Flags, for iflag="..." and oflag="...".  */
ساكن بنية نماد_مقدار ثابت flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_دودویی},
  {"cio",	  O_CIO},
  {"direct",	  O_مستقیم},
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
  {"count_bytes", O_شمار_بایتها},
  {"skip_bytes",  O_SKIP_بایتها},
  {"seek_bytes",  O_SEEK_بایتها},
  {"",		0}
};

/* Status, for status="...".  */
ساكن بنية نماد_مقدار ثابت statuses[] =
{
  {"none",	وضعیت_NONE},
  {"noxfer",	وضعیت_NOXFER},
  {"progress",	وضعیت_پیشرفت},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ساكن بیعلامت حرف trans_جدول[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ساكن حرف ثابت ascii_مقصد_ebcdic[] =
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

ساكن حرف ثابت ascii_مقصد_ibm[] =
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

ساكن حرف ثابت ebcdic_مقصد_ascii[] =
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
ساكن bool close_stdout_required = true;

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
ساكن فراغ
maybe_close_stdout (فراغ)
{
  إذا (close_stdout_required)
    close_stdout ();
  وإلا إذا (close_جریان (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ساكن فراغ
diagnose (صحيح errnum, حرف ثابت *fmt, ...)
{
  إذا (0 < پیشرفت_len)
    {
      fputc ('\n', stderr);
      پیشرفت_len = 0;
    }

  va_list ap;
  va_شروع (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_انتها (ap);
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [عملوند]...\n\
  or:  %s OPTION\n\
"),
              برنامه_name, برنامه_name);
      fputs (_("\
Copy a file, converting and formatting according مقصد the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=بایتها_2        اقرأ and اكتب به_بالا مقصد بایتها_2 بایتها at a زمان (افتراضي: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=بایتها_2       convert بایتها_2 بایتها at a زمان\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as در_هر the comma separated نماد list\n\
"));
      oputs (_("\
  شمار=N         copy only N درون_داد بلوکها\n\
"));
      oputs (_("\
  ibs=بایتها_2       اقرأ به_بالا مقصد بایتها_2 بایتها at a زمان (افتراضي: 512)\n\
"));
      oputs (_("\
  إذا=FILE         اقرأ from FILE instead از standard درون_داد\n\
"));
      oputs (_("\
  iflag=FLAGS     اقرأ as در_هر the comma separated نماد list\n\
"));
      oputs (_("\
  obs=بایتها_2       اكتب بایتها_2 بایتها at a زمان (افتراضي: 512)\n\
"));
      oputs (_("\
  از=FILE         اكتب مقصد FILE instead از standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     اكتب as در_هر the comma separated نماد list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output بلوکها\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized درون_داد بلوکها\n\
"));
      oputs (_("\
  وضعیت=LEVEL    The LEVEL از information مقصد print مقصد standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and بایتها_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
دودویی_2 prefixes can be استفاده_شده, too: KiB=K, MiB=M, and so on.\n\
If N دوسر درون 'B', it counts بایتها not بلوکها.\n\
\n\
Each CONV نماد may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC مقصد ASCII\n\
  ebcdic    from ASCII مقصد EBCDIC\n\
  ibm       from ASCII مقصد alternate EBCDIC\n\
  بلوک     pad newline-terminated رکوردها with spaces مقصد cbs-اندازه\n\
  unblock   replace trailing spaces درون cbs-اندازه رکوردها with newline\n\
  lcase     change فوقانی حالة مقصد پایینی حالة\n\
  ucase     change پایینی حالة مقصد فوقانی حالة\n\
  sparse    try مقصد seek rather than اكتب all-NUL output بلوکها\n\
  swab      جابجا every pair از درون_داد بایتها\n\
  sync      pad every درون_داد بلوک with NULs مقصد ibs-اندازه; when استفاده_شده\n\
            with بلوک or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail إذا the output file already exists\n\
  nocreat   افعل not create the output file\n\
  notrunc   افعل not truncate the output file\n\
  noerror   استمر after اقرأ errors\n\
  fdatasync  physically اكتب output file داده before finishing\n\
  fsync     likewise, but also اكتب metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG نماد may be:\n\
\n\
  append    append نما_2 (makes sense only لكل output; conv=notrunc suggested)\n\
"), stdout);
      إذا (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      إذا (O_مستقیم)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      إذا (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      إذا (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      إذا (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      إذا (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#إذا HAVE_POSIX_FADVISE
      إذا (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#پایان_اگر
      إذا (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      إذا (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      إذا (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      إذا (O_دودویی)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      إذا (O_متن)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        چاپ_کن (_("\
\n\
Sending a %s إشارة مقصد a running 'dd' process makes it\n\
print I/O statistics مقصد standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Common options to use when displaying sizes and rates.  */

تعداد { human_opts = (human_autoscale | human_دور_مقصد_نزدیکترین
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ساكن فراغ
alloc_ibuf (فراغ)
{
  إذا (ibuf)
    أرجع;

  bool extra_byte_for_swab = !!(conversions_ماسک & C_SWAB);
  ibuf = alignalloc (صفحه_اندازه, درون_داد_blocksize + extra_byte_for_swab);
  إذا (!ibuf)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             درون_داد_blocksize,
             human_readable (درون_داد_blocksize, hbuf,
                             human_opts | human_پایه_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ساكن فراغ
alloc_obuf (فراغ)
{
  إذا (obuf)
    أرجع;

  إذا (conversions_ماسک & C_TWOBUFS)
    {
      obuf = alignalloc (صفحه_اندازه, output_blocksize);
      إذا (!obuf)
        {
          حرف hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_پایه_1024, 1, 1));
        }
    }
  وإلا
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

ساكن فراغ
translate_charset (حرف ثابت *new_trans)
{
  لكل (صحيح i = 0; i < 256; i++)
    trans_جدول[i] = new_trans[trans_جدول[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

ساكن مضمن bool
مضرب_بیتها_مجموعه_2 (صحيح i)
{
  أرجع مضرب_بیتها_مجموعه (i);
}

ساكن bool
abbreviation_lacks_پیشوند (حرف ثابت *پیام_2)
{
  أرجع پیام_2[طول_رشته (پیام_2) - 2] == ' ';
}

/* Print transfer statistics.  */

ساكن فراغ
print_xfer_stats (xtime_t پیشرفت_زمان)
{
  xtime_t now = پیشرفت_زمان ? پیشرفت_زمان : gethrxtime ();
  ساكن حرف ثابت slash_s[] = "/s";
  حرف hbuf[3][LONGEST_HUMAN_READABLE + حجم slash_s];
  مزدوج delta_s;
  حرف ثابت *بایتها_در_هر_دومی;
  حرف ثابت *si = human_readable (w_بایتها, hbuf[0], human_opts, 1, 1);
  حرف ثابت *iec = human_readable (w_بایتها, hbuf[1],
                                    human_opts | human_پایه_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  حرف *bpsbuf = hbuf[2];
  صحيح bpsbufsize = حجم hbuf[2];
  إذا (شروع_زمان < now)
    {
      مزدوج XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - شروع_زمان;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      بایتها_در_هر_دومی = human_readable (w_بایتها, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (بایتها_در_هر_دومی - bpsbuf + bpsbuf, slash_s);
    }
  وإلا
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      بایتها_در_هر_دومی = bpsbuf;
    }

  إذا (پیشرفت_زمان)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  حرف delta_s_buf[24];
  snprintf (delta_s_buf, حجم delta_s_buf,
            پیشرفت_زمان ? "%.0f s" : "%g s", delta_s);

  صحيح stats_len
    = (abbreviation_lacks_پیشوند (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_بایتها)),
                  w_بایتها, delta_s_buf, بایتها_در_هر_دومی)
       : abbreviation_lacks_پیشوند (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_بایتها, si, delta_s_buf, بایتها_در_هر_دومی)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_بایتها, si, iec, delta_s_buf, بایتها_در_هر_دومی));

  إذا (پیشرفت_زمان)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      إذا (0 <= stats_len && stats_len < پیشرفت_len)
        fprintf (stderr, "%*s", پیشرفت_len - stats_len, "");
      پیشرفت_len = stats_len;
    }
  وإلا
    fputc ('\n', stderr);

  reported_w_بایتها = w_بایتها;
}

ساكن فراغ
print_stats (فراغ)
{
  إذا (وضعیت_level == وضعیت_NONE)
    أرجع;

  إذا (0 < پیشرفت_len)
    {
      fputc ('\n', stderr);
      پیشرفت_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_پر, r_partial, w_پر, w_partial);

  إذا (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  إذا (وضعیت_level == وضعیت_NOXFER)
    أرجع;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساكن فراغ
وقفه_handler (صحيح sig)
{
  إذا (! SA_RESETHAND)
    إشارة (sig, SIG_DFL);
  وقفه_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ساكن فراغ
siginfo_handler (MAYBE_استفادهنشده صحيح sig)
{
  info_signal_شمار++;
}

/* Install the signal handlers.  */

ساكن فراغ
install_signal_handlers (فراغ)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  بنية sigaction act;
  sigemptyset (&caught_سیگنالها);
  إذا (catch_siginfo)
    sigaddset (&caught_سیگنالها, SIGINFO);
  sigaction (SIGINT, عدم, &act);
  إذا (act.sa_handler != SIG_IGN)
    sigaddset (&caught_سیگنالها, SIGINT);
  act.sa_mask = caught_سیگنالها;

  إذا (sigismember (&caught_سیگنالها, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, عدم);
    }

  إذا (sigismember (&caught_سیگنالها, SIGINT))
    {
      act.sa_handler = وقفه_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, عدم);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
ساكن صحيح
iclose (صحيح fd)
{
  إذا (close (fd) != 0)
    افعل
      إذا (errno != EINTR)
        أرجع -1;
    طالما (close (fd) != 0 && errno != EBADF);

  أرجع 0;
}

ساكن صحيح synchronize_output (فراغ);

ساكن فراغ
cleanup (فراغ)
{
  إذا (!وقفه_signal)
    {
      صحيح sync_وضعیت = synchronize_output ();
      إذا (sync_وضعیت)
        خروج (sync_وضعیت);
    }

  إذا (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (درون_داد_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  إذا (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

ساكن فراغ
process_سیگنالها (فراغ)
{
  طالما (وقفه_signal || info_signal_شمار)
    {
      صحيح وقفه;
      صحيح infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_سیگنالها, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      وقفه = وقفه_signal;
      infos = info_signal_شمار;

      إذا (infos)
        info_signal_شمار = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, عدم);

      إذا (وقفه)
        cleanup ();
      print_stats ();
      إذا (وقفه)
        ارفع (وقفه);
    }
}

ساكن فراغ
پایان_به_بالا (فراغ)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_سیگنالها ();
  cleanup ();
  print_stats ();
}

ساكن فراغ
quit (صحيح کد)
{
  پایان_به_بالا ();
  خروج (کد);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ساكن off_t
cache_دور (صحيح fd, off_t len)
{
  ساكن off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  إذا (len)
    {
      intmax_t c_pending;
      إذا (ckd_جمع_کن (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      إذا (c_pending > *pending)
        len = c_pending - *pending;
      وإلا
        len = 0;
    }
  وإلا
    len = *pending;

  أرجع len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

ساكن bool
invalidate_cache (صحيح fd, off_t len)
{
  صحيح adv_ret = -1;
  off_t جابه_جایی;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_دور (fd, len);
  إذا (len && !clen)
    أرجع true; /* Don't advise this time.  */
  وإلا إذا (! len && ! clen && ! nocache_eof)
    أرجع true;
  off_t pending = len ? cache_دور (fd, 0) : 0;

  إذا (fd == STDIN_FILENO)
    {
      إذا (درون_داد_seekable)
        جابه_جایی = درون_داد_جابه_جایی;
      وإلا
        {
          جابه_جایی = -1;
          errno = ESPIPE;
        }
    }
  وإلا
    {
      ساكن off_t output_جابه_جایی = -2;

      إذا (output_جابه_جایی != -1)
        {
          إذا (output_جابه_جایی < 0)
            output_جابه_جایی = lseek (fd, 0, SEEK_CUR);
          وإلا إذا (len)
            output_جابه_جایی += clen + pending;
        }

      جابه_جایی = output_جابه_جایی;
    }

  إذا (0 <= جابه_جایی)
   {
     إذا (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#إذا HAVE_POSIX_FADVISE
     جابه_جایی = جابه_جایی - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     إذا (clen == 0)
       جابه_جایی -= جابه_جایی % صفحه_اندازه;
     adv_ret = posix_fadvise (fd, جابه_جایی, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#وإلا
     errno = ENOTSUP;
#پایان_اگر
   }

  أرجع adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ساكن ssize_t
iread (صحيح fd, حرف *buf, idx_t اندازه)
{
  ssize_t nread;
  ساكن ssize_t prev_nread;

  افعل
    {
      process_سیگنالها ();
      nread = اقرأ (fd, buf, اندازه);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      إذا (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < اندازه
          && (درون_داد_flags & O_مستقیم))
        {
          errno = 0;
          nread = 0;
        }
    }
  طالما (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  إذا (0 < nread && nread < اندازه)
    process_سیگنالها ();

  إذا (0 < nread && warn_partial_read)
    {
      إذا (0 < prev_nread && prev_nread < اندازه)
        {
          idx_t prev = prev_nread;
          إذا (وضعیت_level != وضعیت_NONE)
            diagnose (0, ngettext (("warning: partial read (%td byte); "
                                    "suggest iflag=fullblock"),
                                   ("warning: partial read (%td bytes); "
                                    "suggest iflag=fullblock"),
                                   select_plural (prev)),
                      prev);
          warn_partial_read = false;
        }
    }

  prev_nread = nread;
  أرجع nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
ساكن ssize_t
iread_fullblock (صحيح fd, حرف *buf, idx_t اندازه)
{
  ssize_t nread = 0;

  طالما (0 < اندازه)
    {
      ssize_t ncurr = iread (fd, buf, اندازه);
      إذا (ncurr < 0)
        أرجع ncurr;
      إذا (ncurr == 0)
        اكسر;
      nread += ncurr;
      buf   += ncurr;
      اندازه  -= ncurr;
    }

  أرجع nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ساكن idx_t
iwrite (صحيح fd, حرف ثابت *buf, idx_t اندازه)
{
  idx_t مجموع_written = 0;

  إذا ((output_flags & O_مستقیم) && اندازه < output_blocksize)
    {
      صحيح old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      إذا (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_مستقیم) != 0
          && وضعیت_level != وضعیت_NONE)
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
      conversions_ماسک |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  طالما (مجموع_written < اندازه)
    {
      ssize_t nwritten = 0;
      process_سیگنالها ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      إذا ((conversions_ماسک & C_SPARSE) && is_nul (buf, اندازه))
        {
          إذا (lseek (fd, اندازه, SEEK_CUR) < 0)
            {
              conversions_ماسک &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          وإلا
            {
              final_op_was_seek = true;
              nwritten = اندازه;
            }
        }

      إذا (!nwritten)
        nwritten = اكتب (fd, buf + مجموع_written, اندازه - مجموع_written);

      إذا (nwritten < 0)
        {
          إذا (errno != EINTR)
            اكسر;
        }
      وإلا إذا (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          اكسر;
        }
      وإلا
        مجموع_written += nwritten;
    }

  إذا (o_nocache && مجموع_written)
    invalidate_cache (fd, مجموع_written);

  أرجع مجموع_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ساكن فراغ
write_output (فراغ)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_بایتها += nwritten;
  إذا (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      إذا (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  وإلا
    w_پر++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ساكن صحيح
ifdatasync (صحيح fd)
{
  صحيح ret;

  افعل
    {
      process_سیگنالها ();
      ret = fdatasync (fd);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from fd_reopen.  */

ساكن صحيح
ifd_reopen (صحيح desired_fd, حرف ثابت *file, صحيح flag, mode_t نما_2)
{
  صحيح ret;

  افعل
    {
      process_سیگنالها ();
      ret = fd_reopen (desired_fd, file, flag, نما_2);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from fstat.  */

ساكن صحيح
ifstat (صحيح fd, بنية stat *st)
{
  صحيح ret;

  افعل
    {
      process_سیگنالها ();
      ret = fstat (fd, st);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from fsync.  */

ساكن صحيح
ifsync (صحيح fd)
{
  صحيح ret;

  افعل
    {
      process_سیگنالها ();
      ret = fsync (fd);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from ftruncate.  */

ساكن صحيح
iftruncate (صحيح fd, off_t درازی)
{
  صحيح ret;

  افعل
    {
      process_سیگنالها ();
      ret = ftruncate (fd, درازی);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_خالص
ساكن bool
عملوند_matches (حرف ثابت *str, حرف ثابت *الگو_2, حرف delim)
{
  طالما (*الگو_2)
    إذا (*str++ != *الگو_2++)
      أرجع false;
  أرجع !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ساكن صحيح
parse_symbols (حرف ثابت *str, بنية نماد_مقدار ثابت *جدول,
               bool exclusive, حرف ثابت *error_msgid)
{
  صحيح مقدار = 0;

  طالما (true)
    {
      حرف ثابت *strcomma = strchr (str, ',');
      بنية نماد_مقدار ثابت *مدخل;

      لكل (مدخل = جدول;
           ! (عملوند_matches (str, مدخل->نماد, ',') && مدخل->مقدار);
           مدخل++)
        {
          إذا (! مدخل->نماد[0])
            {
              idx_t slen = strcomma ? strcomma - str : طول_رشته (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      إذا (exclusive)
        مقدار = مدخل->مقدار;
      وإلا
        مقدار |= مدخل->مقدار;
      إذا (!strcomma)
        اكسر;
      str = strcomma + 1;
    }

  أرجع مقدار;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

ساكن intmax_t
parse_integer (حرف ثابت *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  صحيح indeterminate = 0;
  ساكن حرف ثابت suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t نتیجه = 1;
  bool سرریز = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  طالما (true)
    {
      uintmax_t n = indeterminate;
      حرف *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      إذا ((e & ~LONGINT_سرریز) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          إذا (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_سرریز) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      إذا (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      إذا ((e & ~LONGINT_سرریز) != LONGINT_OK)
        {
          إذا (! (e & LONGINT_سرریز) && n <= INTMAX_MAX)
            {
              *invalid = e;
              أرجع indeterminate;
            }
          e = LONGINT_سرریز;
        }

      إذا (n == 0)
        نتیجه = 0;
      وإلا إذا (نتیجه != 0)
        {
          intmax_t حاصلضرب;
          إذا ((e & LONGINT_سرریز)
              || سرریز
              || ckd_mul (&حاصلضرب, نتیجه, n))
            سرریز = true;
          وإلا
            نتیجه = حاصلضرب;
        }

      إذا (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      إذا (! multiply)
        اكسر;
      str = suffix + 1;
    }

  إذا (نتیجه == 0)
    {
      إذا (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  وإلا إذا (سرریز)
    {
      e = LONGINT_سرریز;
      نتیجه = INTMAX_MAX;
    }

  *invalid = e;
  أرجع نتیجه;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_خالص
ساكن bool
عملوند_is (حرف ثابت *عملوند_2, حرف ثابت *name)
{
  أرجع عملوند_matches (عملوند_2, name, '=');
}

ساكن فراغ
scanargs (صحيح argc, حرف *ثابت *argv)
{
  idx_t blocksize = 0;
  intmax_t شمار = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool شمار_B = false, skip_B = false, seek_B = false;

  لكل (صحيح i = optind; i < argc; i++)
    {
      حرف ثابت *name = argv[i];
      حرف ثابت *val = strchr (name, '=');

      إذا (val == عدم)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      إذا (عملوند_is (name, "if"))
        درون_داد_file = val;
      وإلا إذا (عملوند_is (name, "of"))
        output_file = val;
      وإلا إذا (عملوند_is (name, "conv"))
        conversions_ماسک |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      وإلا إذا (عملوند_is (name, "iflag"))
        درون_داد_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      وإلا إذا (عملوند_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      وإلا إذا (عملوند_is (name, "status"))
        وضعیت_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      وإلا
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = عدم;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          إذا (عملوند_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &درون_داد_blocksize;
            }
          وإلا إذا (عملوند_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          وإلا إذا (عملوند_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          وإلا إذا (عملوند_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          وإلا إذا (عملوند_is (name, "skip") || عملوند_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          وإلا إذا (عملوند_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          وإلا إذا (عملوند_is (name, "count"))
            {
              شمار = n;
              شمار_B = has_B;
            }
          وإلا
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          إذا (n < n_min)
            invalid = LONGINT_INVALID;
          وإلا إذا (n_max < n)
            invalid = LONGINT_سرریز;

          إذا (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_سرریز ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          وإلا إذا (converted_idx)
            *converted_idx = n;
        }
    }

  إذا (blocksize)
    درون_داد_blocksize = output_blocksize = blocksize;
  وإلا
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_ماسک |= C_TWOBUFS;
    }

  إذا (درون_داد_blocksize == 0)
    درون_داد_blocksize = DEFAULT_BLOCKSIZE;
  إذا (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  إذا (conversion_blocksize == 0)
    conversions_ماسک &= ~(C_بلوک | C_UNBLOCK);

  إذا (درون_داد_flags & (O_DSYNC | O_SYNC))
    درون_داد_flags |= O_RSYNC;

  إذا (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  إذا (skip_B)
    درون_داد_flags |= O_SKIP_بایتها;
  إذا (درون_داد_flags & O_SKIP_بایتها && skip != 0)
    {
      skip_رکوردها = skip / درون_داد_blocksize;
      skip_بایتها = skip % درون_داد_blocksize;
    }
  وإلا إذا (skip != 0)
    skip_رکوردها = skip;

  إذا (شمار_B)
    درون_داد_flags |= O_شمار_بایتها;
  إذا (درون_داد_flags & O_شمار_بایتها && شمار != INTMAX_MAX)
    {
      max_رکوردها = شمار / درون_داد_blocksize;
      max_بایتها = شمار % درون_داد_blocksize;
    }
  وإلا إذا (شمار != INTMAX_MAX)
    max_رکوردها = شمار;

  إذا (seek_B)
    output_flags |= O_SEEK_بایتها;
  إذا (output_flags & O_SEEK_بایتها && seek != 0)
    {
      seek_رکوردها = seek / output_blocksize;
      seek_بایتها = seek % output_blocksize;
    }
  وإلا إذا (seek != 0)
    seek_رکوردها = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_ماسک & C_TWOBUFS) && ! (درون_داد_flags & O_FULLBLOCK)
     && (skip_رکوردها
         || (0 < max_رکوردها && max_رکوردها < INTMAX_MAX)
         || (درون_داد_flags | output_flags) & O_مستقیم));

  iread_fnc = ((درون_داد_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  درون_داد_flags &= ~O_FULLBLOCK;

  إذا (مضرب_بیتها_مجموعه_2 (conversions_ماسک & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  إذا (مضرب_بیتها_مجموعه_2 (conversions_ماسک & (C_بلوک | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  إذا (مضرب_بیتها_مجموعه_2 (conversions_ماسک & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  إذا (مضرب_بیتها_مجموعه_2 (conversions_ماسک & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  إذا (مضرب_بیتها_مجموعه_2 (درون_داد_flags & (O_مستقیم | O_NOCACHE))
      || مضرب_بیتها_مجموعه_2 (output_flags & (O_مستقیم | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  إذا (درون_داد_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_رکوردها == 0 && max_بایتها == 0);
      درون_داد_flags &= ~O_NOCACHE;
    }
  إذا (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_رکوردها == 0 && max_بایتها == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ساكن فراغ
apply_translations (فراغ)
{
  إذا (conversions_ماسک & C_ASCII)
    translate_charset (ebcdic_مقصد_ascii);

  إذا (conversions_ماسک & C_UCASE)
    {
      لكل (صحيح i = 0; i < 256; i++)
        trans_جدول[i] = به_بزرگ (trans_جدول[i]);
      translation_needed = true;
    }
  وإلا إذا (conversions_ماسک & C_LCASE)
    {
      لكل (صحيح i = 0; i < 256; i++)
        trans_جدول[i] = tolower (trans_جدول[i]);
      translation_needed = true;
    }

  إذا (conversions_ماسک & C_EBCDIC)
    {
      translate_charset (ascii_مقصد_ebcdic);
      newline_character = ascii_مقصد_ebcdic['\n'];
      space_character = ascii_مقصد_ebcdic[' '];
    }
  وإلا إذا (conversions_ماسک & C_IBM)
    {
      translate_charset (ascii_مقصد_ibm);
      newline_character = ascii_مقصد_ibm['\n'];
      space_character = ascii_مقصد_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ساكن فراغ
translate_میانگیر (حرف *buf, idx_t nread)
{
  حرف *cp = buf;
  لكل (idx_t i = nread; i; i--, cp++)
    *cp = trans_جدول[مقصد_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ساكن حرف *
swab_میانگیر (حرف *buf, idx_t *nread, صحيح *saved_byte)
{
  إذا (*nread == 0)
    أرجع buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  صحيح prev_saved = *saved_byte;
  إذا ((prev_saved < 0) == (*nread & 1))
    {
      بیعلامت حرف c = buf[--*nread];
      *saved_byte = c;
    }
  وإلا
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  لكل (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  إذا (prev_saved < 0)
    أرجع buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  أرجع buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

ساكن فراغ
advance_درون_داد_جابه_جایی (intmax_t جابه_جایی)
{
  إذا (0 <= درون_داد_جابه_جایی && ckd_جمع_کن (&درون_داد_جابه_جایی, درون_داد_جابه_جایی, جابه_جایی))
    درون_داد_جابه_جایی = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

ساكن intmax_t
skip (صحيح fdesc, حرف ثابت *file, intmax_t رکوردها, idx_t blocksize,
      idx_t *بایتها)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t جابه_جایی;
  إذا (! ckd_mul (&جابه_جایی, رکوردها, blocksize)
      && ! ckd_جمع_کن (&جابه_جایی, جابه_جایی, *بایتها)
      && 0 <= lseek (fdesc, جابه_جایی, SEEK_CUR))
    {
      إذا (fdesc == STDIN_FILENO)
        {
           بنية stat st;
           إذا (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           إذا (usable_st_اندازه (&st) && 0 < st.st_اندازه && 0 <= درون_داد_جابه_جایی
               && st.st_اندازه - درون_داد_جابه_جایی < جابه_جایی)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               رکوردها = ( جابه_جایی - st.st_اندازه ) / blocksize;
               جابه_جایی = st.st_اندازه - درون_داد_جابه_جایی;
             }
           وإلا
             رکوردها = 0;
           advance_درون_داد_جابه_جایی (جابه_جایی);
        }
      وإلا
        {
          رکوردها = 0;
          *بایتها = 0;
        }
      أرجع رکوردها;
    }
  وإلا
    {
      صحيح lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      إذا (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          إذا (!lseek_errno)
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
      إذا (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      وإلا
        {
          alloc_obuf ();
          buf = obuf;
        }

      افعل
        {
          ssize_t nread = iread_fnc (fdesc, buf, رکوردها ? blocksize : *بایتها);
          إذا (nread < 0)
            {
              إذا (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  إذا (conversions_ماسک & C_NOERROR)
                    print_stats ();
                }
              وإلا
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          وإلا إذا (nread == 0)
            اكسر;
          وإلا إذا (fdesc == STDIN_FILENO)
            advance_درون_داد_جابه_جایی (nread);

          إذا (رکوردها != 0)
            رکوردها--;
          وإلا
            *بایتها = 0;
        }
      طالما (رکوردها || *بایتها);

      أرجع رکوردها;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ساكن bool
advance_درون_داد_after_read_error (idx_t nbytes)
{
  إذا (! درون_داد_seekable)
    {
      إذا (درون_داد_seek_errno == ESPIPE)
        أرجع true;
      errno = درون_داد_seek_errno;
    }
  وإلا
    {
      off_t جابه_جایی;
      advance_درون_داد_جابه_جایی (nbytes);
      إذا (درون_داد_جابه_جایی < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (درون_داد_file));
          أرجع false;
        }
      جابه_جایی = lseek (STDIN_FILENO, 0, SEEK_CUR);
      إذا (0 <= جابه_جایی)
        {
          off_t diff;
          إذا (جابه_جایی == درون_داد_جابه_جایی)
            أرجع true;
          diff = درون_داد_جابه_جایی - جابه_جایی;
          إذا (! (0 <= diff && diff <= nbytes) && وضعیت_level != وضعیت_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          إذا (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            أرجع true;
          إذا (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (درون_داد_file));
  أرجع false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ساكن فراغ
copy_simple (حرف ثابت *buf, idx_t nread)
{
  حرف ثابت *شروع = buf;	/* First uncopied char in BUF.  */

  افعل
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      کپی_حافظه (obuf + oc, شروع, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      شروع += nfree;
      oc += nfree;
      إذا (oc >= output_blocksize)
        write_output ();
    }
  طالما (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

ساكن فراغ
copy_with_بلوک (حرف ثابت *buf, idx_t nread)
{
  لكل (idx_t i = nread; i; i--, buf++)
    {
      إذا (*buf == newline_character)
        {
          إذا (ستون < conversion_blocksize)
            {
              لكل (idx_t j = ستون; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          ستون = 0;
        }
      وإلا
        {
          إذا (ستون == conversion_blocksize)
            r_truncate++;
          وإلا إذا (ستون < conversion_blocksize)
            output_char (*buf);
          ستون++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

ساكن فراغ
copy_with_unblock (حرف ثابت *buf, idx_t nread)
{
  ساكن idx_t pending_spaces = 0;

  لكل (idx_t i = 0; i < nread; i++)
    {
      حرف c = buf[i];

      إذا (ستون++ >= conversion_blocksize)
        {
          ستون = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      وإلا إذا (c == space_character)
        pending_spaces++;
      وإلا
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          طالما (pending_spaces)
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

ساكن فراغ
مجموعه_fd_flags (صحيح fd, صحيح جمع_کن_flags, حرف ثابت *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  جمع_کن_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  إذا (جمع_کن_flags)
    {
      صحيح old_flags = fcntl (fd, F_GETFL);
      صحيح new_flags = old_flags | جمع_کن_flags;
      bool ok = true;
      إذا (old_flags < 0)
        ok = false;
      وإلا إذا (old_flags != new_flags)
        {
          إذا (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              بنية stat st;
              إذا (ifstat (fd, &st) != 0)
                ok = false;
              وإلا إذا ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_نما))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              وإلا إذا ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          إذا (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      إذا (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

ساكن صحيح
dd_copy (فراغ)
{
  حرف *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  صحيح exit_وضعیت = EXIT_SUCCESS;
  idx_t n_بایتها_read;

  إذا (skip_رکوردها != 0 || skip_بایتها != 0)
    {
      intmax_t us_بایتها;
      bool us_بایتها_سرریز =
        (ckd_mul (&us_بایتها, skip_رکوردها, درون_داد_blocksize)
         || ckd_جمع_کن (&us_بایتها, skip_بایتها, us_بایتها));
      off_t درون_داد_offset0 = درون_داد_جابه_جایی;
      intmax_t us_بلوکها = skip (STDIN_FILENO, درون_داد_file,
                                 skip_رکوردها, درون_داد_blocksize, &skip_بایتها);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      إذا ((us_بلوکها
           || (0 <= درون_داد_جابه_جایی
               && (us_بایتها_سرریز
                   || us_بایتها != درون_داد_جابه_جایی - درون_داد_offset0)))
          && وضعیت_level != وضعیت_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (درون_داد_file));
        }
    }

  إذا (seek_رکوردها != 0 || seek_بایتها != 0)
    {
      idx_t بایتها = seek_بایتها;
      intmax_t write_رکوردها = skip (STDOUT_FILENO, output_file,
                                      seek_رکوردها, output_blocksize, &بایتها);

      إذا (write_رکوردها != 0 || بایتها != 0)
        {
          memset (obuf, 0, write_رکوردها ? output_blocksize : بایتها);

          افعل
            {
              idx_t اندازه = write_رکوردها ? output_blocksize : بایتها;
              إذا (iwrite (STDOUT_FILENO, obuf, اندازه) != اندازه)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              إذا (write_رکوردها != 0)
                write_رکوردها--;
              وإلا
                بایتها = 0;
            }
          طالما (write_رکوردها || بایتها);
        }
    }

  إذا (max_رکوردها == 0 && max_بایتها == 0)
    أرجع exit_وضعیت;

  alloc_ibuf ();
  alloc_obuf ();
  صحيح saved_byte = -1;

  طالما (true)
    {
      إذا (وضعیت_level == وضعیت_پیشرفت)
        {
          xtime_t پیشرفت_زمان = gethrxtime ();
          إذا (بعدی_زمان <= پیشرفت_زمان)
            {
              print_xfer_stats (پیشرفت_زمان);
              بعدی_زمان += XTIME_PRECISION;
            }
        }

      إذا (r_partial + r_پر >= max_رکوردها + !!max_بایتها)
        اكسر;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      إذا ((conversions_ماسک & C_SYNC) && (conversions_ماسک & C_NOERROR))
        memset (ibuf,
                (conversions_ماسک & (C_بلوک | C_UNBLOCK)) ? ' ' : '\0',
                درون_داد_blocksize);

      إذا (r_partial + r_پر >= max_رکوردها)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_بایتها);
      وإلا
        nread = iread_fnc (STDIN_FILENO, ibuf, درون_داد_blocksize);

      إذا (nread > 0)
        {
          advance_درون_داد_جابه_جایی (nread);
          إذا (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      وإلا إذا (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_ماسک & C_NOTRUNC);
          اكسر;			/* EOF.  */
        }
      وإلا
        {
          إذا (!(conversions_ماسک & C_NOERROR) || وضعیت_level != وضعیت_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (درون_داد_file));

          إذا (conversions_ماسک & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = درون_داد_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              إذا (!advance_درون_داد_after_read_error (bad_portion))
                {
                  exit_وضعیت = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  درون_داد_seekable = false;
                  درون_داد_seek_errno = ESPIPE;
                }
              إذا ((conversions_ماسک & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              وإلا
                استمر;
            }
          وإلا
            {
              /* Write any partial block. */
              exit_وضعیت = EXIT_FAILURE;
              اكسر;
            }
        }

      n_بایتها_read = nread;

      إذا (n_بایتها_read < درون_داد_blocksize)
        {
          r_partial++;
          partread = n_بایتها_read;
          إذا (conversions_ماسک & C_SYNC)
            {
              إذا (!(conversions_ماسک & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_بایتها_read,
                        (conversions_ماسک & (C_بلوک | C_UNBLOCK)) ? ' ' : '\0',
                        درون_داد_blocksize - n_بایتها_read);
              n_بایتها_read = درون_داد_blocksize;
            }
        }
      وإلا
        {
          r_پر++;
          partread = 0;
        }

      إذا (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_بایتها_read);
          w_بایتها += nwritten;
          إذا (nwritten != n_بایتها_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              إذا (nwritten != 0)
                w_partial++;
              أرجع EXIT_FAILURE;
            }
          وإلا إذا (n_بایتها_read == درون_داد_blocksize)
            w_پر++;
          وإلا
            w_partial++;
          استمر;
        }

      /* Do any translations on the whole buffer at once.  */

      إذا (translation_needed)
        translate_میانگیر (ibuf, n_بایتها_read);

      إذا (conversions_ماسک & C_SWAB)
        bufstart = swab_میانگیر (ibuf, &n_بایتها_read, &saved_byte);
      وإلا
        bufstart = ibuf;

      إذا (conversions_ماسک & C_بلوک)
        copy_with_بلوک (bufstart, n_بایتها_read);
      وإلا إذا (conversions_ماسک & C_UNBLOCK)
        copy_with_unblock (bufstart, n_بایتها_read);
      وإلا
        copy_simple (bufstart, n_بایتها_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  إذا (0 <= saved_byte)
    {
      حرف saved_char = saved_byte;
      إذا (conversions_ماسک & C_بلوک)
        copy_with_بلوک (&saved_char, 1);
      وإلا إذا (conversions_ماسک & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      وإلا
        output_char (saved_char);
    }

  إذا ((conversions_ماسک & C_بلوک) && ستون > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      لكل (idx_t i = ستون; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  إذا (ستون && (conversions_ماسک & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  إذا (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_بایتها += nwritten;
      إذا (nwritten != 0)
        w_partial++;
      إذا (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          أرجع EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  إذا (final_op_was_seek)
    {
      بنية stat stdout_stat;
      إذا (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          أرجع EXIT_FAILURE;
        }
      إذا (S_ISREG (stdout_stat.st_نما) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_جابه_جایی = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          إذا (0 <= output_جابه_جایی && stdout_stat.st_اندازه < output_جابه_جایی)
            {
              إذا (iftruncate (STDOUT_FILENO, output_جابه_جایی) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_جابه_جایی}, quoteaf (output_file));
                  أرجع EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  إذا (conversions_ماسک & (C_FDATASYNC | C_FSYNC)
      && وضعیت_level == وضعیت_پیشرفت
      && 0 <= reported_w_بایتها && reported_w_بایتها < w_بایتها)
    print_xfer_stats (0);

  أرجع exit_وضعیت;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ساكن صحيح
synchronize_output (فراغ)
{
  صحيح exit_وضعیت = 0;
  صحيح ماسک = conversions_ماسک;
  conversions_ماسک &= ~ (C_FDATASYNC | C_FSYNC);

  إذا ((ماسک & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      إذا (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_وضعیت = EXIT_FAILURE;
        }
      ماسک |= C_FSYNC;
    }

  إذا ((ماسک & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      أرجع EXIT_FAILURE;
    }

  أرجع exit_وضعیت;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  off_t جابه_جایی;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  صفحه_اندازه = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, برنامه_NAME, PACKAGE, نسخه,
                                   true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  لكل (صحيح i = 0; i < 256; i++)
    trans_جدول[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  إذا (درون_داد_file == عدم)
    {
      درون_داد_file = _("standard input");
      مجموعه_fd_flags (STDIN_FILENO, درون_داد_flags, درون_داد_file);
    }
  وإلا
    {
      إذا (ifd_reopen (STDIN_FILENO, درون_داد_file, O_RDONLY | درون_داد_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (درون_داد_file));
    }

  جابه_جایی = lseek (STDIN_FILENO, 0, SEEK_CUR);
  درون_داد_seekable = (0 <= جابه_جایی);
  درون_داد_جابه_جایی = MAX (0, جابه_جایی);
  درون_داد_seek_errno = errno;

  إذا (output_file == عدم)
    {
      output_file = _("standard output");
      مجموعه_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  وإلا
    {
      mode_t perms = نما_RW_UGO;
      صحيح opts
        = (output_flags
           | (conversions_ماسک & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_ماسک & C_EXCL ? O_EXCL : 0)
           | (seek_رکوردها || (conversions_ماسک & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t اندازه;
      إذا ((ckd_mul (&اندازه, seek_رکوردها, output_blocksize)
           || ckd_جمع_کن (&اندازه, seek_بایتها, اندازه))
          && !(conversions_ماسک & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_رکوردها, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      إذا ((! seek_رکوردها
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      إذا (seek_رکوردها != 0 && !(conversions_ماسک & C_NOTRUNC))
        {
          إذا (iftruncate (STDOUT_FILENO, اندازه) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              صحيح ftruncate_errno = errno;
              بنية stat stdout_stat;
              إذا (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              وإلا إذا (S_ISREG (stdout_stat.st_نما)
                       || S_ISDIR (stdout_stat.st_نما)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = اندازه;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  شروع_زمان = gethrxtime ();
  بعدی_زمان = شروع_زمان + XTIME_PRECISION;

  صحيح copy_وضعیت = dd_copy ();
  صحيح sync_وضعیت = synchronize_output ();
  صحيح exit_وضعیت = copy_وضعیت | sync_وضعیت;

  إذا (max_رکوردها == 0 && max_بایتها == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      إذا (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (درون_داد_file));
          exit_وضعیت = EXIT_FAILURE;
        }
      إذا (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_وضعیت = EXIT_FAILURE;
        }
    }
  وإلا
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      إذا (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      إذا (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  پایان_به_بالا ();
  main_exit (exit_وضعیت);
}
