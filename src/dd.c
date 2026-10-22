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
#وصف_ڪريو پروگرام_NAME "dd"

#وصف_ڪريو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#جيڪڏهن_وصف_نه_ٿيل SA_RESETHAND
# define SA_RESETHAND 0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل SIGINFO
# define SIGINFO SIGUSR1
#جيڪڏهن_پورو

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#جيڪڏهن_وصف_نه_ٿيل O_CIO
# define O_CIO 0
#جيڪڏهن_پورو

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#وصف_رد O_NOCACHE

#وصف_ڪريو output_char(c)				\
  افعل						\
    {						\
      obuf[oc++] = (c);				\
      إذا (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  طالما (0)

/* Default input and output blocksize. */
#وصف_ڪريو DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
تعداد
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_بلاڪ = 010,
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
    حالت_NONE = 1,
    حالت_NOXFER = 2,
    حالت_DEFAULT = 3,
    حالت_ترقي = 4
  };

/* The name of the input file, or NULL for the standard input. */
ساكن حرف ثابت *داخلا_file = عدم;

/* The name of the output file, or NULL for the standard output. */
ساكن حرف ثابت *output_file = عدم;

/* The page size on this host.  */
ساكن idx_t صفحو_سائيز;

/* The number of bytes in which atomic reads are done. */
ساكن idx_t داخلا_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ساكن idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ساكن idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ساكن intmax_t skip_رڪارڊون = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ساكن idx_t skip_بائيٽون = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ساكن intmax_t seek_رڪارڊون = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ساكن intmax_t seek_بائيٽون = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ساكن bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ساكن intmax_t max_رڪارڊون = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ساكن idx_t max_بائيٽون = 0;

/* Bit vector of conversions to apply. */
ساكن صحيح conversions_نقاب = 0;

/* Open flags for the input and output files.  */
ساكن صحيح داخلا_flags = 0;
ساكن صحيح output_flags = 0;

/* Status flags for what is printed to stderr.  */
ساكن صحيح حالت_level = حالت_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ساكن bool translation_needed = false;

/* Number of partial blocks written. */
ساكن intmax_t w_partial = 0;

/* Number of full blocks written. */
ساكن intmax_t w_ڀريل = 0;

/* Number of partial blocks read. */
ساكن intmax_t r_partial = 0;

/* Number of full blocks read. */
ساكن intmax_t r_ڀريل = 0;

/* Number of bytes written.  */
ساكن intmax_t w_بائيٽون = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ساكن intmax_t reported_w_بائيٽون = -1;

/* Time that dd started.  */
ساكن xtime_t شروعات_وقت;

/* Next time to report periodic progress.  */
ساكن xtime_t ايندڙ_وقت;

/* If positive, the number of bytes output in the current progress line.  */
ساكن صحيح ترقي_len;

/* True if input is seekable.  */
ساكن bool داخلا_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ساكن صحيح داخلا_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ساكن off_t داخلا_وٿي;

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
ساكن idx_t کانو = 0;

/* The set of signals that are caught.  */
ساكن sigset_t caught_سگنل;

/* If nonzero, the value of the pending fatal signal.  */
ساكن sig_atomic_t متطاير مداخلت_signal;

/* A count of the number of pending info signals that have been received.  */
ساكن sig_atomic_t متطاير info_signal_ڳڻپ;

/* Whether to discard cache for input or output.  */
ساكن bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ساكن bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ساكن ssize_t (*iread_fnc) (صحيح fd, حرف *buf, idx_t سائيز);

/* A longest symbol in the struct symbol_values tables below.  */
#وصف_ڪريو LONGEST_نشان "count_bytes"

/* A symbol and the corresponding integer value.  */
بنية نشان_قدر
{
  حرف نشان[حجم LONGEST_نشان];
  صحيح قدر;
};

/* Conversion symbols, for conv="...".  */
ساكن بنية نشان_قدر ثابت conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_بلاڪ | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_بلاڪ | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_بلاڪ | C_TWOBUFS},	/* Variable to fixed length records. */
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

#وصف_ڪريو FFS_نقاب(x) ((x) ^ ((x) & ((x) - 1)))
تعداد
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ٻٽو
          | O_CIO
          | O_سڌو
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

    O_ڳڻپ_بائيٽون = FFS_نقاب (v3),
    v4 = v3 ^ O_ڳڻپ_بائيٽون,

    O_SKIP_بائيٽون = FFS_نقاب (v4),
    v5 = v4 ^ O_SKIP_بائيٽون,

    O_SEEK_بائيٽون = FFS_نقاب (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_ڳڻپ_بائيٽون != 0);
static_assert (O_SKIP_بائيٽون != 0);
static_assert (O_SEEK_بائيٽون != 0);

#وصف_ڪريو گڻو_بٽ_سيٽ(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! گڻو_بٽ_سيٽ (O_FULLBLOCK));
static_assert ( ! گڻو_بٽ_سيٽ (O_NOCACHE));
static_assert ( ! گڻو_بٽ_سيٽ (O_ڳڻپ_بائيٽون));
static_assert ( ! گڻو_بٽ_سيٽ (O_SKIP_بائيٽون));
static_assert ( ! گڻو_بٽ_سيٽ (O_SEEK_بائيٽون));

/* Flags, for iflag="..." and oflag="...".  */
ساكن بنية نشان_قدر ثابت flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ٻٽو},
  {"cio",	  O_CIO},
  {"direct",	  O_سڌو},
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
  {"count_bytes", O_ڳڻپ_بائيٽون},
  {"skip_bytes",  O_SKIP_بائيٽون},
  {"seek_bytes",  O_SEEK_بائيٽون},
  {"",		0}
};

/* Status, for status="...".  */
ساكن بنية نشان_قدر ثابت statuses[] =
{
  {"none",	حالت_NONE},
  {"noxfer",	حالت_NOXFER},
  {"progress",	حالت_ترقي},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ساكن بنانشان حرف trans_جدول[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ساكن حرف ثابت ascii_منزل_ebcdic[] =
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

ساكن حرف ثابت ascii_منزل_ibm[] =
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

ساكن حرف ثابت ebcdic_منزل_ascii[] =
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
  وإلا إذا (close_وهڪرو (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ساكن فراغ
diagnose (صحيح errnum, حرف ثابت *fmt, ...)
{
  إذا (0 < ترقي_len)
    {
      fputc ('\n', stderr);
      ترقي_len = 0;
    }

  va_list ap;
  va_شروعات (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_پڇاڙي (ap);
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [عمليو]...\n\
  or:  %s OPTION\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Copy a file, converting and formatting according منزل the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=بائيٽون_2        اقرأ and اكتب مٿي منزل بائيٽون_2 بائيٽون at a وقت (افتراضي: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=بائيٽون_2       convert بائيٽون_2 بائيٽون at a وقت\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as في the comma separated نشان list\n\
"));
      oputs (_("\
  ڳڻپ=N         copy only N داخلا بلاڪون\n\
"));
      oputs (_("\
  ibs=بائيٽون_2       اقرأ مٿي منزل بائيٽون_2 بائيٽون at a وقت (افتراضي: 512)\n\
"));
      oputs (_("\
  إذا=FILE         اقرأ from FILE instead جو standard داخلا\n\
"));
      oputs (_("\
  iflag=FLAGS     اقرأ as في the comma separated نشان list\n\
"));
      oputs (_("\
  obs=بائيٽون_2       اكتب بائيٽون_2 بائيٽون at a وقت (افتراضي: 512)\n\
"));
      oputs (_("\
  جو=FILE         اكتب منزل FILE instead جو standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     اكتب as في the comma separated نشان list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output بلاڪون\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized داخلا بلاڪون\n\
"));
      oputs (_("\
  حالت=LEVEL    The LEVEL جو information منزل print منزل standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and بائيٽون_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on لكل T, P, E, Z, Y, R, Q.\n\
ٻٽو_2 prefixes can be استعمال_ٿيل, too: KiB=K, MiB=M, and so on.\n\
If N پڇاڙيون اندريون 'B', it counts بائيٽون not بلاڪون.\n\
\n\
Each CONV نشان may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC منزل ASCII\n\
  ebcdic    from ASCII منزل EBCDIC\n\
  ibm       from ASCII منزل alternate EBCDIC\n\
  بلاڪ     pad newline-terminated رڪارڊون with spaces منزل cbs-سائيز\n\
  unblock   replace trailing spaces اندريون cbs-سائيز رڪارڊون with newline\n\
  lcase     change مٿانهون حالة منزل هيٺاهون حالة\n\
  ucase     change هيٺاهون حالة منزل مٿانهون حالة\n\
  sparse    try منزل seek rather than اكتب all-NUL output بلاڪون\n\
  swab      مٽا_سٽا every pair جو داخلا بائيٽون\n\
  sync      pad every داخلا بلاڪ with NULs منزل ibs-سائيز; when استعمال_ٿيل\n\
            with بلاڪ or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail إذا the output file already exists\n\
  nocreat   افعل not create the output file\n\
  notrunc   افعل not truncate the output file\n\
  noerror   استمر after اقرأ errors\n\
  fdatasync  physically اكتب output file معلومات before finishing\n\
  fsync     likewise, but also اكتب metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG نشان may be:\n\
\n\
  append    append گهڻائي_2 (makes sense only لكل output; conv=notrunc suggested)\n\
"), stdout);
      إذا (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      إذا (O_سڌو)
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
#جيڪڏهن_پورو
      إذا (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      إذا (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      إذا (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      إذا (O_ٻٽو)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      إذا (O_متن)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        لکو (_("\
\n\
Sending a %s إشارة منزل a running 'dd' process makes it\n\
print I/O statistics منزل standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Common options to use when displaying sizes and rates.  */

تعداد { human_opts = (human_autoscale | human_ڦيرو_منزل_سڀ_کان_ويجهو
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ساكن فراغ
alloc_ibuf (فراغ)
{
  إذا (ibuf)
    أرجع;

  bool extra_byte_for_swab = !!(conversions_نقاب & C_SWAB);
  ibuf = alignalloc (صفحو_سائيز, داخلا_blocksize + extra_byte_for_swab);
  إذا (!ibuf)
    {
      حرف hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             داخلا_blocksize,
             human_readable (داخلا_blocksize, hbuf,
                             human_opts | human_بنياد_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ساكن فراغ
alloc_obuf (فراغ)
{
  إذا (obuf)
    أرجع;

  إذا (conversions_نقاب & C_TWOBUFS)
    {
      obuf = alignalloc (صفحو_سائيز, output_blocksize);
      إذا (!obuf)
        {
          حرف hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_بنياد_1024, 1, 1));
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
گڻو_بٽ_سيٽ_2 (صحيح i)
{
  أرجع گڻو_بٽ_سيٽ (i);
}

ساكن bool
abbreviation_lacks_اڳياڙي (حرف ثابت *پيغام_2)
{
  أرجع پيغام_2[لڙي_ڊيگهه (پيغام_2) - 2] == ' ';
}

/* Print transfer statistics.  */

ساكن فراغ
print_xfer_stats (xtime_t ترقي_وقت)
{
  xtime_t now = ترقي_وقت ? ترقي_وقت : gethrxtime ();
  ساكن حرف ثابت slash_s[] = "/s";
  حرف hbuf[3][LONGEST_HUMAN_READABLE + حجم slash_s];
  مزدوج delta_s;
  حرف ثابت *بائيٽون_في_ٻيو;
  حرف ثابت *si = human_readable (w_بائيٽون, hbuf[0], human_opts, 1, 1);
  حرف ثابت *iec = human_readable (w_بائيٽون, hbuf[1],
                                    human_opts | human_بنياد_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  حرف *bpsbuf = hbuf[2];
  صحيح bpsbufsize = حجم hbuf[2];
  إذا (شروعات_وقت < now)
    {
      مزدوج XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - شروعات_وقت;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      بائيٽون_في_ٻيو = human_readable (w_بائيٽون, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (بائيٽون_في_ٻيو - bpsbuf + bpsbuf, slash_s);
    }
  وإلا
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      بائيٽون_في_ٻيو = bpsbuf;
    }

  إذا (ترقي_وقت)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  حرف delta_s_buf[24];
  snprintf (delta_s_buf, حجم delta_s_buf,
            ترقي_وقت ? "%.0f s" : "%g s", delta_s);

  صحيح stats_len
    = (abbreviation_lacks_اڳياڙي (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_بائيٽون)),
                  w_بائيٽون, delta_s_buf, بائيٽون_في_ٻيو)
       : abbreviation_lacks_اڳياڙي (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_بائيٽون, si, delta_s_buf, بائيٽون_في_ٻيو)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_بائيٽون, si, iec, delta_s_buf, بائيٽون_في_ٻيو));

  إذا (ترقي_وقت)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      إذا (0 <= stats_len && stats_len < ترقي_len)
        fprintf (stderr, "%*s", ترقي_len - stats_len, "");
      ترقي_len = stats_len;
    }
  وإلا
    fputc ('\n', stderr);

  reported_w_بائيٽون = w_بائيٽون;
}

ساكن فراغ
print_stats (فراغ)
{
  إذا (حالت_level == حالت_NONE)
    أرجع;

  إذا (0 < ترقي_len)
    {
      fputc ('\n', stderr);
      ترقي_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_ڀريل, r_partial, w_ڀريل, w_partial);

  إذا (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  إذا (حالت_level == حالت_NOXFER)
    أرجع;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ساكن فراغ
مداخلت_handler (صحيح sig)
{
  إذا (! SA_RESETHAND)
    إشارة (sig, SIG_DFL);
  مداخلت_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ساكن فراغ
siginfo_handler (MAYBE_اڻ_استعمال_ٿيل صحيح sig)
{
  info_signal_ڳڻپ++;
}

/* Install the signal handlers.  */

ساكن فراغ
install_signal_handlers (فراغ)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  بنية sigaction act;
  sigemptyset (&caught_سگنل);
  إذا (catch_siginfo)
    sigaddset (&caught_سگنل, SIGINFO);
  sigaction (SIGINT, عدم, &act);
  إذا (act.sa_handler != SIG_IGN)
    sigaddset (&caught_سگنل, SIGINT);
  act.sa_mask = caught_سگنل;

  إذا (sigismember (&caught_سگنل, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, عدم);
    }

  إذا (sigismember (&caught_سگنل, SIGINT))
    {
      act.sa_handler = مداخلت_handler;
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
  إذا (!مداخلت_signal)
    {
      صحيح sync_حالت = synchronize_output ();
      إذا (sync_حالت)
        ٻاهر_نڪرو (sync_حالت);
    }

  إذا (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (داخلا_file));

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
process_سگنل (فراغ)
{
  طالما (مداخلت_signal || info_signal_ڳڻپ)
    {
      صحيح مداخلت;
      صحيح infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_سگنل, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      مداخلت = مداخلت_signal;
      infos = info_signal_ڳڻپ;

      إذا (infos)
        info_signal_ڳڻپ = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, عدم);

      إذا (مداخلت)
        cleanup ();
      print_stats ();
      إذا (مداخلت)
        ارفع (مداخلت);
    }
}

ساكن فراغ
پڄاڻي_مٿي (فراغ)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_سگنل ();
  cleanup ();
  print_stats ();
}

ساكن فراغ
quit (صحيح ڪوڊ)
{
  پڄاڻي_مٿي ();
  ٻاهر_نڪرو (ڪوڊ);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ساكن off_t
cache_ڦيرو (صحيح fd, off_t len)
{
  ساكن off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  إذا (len)
    {
      intmax_t c_pending;
      إذا (ckd_جوڙيو (&c_pending, *pending, len))
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
  off_t وٿي;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_ڦيرو (fd, len);
  إذا (len && !clen)
    أرجع true; /* Don't advise this time.  */
  وإلا إذا (! len && ! clen && ! nocache_eof)
    أرجع true;
  off_t pending = len ? cache_ڦيرو (fd, 0) : 0;

  إذا (fd == STDIN_FILENO)
    {
      إذا (داخلا_seekable)
        وٿي = داخلا_وٿي;
      وإلا
        {
          وٿي = -1;
          errno = ESPIPE;
        }
    }
  وإلا
    {
      ساكن off_t output_وٿي = -2;

      إذا (output_وٿي != -1)
        {
          إذا (output_وٿي < 0)
            output_وٿي = lseek (fd, 0, SEEK_CUR);
          وإلا إذا (len)
            output_وٿي += clen + pending;
        }

      وٿي = output_وٿي;
    }

  إذا (0 <= وٿي)
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
     وٿي = وٿي - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     إذا (clen == 0)
       وٿي -= وٿي % صفحو_سائيز;
     adv_ret = posix_fadvise (fd, وٿي, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#وإلا
     errno = ENOTSUP;
#جيڪڏهن_پورو
   }

  أرجع adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ساكن ssize_t
iread (صحيح fd, حرف *buf, idx_t سائيز)
{
  ssize_t nread;
  ساكن ssize_t prev_nread;

  افعل
    {
      process_سگنل ();
      nread = اقرأ (fd, buf, سائيز);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      إذا (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < سائيز
          && (داخلا_flags & O_سڌو))
        {
          errno = 0;
          nread = 0;
        }
    }
  طالما (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  إذا (0 < nread && nread < سائيز)
    process_سگنل ();

  إذا (0 < nread && warn_partial_read)
    {
      إذا (0 < prev_nread && prev_nread < سائيز)
        {
          idx_t prev = prev_nread;
          إذا (حالت_level != حالت_NONE)
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
iread_fullblock (صحيح fd, حرف *buf, idx_t سائيز)
{
  ssize_t nread = 0;

  طالما (0 < سائيز)
    {
      ssize_t ncurr = iread (fd, buf, سائيز);
      إذا (ncurr < 0)
        أرجع ncurr;
      إذا (ncurr == 0)
        اكسر;
      nread += ncurr;
      buf   += ncurr;
      سائيز  -= ncurr;
    }

  أرجع nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ساكن idx_t
iwrite (صحيح fd, حرف ثابت *buf, idx_t سائيز)
{
  idx_t ڪل_written = 0;

  إذا ((output_flags & O_سڌو) && سائيز < output_blocksize)
    {
      صحيح old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      إذا (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_سڌو) != 0
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

  طالما (ڪل_written < سائيز)
    {
      ssize_t nwritten = 0;
      process_سگنل ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      إذا ((conversions_نقاب & C_SPARSE) && is_nul (buf, سائيز))
        {
          إذا (lseek (fd, سائيز, SEEK_CUR) < 0)
            {
              conversions_نقاب &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          وإلا
            {
              final_op_was_seek = true;
              nwritten = سائيز;
            }
        }

      إذا (!nwritten)
        nwritten = اكتب (fd, buf + ڪل_written, سائيز - ڪل_written);

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
        ڪل_written += nwritten;
    }

  إذا (o_nocache && ڪل_written)
    invalidate_cache (fd, ڪل_written);

  أرجع ڪل_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ساكن فراغ
write_output (فراغ)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_بائيٽون += nwritten;
  إذا (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      إذا (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  وإلا
    w_ڀريل++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ساكن صحيح
ifdatasync (صحيح fd)
{
  صحيح ret;

  افعل
    {
      process_سگنل ();
      ret = fdatasync (fd);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from fd_reopen.  */

ساكن صحيح
ifd_reopen (صحيح desired_fd, حرف ثابت *file, صحيح flag, mode_t گهڻائي_2)
{
  صحيح ret;

  افعل
    {
      process_سگنل ();
      ret = fd_reopen (desired_fd, file, flag, گهڻائي_2);
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
      process_سگنل ();
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
      process_سگنل ();
      ret = fsync (fd);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Restart on EINTR from ftruncate.  */

ساكن صحيح
iftruncate (صحيح fd, off_t ڊيگهائي)
{
  صحيح ret;

  افعل
    {
      process_سگنل ();
      ret = ftruncate (fd, ڊيگهائي);
    }
  طالما (ret < 0 && errno == EINTR);

  أرجع ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_خالص
ساكن bool
عمليو_matches (حرف ثابت *str, حرف ثابت *نمونو_2, حرف delim)
{
  طالما (*نمونو_2)
    إذا (*str++ != *نمونو_2++)
      أرجع false;
  أرجع !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ساكن صحيح
parse_symbols (حرف ثابت *str, بنية نشان_قدر ثابت *جدول,
               bool exclusive, حرف ثابت *error_msgid)
{
  صحيح قدر = 0;

  طالما (true)
    {
      حرف ثابت *strcomma = strchr (str, ',');
      بنية نشان_قدر ثابت *داخلا_2;

      لكل (داخلا_2 = جدول;
           ! (عمليو_matches (str, داخلا_2->نشان, ',') && داخلا_2->قدر);
           داخلا_2++)
        {
          إذا (! داخلا_2->نشان[0])
            {
              idx_t slen = strcomma ? strcomma - str : لڙي_ڊيگهه (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      إذا (exclusive)
        قدر = داخلا_2->قدر;
      وإلا
        قدر |= داخلا_2->قدر;
      إذا (!strcomma)
        اكسر;
      str = strcomma + 1;
    }

  أرجع قدر;
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
  intmax_t نتيجو = 1;
  bool حد_کان_مٿي = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  طالما (true)
    {
      uintmax_t n = indeterminate;
      حرف *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      إذا ((e & ~LONGINT_حد_کان_مٿي) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          إذا (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_حد_کان_مٿي) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      إذا (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      إذا ((e & ~LONGINT_حد_کان_مٿي) != LONGINT_OK)
        {
          إذا (! (e & LONGINT_حد_کان_مٿي) && n <= INTMAX_MAX)
            {
              *invalid = e;
              أرجع indeterminate;
            }
          e = LONGINT_حد_کان_مٿي;
        }

      إذا (n == 0)
        نتيجو = 0;
      وإلا إذا (نتيجو != 0)
        {
          intmax_t حاصل_ضرب;
          إذا ((e & LONGINT_حد_کان_مٿي)
              || حد_کان_مٿي
              || ckd_mul (&حاصل_ضرب, نتيجو, n))
            حد_کان_مٿي = true;
          وإلا
            نتيجو = حاصل_ضرب;
        }

      إذا (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      إذا (! multiply)
        اكسر;
      str = suffix + 1;
    }

  إذا (نتيجو == 0)
    {
      إذا (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  وإلا إذا (حد_کان_مٿي)
    {
      e = LONGINT_حد_کان_مٿي;
      نتيجو = INTMAX_MAX;
    }

  *invalid = e;
  أرجع نتيجو;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_خالص
ساكن bool
عمليو_is (حرف ثابت *عمليو_2, حرف ثابت *name)
{
  أرجع عمليو_matches (عمليو_2, name, '=');
}

ساكن فراغ
scanargs (صحيح argc, حرف *ثابت *argv)
{
  idx_t blocksize = 0;
  intmax_t ڳڻپ = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool ڳڻپ_B = false, skip_B = false, seek_B = false;

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

      إذا (عمليو_is (name, "if"))
        داخلا_file = val;
      وإلا إذا (عمليو_is (name, "of"))
        output_file = val;
      وإلا إذا (عمليو_is (name, "conv"))
        conversions_نقاب |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      وإلا إذا (عمليو_is (name, "iflag"))
        داخلا_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      وإلا إذا (عمليو_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      وإلا إذا (عمليو_is (name, "status"))
        حالت_level = parse_symbols (val, statuses, true,
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

          إذا (عمليو_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &داخلا_blocksize;
            }
          وإلا إذا (عمليو_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          وإلا إذا (عمليو_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          وإلا إذا (عمليو_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          وإلا إذا (عمليو_is (name, "skip") || عمليو_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          وإلا إذا (عمليو_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          وإلا إذا (عمليو_is (name, "count"))
            {
              ڳڻپ = n;
              ڳڻپ_B = has_B;
            }
          وإلا
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          إذا (n < n_min)
            invalid = LONGINT_INVALID;
          وإلا إذا (n_max < n)
            invalid = LONGINT_حد_کان_مٿي;

          إذا (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_حد_کان_مٿي ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          وإلا إذا (converted_idx)
            *converted_idx = n;
        }
    }

  إذا (blocksize)
    داخلا_blocksize = output_blocksize = blocksize;
  وإلا
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_نقاب |= C_TWOBUFS;
    }

  إذا (داخلا_blocksize == 0)
    داخلا_blocksize = DEFAULT_BLOCKSIZE;
  إذا (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  إذا (conversion_blocksize == 0)
    conversions_نقاب &= ~(C_بلاڪ | C_UNBLOCK);

  إذا (داخلا_flags & (O_DSYNC | O_SYNC))
    داخلا_flags |= O_RSYNC;

  إذا (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  إذا (skip_B)
    داخلا_flags |= O_SKIP_بائيٽون;
  إذا (داخلا_flags & O_SKIP_بائيٽون && skip != 0)
    {
      skip_رڪارڊون = skip / داخلا_blocksize;
      skip_بائيٽون = skip % داخلا_blocksize;
    }
  وإلا إذا (skip != 0)
    skip_رڪارڊون = skip;

  إذا (ڳڻپ_B)
    داخلا_flags |= O_ڳڻپ_بائيٽون;
  إذا (داخلا_flags & O_ڳڻپ_بائيٽون && ڳڻپ != INTMAX_MAX)
    {
      max_رڪارڊون = ڳڻپ / داخلا_blocksize;
      max_بائيٽون = ڳڻپ % داخلا_blocksize;
    }
  وإلا إذا (ڳڻپ != INTMAX_MAX)
    max_رڪارڊون = ڳڻپ;

  إذا (seek_B)
    output_flags |= O_SEEK_بائيٽون;
  إذا (output_flags & O_SEEK_بائيٽون && seek != 0)
    {
      seek_رڪارڊون = seek / output_blocksize;
      seek_بائيٽون = seek % output_blocksize;
    }
  وإلا إذا (seek != 0)
    seek_رڪارڊون = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_نقاب & C_TWOBUFS) && ! (داخلا_flags & O_FULLBLOCK)
     && (skip_رڪارڊون
         || (0 < max_رڪارڊون && max_رڪارڊون < INTMAX_MAX)
         || (داخلا_flags | output_flags) & O_سڌو));

  iread_fnc = ((داخلا_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  داخلا_flags &= ~O_FULLBLOCK;

  إذا (گڻو_بٽ_سيٽ_2 (conversions_نقاب & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  إذا (گڻو_بٽ_سيٽ_2 (conversions_نقاب & (C_بلاڪ | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  إذا (گڻو_بٽ_سيٽ_2 (conversions_نقاب & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  إذا (گڻو_بٽ_سيٽ_2 (conversions_نقاب & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  إذا (گڻو_بٽ_سيٽ_2 (داخلا_flags & (O_سڌو | O_NOCACHE))
      || گڻو_بٽ_سيٽ_2 (output_flags & (O_سڌو | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  إذا (داخلا_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_رڪارڊون == 0 && max_بائيٽون == 0);
      داخلا_flags &= ~O_NOCACHE;
    }
  إذا (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_رڪارڊون == 0 && max_بائيٽون == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ساكن فراغ
apply_translations (فراغ)
{
  إذا (conversions_نقاب & C_ASCII)
    translate_charset (ebcdic_منزل_ascii);

  إذا (conversions_نقاب & C_UCASE)
    {
      لكل (صحيح i = 0; i < 256; i++)
        trans_جدول[i] = وڏو_اکر (trans_جدول[i]);
      translation_needed = true;
    }
  وإلا إذا (conversions_نقاب & C_LCASE)
    {
      لكل (صحيح i = 0; i < 256; i++)
        trans_جدول[i] = tolower (trans_جدول[i]);
      translation_needed = true;
    }

  إذا (conversions_نقاب & C_EBCDIC)
    {
      translate_charset (ascii_منزل_ebcdic);
      newline_character = ascii_منزل_ebcdic['\n'];
      space_character = ascii_منزل_ebcdic[' '];
    }
  وإلا إذا (conversions_نقاب & C_IBM)
    {
      translate_charset (ascii_منزل_ibm);
      newline_character = ascii_منزل_ibm['\n'];
      space_character = ascii_منزل_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ساكن فراغ
translate_بفر (حرف *buf, idx_t nread)
{
  حرف *cp = buf;
  لكل (idx_t i = nread; i; i--, cp++)
    *cp = trans_جدول[منزل_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ساكن حرف *
swab_بفر (حرف *buf, idx_t *nread, صحيح *saved_byte)
{
  إذا (*nread == 0)
    أرجع buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  صحيح prev_saved = *saved_byte;
  إذا ((prev_saved < 0) == (*nread & 1))
    {
      بنانشان حرف c = buf[--*nread];
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
advance_داخلا_وٿي (intmax_t وٿي)
{
  إذا (0 <= داخلا_وٿي && ckd_جوڙيو (&داخلا_وٿي, داخلا_وٿي, وٿي))
    داخلا_وٿي = -1;
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
skip (صحيح fdesc, حرف ثابت *file, intmax_t رڪارڊون, idx_t blocksize,
      idx_t *بائيٽون)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t وٿي;
  إذا (! ckd_mul (&وٿي, رڪارڊون, blocksize)
      && ! ckd_جوڙيو (&وٿي, وٿي, *بائيٽون)
      && 0 <= lseek (fdesc, وٿي, SEEK_CUR))
    {
      إذا (fdesc == STDIN_FILENO)
        {
           بنية stat st;
           إذا (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           إذا (usable_st_سائيز (&st) && 0 < st.st_سائيز && 0 <= داخلا_وٿي
               && st.st_سائيز - داخلا_وٿي < وٿي)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               رڪارڊون = ( وٿي - st.st_سائيز ) / blocksize;
               وٿي = st.st_سائيز - داخلا_وٿي;
             }
           وإلا
             رڪارڊون = 0;
           advance_داخلا_وٿي (وٿي);
        }
      وإلا
        {
          رڪارڊون = 0;
          *بائيٽون = 0;
        }
      أرجع رڪارڊون;
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
          ssize_t nread = iread_fnc (fdesc, buf, رڪارڊون ? blocksize : *بائيٽون);
          إذا (nread < 0)
            {
              إذا (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  إذا (conversions_نقاب & C_NOERROR)
                    print_stats ();
                }
              وإلا
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          وإلا إذا (nread == 0)
            اكسر;
          وإلا إذا (fdesc == STDIN_FILENO)
            advance_داخلا_وٿي (nread);

          إذا (رڪارڊون != 0)
            رڪارڊون--;
          وإلا
            *بائيٽون = 0;
        }
      طالما (رڪارڊون || *بائيٽون);

      أرجع رڪارڊون;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ساكن bool
advance_داخلا_after_read_error (idx_t nbytes)
{
  إذا (! داخلا_seekable)
    {
      إذا (داخلا_seek_errno == ESPIPE)
        أرجع true;
      errno = داخلا_seek_errno;
    }
  وإلا
    {
      off_t وٿي;
      advance_داخلا_وٿي (nbytes);
      إذا (داخلا_وٿي < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (داخلا_file));
          أرجع false;
        }
      وٿي = lseek (STDIN_FILENO, 0, SEEK_CUR);
      إذا (0 <= وٿي)
        {
          off_t diff;
          إذا (وٿي == داخلا_وٿي)
            أرجع true;
          diff = داخلا_وٿي - وٿي;
          إذا (! (0 <= diff && diff <= nbytes) && حالت_level != حالت_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          إذا (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            أرجع true;
          إذا (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (داخلا_file));
  أرجع false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ساكن فراغ
copy_simple (حرف ثابت *buf, idx_t nread)
{
  حرف ثابت *شروعات = buf;	/* First uncopied char in BUF.  */

  افعل
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      يادگيري_نقل (obuf + oc, شروعات, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      شروعات += nfree;
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
copy_with_بلاڪ (حرف ثابت *buf, idx_t nread)
{
  لكل (idx_t i = nread; i; i--, buf++)
    {
      إذا (*buf == newline_character)
        {
          إذا (کانو < conversion_blocksize)
            {
              لكل (idx_t j = کانو; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          کانو = 0;
        }
      وإلا
        {
          إذا (کانو == conversion_blocksize)
            r_truncate++;
          وإلا إذا (کانو < conversion_blocksize)
            output_char (*buf);
          کانو++;
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

      إذا (کانو++ >= conversion_blocksize)
        {
          کانو = pending_spaces = 0; /* Wipe out any pending spaces.  */
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
سيٽ_fd_flags (صحيح fd, صحيح جوڙيو_flags, حرف ثابت *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  جوڙيو_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  إذا (جوڙيو_flags)
    {
      صحيح old_flags = fcntl (fd, F_GETFL);
      صحيح new_flags = old_flags | جوڙيو_flags;
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
              وإلا إذا ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_گهڻائي))
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

  صحيح exit_حالت = EXIT_SUCCESS;
  idx_t n_بائيٽون_read;

  إذا (skip_رڪارڊون != 0 || skip_بائيٽون != 0)
    {
      intmax_t us_بائيٽون;
      bool us_بائيٽون_حد_کان_مٿي =
        (ckd_mul (&us_بائيٽون, skip_رڪارڊون, داخلا_blocksize)
         || ckd_جوڙيو (&us_بائيٽون, skip_بائيٽون, us_بائيٽون));
      off_t داخلا_offset0 = داخلا_وٿي;
      intmax_t us_بلاڪون = skip (STDIN_FILENO, داخلا_file,
                                 skip_رڪارڊون, داخلا_blocksize, &skip_بائيٽون);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      إذا ((us_بلاڪون
           || (0 <= داخلا_وٿي
               && (us_بائيٽون_حد_کان_مٿي
                   || us_بائيٽون != داخلا_وٿي - داخلا_offset0)))
          && حالت_level != حالت_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (داخلا_file));
        }
    }

  إذا (seek_رڪارڊون != 0 || seek_بائيٽون != 0)
    {
      idx_t بائيٽون = seek_بائيٽون;
      intmax_t write_رڪارڊون = skip (STDOUT_FILENO, output_file,
                                      seek_رڪارڊون, output_blocksize, &بائيٽون);

      إذا (write_رڪارڊون != 0 || بائيٽون != 0)
        {
          memset (obuf, 0, write_رڪارڊون ? output_blocksize : بائيٽون);

          افعل
            {
              idx_t سائيز = write_رڪارڊون ? output_blocksize : بائيٽون;
              إذا (iwrite (STDOUT_FILENO, obuf, سائيز) != سائيز)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              إذا (write_رڪارڊون != 0)
                write_رڪارڊون--;
              وإلا
                بائيٽون = 0;
            }
          طالما (write_رڪارڊون || بائيٽون);
        }
    }

  إذا (max_رڪارڊون == 0 && max_بائيٽون == 0)
    أرجع exit_حالت;

  alloc_ibuf ();
  alloc_obuf ();
  صحيح saved_byte = -1;

  طالما (true)
    {
      إذا (حالت_level == حالت_ترقي)
        {
          xtime_t ترقي_وقت = gethrxtime ();
          إذا (ايندڙ_وقت <= ترقي_وقت)
            {
              print_xfer_stats (ترقي_وقت);
              ايندڙ_وقت += XTIME_PRECISION;
            }
        }

      إذا (r_partial + r_ڀريل >= max_رڪارڊون + !!max_بائيٽون)
        اكسر;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      إذا ((conversions_نقاب & C_SYNC) && (conversions_نقاب & C_NOERROR))
        memset (ibuf,
                (conversions_نقاب & (C_بلاڪ | C_UNBLOCK)) ? ' ' : '\0',
                داخلا_blocksize);

      إذا (r_partial + r_ڀريل >= max_رڪارڊون)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_بائيٽون);
      وإلا
        nread = iread_fnc (STDIN_FILENO, ibuf, داخلا_blocksize);

      إذا (nread > 0)
        {
          advance_داخلا_وٿي (nread);
          إذا (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      وإلا إذا (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_نقاب & C_NOTRUNC);
          اكسر;			/* EOF.  */
        }
      وإلا
        {
          إذا (!(conversions_نقاب & C_NOERROR) || حالت_level != حالت_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (داخلا_file));

          إذا (conversions_نقاب & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = داخلا_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              إذا (!advance_داخلا_after_read_error (bad_portion))
                {
                  exit_حالت = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  داخلا_seekable = false;
                  داخلا_seek_errno = ESPIPE;
                }
              إذا ((conversions_نقاب & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              وإلا
                استمر;
            }
          وإلا
            {
              /* Write any partial block. */
              exit_حالت = EXIT_FAILURE;
              اكسر;
            }
        }

      n_بائيٽون_read = nread;

      إذا (n_بائيٽون_read < داخلا_blocksize)
        {
          r_partial++;
          partread = n_بائيٽون_read;
          إذا (conversions_نقاب & C_SYNC)
            {
              إذا (!(conversions_نقاب & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_بائيٽون_read,
                        (conversions_نقاب & (C_بلاڪ | C_UNBLOCK)) ? ' ' : '\0',
                        داخلا_blocksize - n_بائيٽون_read);
              n_بائيٽون_read = داخلا_blocksize;
            }
        }
      وإلا
        {
          r_ڀريل++;
          partread = 0;
        }

      إذا (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_بائيٽون_read);
          w_بائيٽون += nwritten;
          إذا (nwritten != n_بائيٽون_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              إذا (nwritten != 0)
                w_partial++;
              أرجع EXIT_FAILURE;
            }
          وإلا إذا (n_بائيٽون_read == داخلا_blocksize)
            w_ڀريل++;
          وإلا
            w_partial++;
          استمر;
        }

      /* Do any translations on the whole buffer at once.  */

      إذا (translation_needed)
        translate_بفر (ibuf, n_بائيٽون_read);

      إذا (conversions_نقاب & C_SWAB)
        bufstart = swab_بفر (ibuf, &n_بائيٽون_read, &saved_byte);
      وإلا
        bufstart = ibuf;

      إذا (conversions_نقاب & C_بلاڪ)
        copy_with_بلاڪ (bufstart, n_بائيٽون_read);
      وإلا إذا (conversions_نقاب & C_UNBLOCK)
        copy_with_unblock (bufstart, n_بائيٽون_read);
      وإلا
        copy_simple (bufstart, n_بائيٽون_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  إذا (0 <= saved_byte)
    {
      حرف saved_char = saved_byte;
      إذا (conversions_نقاب & C_بلاڪ)
        copy_with_بلاڪ (&saved_char, 1);
      وإلا إذا (conversions_نقاب & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      وإلا
        output_char (saved_char);
    }

  إذا ((conversions_نقاب & C_بلاڪ) && کانو > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      لكل (idx_t i = کانو; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  إذا (کانو && (conversions_نقاب & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  إذا (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_بائيٽون += nwritten;
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
      إذا (S_ISREG (stdout_stat.st_گهڻائي) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_وٿي = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          إذا (0 <= output_وٿي && stdout_stat.st_سائيز < output_وٿي)
            {
              إذا (iftruncate (STDOUT_FILENO, output_وٿي) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_وٿي}, quoteaf (output_file));
                  أرجع EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  إذا (conversions_نقاب & (C_FDATASYNC | C_FSYNC)
      && حالت_level == حالت_ترقي
      && 0 <= reported_w_بائيٽون && reported_w_بائيٽون < w_بائيٽون)
    print_xfer_stats (0);

  أرجع exit_حالت;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ساكن صحيح
synchronize_output (فراغ)
{
  صحيح exit_حالت = 0;
  صحيح نقاب = conversions_نقاب;
  conversions_نقاب &= ~ (C_FDATASYNC | C_FSYNC);

  إذا ((نقاب & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      إذا (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_حالت = EXIT_FAILURE;
        }
      نقاب |= C_FSYNC;
    }

  إذا ((نقاب & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      أرجع EXIT_FAILURE;
    }

  أرجع exit_حالت;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  off_t وٿي;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  صفحو_سائيز = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE, نسخو,
                                   true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  لكل (صحيح i = 0; i < 256; i++)
    trans_جدول[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  إذا (داخلا_file == عدم)
    {
      داخلا_file = _("standard input");
      سيٽ_fd_flags (STDIN_FILENO, داخلا_flags, داخلا_file);
    }
  وإلا
    {
      إذا (ifd_reopen (STDIN_FILENO, داخلا_file, O_RDONLY | داخلا_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (داخلا_file));
    }

  وٿي = lseek (STDIN_FILENO, 0, SEEK_CUR);
  داخلا_seekable = (0 <= وٿي);
  داخلا_وٿي = MAX (0, وٿي);
  داخلا_seek_errno = errno;

  إذا (output_file == عدم)
    {
      output_file = _("standard output");
      سيٽ_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  وإلا
    {
      mode_t perms = گهڻائي_RW_UGO;
      صحيح opts
        = (output_flags
           | (conversions_نقاب & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_نقاب & C_EXCL ? O_EXCL : 0)
           | (seek_رڪارڊون || (conversions_نقاب & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t سائيز;
      إذا ((ckd_mul (&سائيز, seek_رڪارڊون, output_blocksize)
           || ckd_جوڙيو (&سائيز, seek_بائيٽون, سائيز))
          && !(conversions_نقاب & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_رڪارڊون, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      إذا ((! seek_رڪارڊون
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      إذا (seek_رڪارڊون != 0 && !(conversions_نقاب & C_NOTRUNC))
        {
          إذا (iftruncate (STDOUT_FILENO, سائيز) != 0)
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
              وإلا إذا (S_ISREG (stdout_stat.st_گهڻائي)
                       || S_ISDIR (stdout_stat.st_گهڻائي)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = سائيز;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  شروعات_وقت = gethrxtime ();
  ايندڙ_وقت = شروعات_وقت + XTIME_PRECISION;

  صحيح copy_حالت = dd_copy ();
  صحيح sync_حالت = synchronize_output ();
  صحيح exit_حالت = copy_حالت | sync_حالت;

  إذا (max_رڪارڊون == 0 && max_بائيٽون == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      إذا (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (داخلا_file));
          exit_حالت = EXIT_FAILURE;
        }
      إذا (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_حالت = EXIT_FAILURE;
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

  پڄاڻي_مٿي ();
  main_exit (exit_حالت);
}
