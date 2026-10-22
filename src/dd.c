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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "alignalloc.h"
#অন্তর্ভাব "close-stream.h"
#অন্তর্ভাব "fd-reopen.h"
#অন্তর্ভাব "gethrxtime.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "long-options.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dd"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#ifndef SA_RESETHAND
# define SA_RESETHAND 0
#endif

#ifndef SIGINFO
# define SIGINFO SIGUSR1
#endif

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#ifndef O_CIO
# define O_CIO 0
#endif

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#undef O_NOCACHE

#define output_char(c)				\
  করো						\
    {						\
      obuf[oc++] = (c);				\
      যদি (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  যখন (0)

/* Default input and output blocksize. */
#define DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
গণনা
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_BLOCK = 010,
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
গণনা
  {
    STATUS_NONE = 1,
    STATUS_NOXFER = 2,
    STATUS_DEFAULT = 3,
    STATUS_PROGRESS = 4
  };

/* The name of the input file, or NULL for the standard input. */
স্থির অক্ষর ধ্রুবক *প্রবেশ_file = ফাঁকা;

/* The name of the output file, or NULL for the standard output. */
স্থির অক্ষর ধ্রুবক *output_file = ফাঁকা;

/* The page size on this host.  */
স্থির idx_t page_আকার;

/* The number of bytes in which atomic reads are done. */
স্থির idx_t প্রবেশ_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
স্থির idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
স্থির idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
স্থির intmax_t skip_records = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
স্থির idx_t skip_bytes = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
স্থির intmax_t seek_records = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
স্থির intmax_t seek_bytes = 0;

/* Whether the final output was done with a seek (rather than a write).  */
স্থির bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
স্থির intmax_t max_records = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
স্থির idx_t max_bytes = 0;

/* Bit vector of conversions to apply. */
স্থির পূর্ণসংখ্যা conversions_mask = 0;

/* Open flags for the input and output files.  */
স্থির পূর্ণসংখ্যা প্রবেশ_flags = 0;
স্থির পূর্ণসংখ্যা output_flags = 0;

/* Status flags for what is printed to stderr.  */
স্থির পূর্ণসংখ্যা status_level = STATUS_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
স্থির bool translation_needed = false;

/* Number of partial blocks written. */
স্থির intmax_t w_partial = 0;

/* Number of full blocks written. */
স্থির intmax_t w_full = 0;

/* Number of partial blocks read. */
স্থির intmax_t r_partial = 0;

/* Number of full blocks read. */
স্থির intmax_t r_full = 0;

/* Number of bytes written.  */
স্থির intmax_t w_bytes = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
স্থির intmax_t reported_w_bytes = -1;

/* Time that dd started.  */
স্থির xtime_t start_time;

/* Next time to report periodic progress.  */
স্থির xtime_t পরবর্তী_time;

/* If positive, the number of bytes output in the current progress line.  */
স্থির পূর্ণসংখ্যা progress_len;

/* True if input is seekable.  */
স্থির bool প্রবেশ_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
স্থির পূর্ণসংখ্যা প্রবেশ_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
স্থির off_t প্রবেশ_offset;

/* True if a partial read should be diagnosed.  */
স্থির bool warn_partial_read;

/* Records truncated by conv=block. */
স্থির intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
স্থির অক্ষর newline_character = '\n';
স্থির অক্ষর space_character = ' ';

/* I/O buffers.  */
স্থির অক্ষর *ibuf;
স্থির অক্ষর *obuf;

/* Current index into 'obuf'. */
স্থির idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
স্থির idx_t col = 0;

/* The set of signals that are caught.  */
স্থির sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
স্থির sig_atomic_t অস্থির interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
স্থির sig_atomic_t অস্থির info_signal_গুনতি;

/* Whether to discard cache for input or output.  */
স্থির bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
স্থির bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
স্থির ssize_t (*iread_fnc) (পূর্ণসংখ্যা fd, অক্ষর *buf, idx_t আকার_2);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_প্রতীক "count_bytes"

/* A symbol and the corresponding integer value.  */
কাঠামো প্রতীক_মান
{
  অক্ষর প্রতীক[আকার LONGEST_প্রতীক];
  পূর্ণসংখ্যা মান;
};

/* Conversion symbols, for conv="...".  */
স্থির কাঠামো প্রতীক_মান ধ্রুবক conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_BLOCK | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_BLOCK | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_BLOCK | C_TWOBUFS},	/* Variable to fixed length records. */
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

#define FFS_MASK(x) ((x) ^ ((x) & ((x) - 1)))
গণনা
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_দ্বিমিক
          | O_CIO
          | O_DIRECT
          | O_DIRECTORY
          | O_DSYNC
          | O_EXCL
          | O_NOATIME
          | O_NOCTTY
          | O_NOFOLLOW
          | O_NOLINKS
          | O_NONBLOCK
          | O_SYNC
          | O_TEXT
          ),

    /* Use its lowest bits for private flags.  */
    O_FULLBLOCK = FFS_MASK (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_MASK (v2),
    v3 = v2 ^ O_NOCACHE,

    O_গুনতি_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_গুনতি_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_গুনতি_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_গুনতি_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
স্থির কাঠামো প্রতীক_মান ধ্রুবক flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_দ্বিমিক},
  {"cio",	  O_CIO},
  {"direct",	  O_DIRECT},
  {"directory",   O_DIRECTORY},
  {"dsync",	  O_DSYNC},
  {"noatime",	  O_NOATIME},
  {"nocache",	  O_NOCACHE},   /* Discard cache.  */
  {"noctty",	  O_NOCTTY},
  {"nofollow",	  HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0},
  {"nolinks",	  O_NOLINKS},
  {"nonblock",	  O_NONBLOCK},
  {"sync",	  O_SYNC},
  {"text",	  O_TEXT},
  {"fullblock",   O_FULLBLOCK}, /* Accumulate full blocks from input.  */
  {"count_bytes", O_গুনতি_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
স্থির কাঠামো প্রতীক_মান ধ্রুবক statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
স্থির unsigned অক্ষর trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

স্থির অক্ষর ধ্রুবক ascii_to_ebcdic[] =
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

স্থির অক্ষর ধ্রুবক ascii_to_ibm[] =
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

স্থির অক্ষর ধ্রুবক ebcdic_to_ascii[] =
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
স্থির bool close_stdout_required = true;

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
স্থির শূন্য
maybe_close_stdout (শূন্য)
{
  যদি (close_stdout_required)
    close_stdout ();
  অন্যথা যদি (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
স্থির শূন্য
diagnose (পূর্ণসংখ্যা errnum, অক্ষর ধ্রুবক *fmt, ...)
{
  যদি (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  va_list ap;
  va_start (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_end (ap);
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPERAND]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      fputs (_("\
Copy a file, converting and formatting according to the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=BYTES        পঠ and লিখ up to BYTES bytes at a time (পূর্বনির্ধারিত: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a time\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated প্রতীক list\n\
"));
      oputs (_("\
  গুনতি=N         copy only N প্রবেশ blocks\n\
"));
      oputs (_("\
  ibs=BYTES       পঠ up to BYTES bytes at a time (পূর্বনির্ধারিত: 512)\n\
"));
      oputs (_("\
  যদি=FILE         পঠ from FILE instead of standard প্রবেশ\n\
"));
      oputs (_("\
  iflag=FLAGS     পঠ as per the comma separated প্রতীক list\n\
"));
      oputs (_("\
  obs=BYTES       লিখ BYTES bytes at a time (পূর্বনির্ধারিত: 512)\n\
"));
      oputs (_("\
  of=FILE         লিখ to FILE instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     লিখ as per the comma separated প্রতীক list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized প্রবেশ blocks\n\
"));
      oputs (_("\
  status=LEVEL    The LEVEL of information to print to standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and BYTES may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on জন্য T, P, E, Z, Y, R, Q.\n\
দ্বিমিক_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV প্রতীক may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-আকার_2\n\
  unblock   replace trailing spaces in cbs-আকার_2 records with newline\n\
  lcase     change upper ক্ষেত্র to lower ক্ষেত্র\n\
  ucase     change lower ক্ষেত্র to upper ক্ষেত্র\n\
  sparse    try to seek rather than লিখ all-NUL output blocks\n\
  swab      swap every pair of প্রবেশ bytes\n\
  sync      pad every প্রবেশ block with NULs to ibs-আকার_2; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail যদি the output file already exists\n\
  nocreat   করো not create the output file\n\
  notrunc   করো not truncate the output file\n\
  noerror   চালু after পঠ errors\n\
  fdatasync  physically লিখ output file তথ্য before finishing\n\
  fsync     likewise, but also লিখ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG প্রতীক may be:\n\
\n\
  append    append mode (makes sense only জন্য output; conv=notrunc suggested)\n\
"), stdout);
      যদি (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      যদি (O_DIRECT)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      যদি (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      যদি (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      যদি (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      যদি (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#যদি HAVE_POSIX_FADVISE
      যদি (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#endif
      যদি (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      যদি (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      যদি (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      যদি (O_দ্বিমিক)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      যদি (O_TEXT)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        printf (_("\
\n\
Sending a %s সংকেত to a running 'dd' process makes it\n\
print I/O statistics to standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Common options to use when displaying sizes and rates.  */

গণনা { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

স্থির শূন্য
alloc_ibuf (শূন্য)
{
  যদি (ibuf)
    ফেরত;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_আকার, প্রবেশ_blocksize + extra_byte_for_swab);
  যদি (!ibuf)
    {
      অক্ষর hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             প্রবেশ_blocksize,
             human_readable (প্রবেশ_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

স্থির শূন্য
alloc_obuf (শূন্য)
{
  যদি (obuf)
    ফেরত;

  যদি (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_আকার, output_blocksize);
      যদি (!obuf)
        {
          অক্ষর hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  অন্যথা
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

স্থির শূন্য
translate_charset (অক্ষর ধ্রুবক *new_trans)
{
  জন্য (পূর্ণসংখ্যা i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

স্থির অন্তঃরেখ bool
multiple_bits_set (পূর্ণসংখ্যা i)
{
  ফেরত MULTIPLE_BITS_SET (i);
}

স্থির bool
abbreviation_lacks_prefix (অক্ষর ধ্রুবক *message)
{
  ফেরত message[strlen (message) - 2] == ' ';
}

/* Print transfer statistics.  */

স্থির শূন্য
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  স্থির অক্ষর ধ্রুবক slash_s[] = "/s";
  অক্ষর hbuf[3][LONGEST_HUMAN_READABLE + আকার slash_s];
  দ্বিগুণ delta_s;
  অক্ষর ধ্রুবক *bytes_per_second;
  অক্ষর ধ্রুবক *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  অক্ষর ধ্রুবক *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  অক্ষর *bpsbuf = hbuf[2];
  পূর্ণসংখ্যা bpsbufsize = আকার hbuf[2];
  যদি (start_time < now)
    {
      দ্বিগুণ XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  অন্যথা
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  যদি (progress_time)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  অক্ষর delta_s_buf[24];
  snprintf (delta_s_buf, আকার delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  পূর্ণসংখ্যা stats_len
    = (abbreviation_lacks_prefix (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_bytes)),
                  w_bytes, delta_s_buf, bytes_per_second)
       : abbreviation_lacks_prefix (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_bytes, si, delta_s_buf, bytes_per_second)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_bytes, si, iec, delta_s_buf, bytes_per_second));

  যদি (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      যদি (0 <= stats_len && stats_len < progress_len)
        fprintf (stderr, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  অন্যথা
    fputc ('\n', stderr);

  reported_w_bytes = w_bytes;
}

স্থির শূন্য
print_stats (শূন্য)
{
  যদি (status_level == STATUS_NONE)
    ফেরত;

  যদি (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  যদি (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  যদি (status_level == STATUS_NOXFER)
    ফেরত;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

স্থির শূন্য
interrupt_handler (পূর্ণসংখ্যা sig)
{
  যদি (! SA_RESETHAND)
    সংকেত (sig, SIG_DFL);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

স্থির শূন্য
siginfo_handler (MAYBE_UNUSED পূর্ণসংখ্যা sig)
{
  info_signal_গুনতি++;
}

/* Install the signal handlers.  */

স্থির শূন্য
install_signal_handlers (শূন্য)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  কাঠামো sigaction act;
  sigemptyset (&caught_signals);
  যদি (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (SIGINT, ফাঁকা, &act);
  যদি (act.sa_handler != SIG_IGN)
    sigaddset (&caught_signals, SIGINT);
  act.sa_mask = caught_signals;

  যদি (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ফাঁকা);
    }

  যদি (sigismember (&caught_signals, SIGINT))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ফাঁকা);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
স্থির পূর্ণসংখ্যা
iclose (পূর্ণসংখ্যা fd)
{
  যদি (close (fd) != 0)
    করো
      যদি (errno != EINTR)
        ফেরত -1;
    যখন (close (fd) != 0 && errno != EBADF);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা synchronize_output (শূন্য);

স্থির শূন্য
cleanup (শূন্য)
{
  যদি (!interrupt_signal)
    {
      পূর্ণসংখ্যা sync_status = synchronize_output ();
      যদি (sync_status)
        exit (sync_status);
    }

  যদি (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (প্রবেশ_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  যদি (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

স্থির শূন্য
process_signals (শূন্য)
{
  যখন (interrupt_signal || info_signal_গুনতি)
    {
      পূর্ণসংখ্যা interrupt;
      পূর্ণসংখ্যা infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_গুনতি;

      যদি (infos)
        info_signal_গুনতি = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ফাঁকা);

      যদি (interrupt)
        cleanup ();
      print_stats ();
      যদি (interrupt)
        তোলো (interrupt);
    }
}

স্থির শূন্য
finish_up (শূন্য)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_signals ();
  cleanup ();
  print_stats ();
}

স্থির শূন্য
quit (পূর্ণসংখ্যা code)
{
  finish_up ();
  exit (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

স্থির off_t
cache_round (পূর্ণসংখ্যা fd, off_t len)
{
  স্থির off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  যদি (len)
    {
      intmax_t c_pending;
      যদি (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      যদি (c_pending > *pending)
        len = c_pending - *pending;
      অন্যথা
        len = 0;
    }
  অন্যথা
    len = *pending;

  ফেরত len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

স্থির bool
invalidate_cache (পূর্ণসংখ্যা fd, off_t len)
{
  পূর্ণসংখ্যা adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  যদি (len && !clen)
    ফেরত true; /* Don't advise this time.  */
  অন্যথা যদি (! len && ! clen && ! nocache_eof)
    ফেরত true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  যদি (fd == STDIN_FILENO)
    {
      যদি (প্রবেশ_seekable)
        offset = প্রবেশ_offset;
      অন্যথা
        {
          offset = -1;
          errno = ESPIPE;
        }
    }
  অন্যথা
    {
      স্থির off_t output_offset = -2;

      যদি (output_offset != -1)
        {
          যদি (output_offset < 0)
            output_offset = lseek (fd, 0, SEEK_CUR);
          অন্যথা যদি (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  যদি (0 <= offset)
   {
     যদি (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#যদি HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     যদি (clen == 0)
       offset -= offset % page_আকার;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#অন্যথা
     errno = ENOTSUP;
#endif
   }

  ফেরত adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

স্থির ssize_t
iread (পূর্ণসংখ্যা fd, অক্ষর *buf, idx_t আকার_2)
{
  ssize_t nread;
  স্থির ssize_t পূর্ব_nread;

  করো
    {
      process_signals ();
      nread = পঠ (fd, buf, আকার_2);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      যদি (nread == -1 && errno == EINVAL
          && 0 < পূর্ব_nread && পূর্ব_nread < আকার_2
          && (প্রবেশ_flags & O_DIRECT))
        {
          errno = 0;
          nread = 0;
        }
    }
  যখন (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  যদি (0 < nread && nread < আকার_2)
    process_signals ();

  যদি (0 < nread && warn_partial_read)
    {
      যদি (0 < পূর্ব_nread && পূর্ব_nread < আকার_2)
        {
          idx_t পূর্ব = পূর্ব_nread;
          যদি (status_level != STATUS_NONE)
            diagnose (0, ngettext (("warning: partial read (%td byte); "
                                    "suggest iflag=fullblock"),
                                   ("warning: partial read (%td bytes); "
                                    "suggest iflag=fullblock"),
                                   select_plural (পূর্ব)),
                      পূর্ব);
          warn_partial_read = false;
        }
    }

  পূর্ব_nread = nread;
  ফেরত nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
স্থির ssize_t
iread_fullblock (পূর্ণসংখ্যা fd, অক্ষর *buf, idx_t আকার_2)
{
  ssize_t nread = 0;

  যখন (0 < আকার_2)
    {
      ssize_t ncurr = iread (fd, buf, আকার_2);
      যদি (ncurr < 0)
        ফেরত ncurr;
      যদি (ncurr == 0)
        বিরতি;
      nread += ncurr;
      buf   += ncurr;
      আকার_2  -= ncurr;
    }

  ফেরত nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

স্থির idx_t
iwrite (পূর্ণসংখ্যা fd, অক্ষর ধ্রুবক *buf, idx_t আকার_2)
{
  idx_t যোগ_written = 0;

  যদি ((output_flags & O_DIRECT) && আকার_2 < output_blocksize)
    {
      পূর্ণসংখ্যা old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      যদি (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
          && status_level != STATUS_NONE)
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
      conversions_mask |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  যখন (যোগ_written < আকার_2)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      যদি ((conversions_mask & C_SPARSE) && is_nul (buf, আকার_2))
        {
          যদি (lseek (fd, আকার_2, SEEK_CUR) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          অন্যথা
            {
              final_op_was_seek = true;
              nwritten = আকার_2;
            }
        }

      যদি (!nwritten)
        nwritten = লিখ (fd, buf + যোগ_written, আকার_2 - যোগ_written);

      যদি (nwritten < 0)
        {
          যদি (errno != EINTR)
            বিরতি;
        }
      অন্যথা যদি (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          বিরতি;
        }
      অন্যথা
        যোগ_written += nwritten;
    }

  যদি (o_nocache && যোগ_written)
    invalidate_cache (fd, যোগ_written);

  ফেরত যোগ_written;
}

/* Write, then empty, the output buffer 'obuf'. */

স্থির শূন্য
write_output (শূন্য)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  যদি (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      যদি (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  অন্যথা
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

স্থির পূর্ণসংখ্যা
ifdatasync (পূর্ণসংখ্যা fd)
{
  পূর্ণসংখ্যা ret;

  করো
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  যখন (ret < 0 && errno == EINTR);

  ফেরত ret;
}

/* Restart on EINTR from fd_reopen.  */

স্থির পূর্ণসংখ্যা
ifd_reopen (পূর্ণসংখ্যা desired_fd, অক্ষর ধ্রুবক *file, পূর্ণসংখ্যা flag, mode_t mode)
{
  পূর্ণসংখ্যা ret;

  করো
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  যখন (ret < 0 && errno == EINTR);

  ফেরত ret;
}

/* Restart on EINTR from fstat.  */

স্থির পূর্ণসংখ্যা
ifstat (পূর্ণসংখ্যা fd, কাঠামো stat *st)
{
  পূর্ণসংখ্যা ret;

  করো
    {
      process_signals ();
      ret = fstat (fd, st);
    }
  যখন (ret < 0 && errno == EINTR);

  ফেরত ret;
}

/* Restart on EINTR from fsync.  */

স্থির পূর্ণসংখ্যা
ifsync (পূর্ণসংখ্যা fd)
{
  পূর্ণসংখ্যা ret;

  করো
    {
      process_signals ();
      ret = fsync (fd);
    }
  যখন (ret < 0 && errno == EINTR);

  ফেরত ret;
}

/* Restart on EINTR from ftruncate.  */

স্থির পূর্ণসংখ্যা
iftruncate (পূর্ণসংখ্যা fd, off_t দৈর্ঘ্য)
{
  পূর্ণসংখ্যা ret;

  করো
    {
      process_signals ();
      ret = ftruncate (fd, দৈর্ঘ্য);
    }
  যখন (ret < 0 && errno == EINTR);

  ফেরত ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
স্থির bool
operand_matches (অক্ষর ধ্রুবক *str, অক্ষর ধ্রুবক *pattern, অক্ষর delim)
{
  যখন (*pattern)
    যদি (*str++ != *pattern++)
      ফেরত false;
  ফেরত !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

স্থির পূর্ণসংখ্যা
parse_symbols (অক্ষর ধ্রুবক *str, কাঠামো প্রতীক_মান ধ্রুবক *table,
               bool exclusive, অক্ষর ধ্রুবক *error_msgid)
{
  পূর্ণসংখ্যা মান = 0;

  যখন (true)
    {
      অক্ষর ধ্রুবক *strcomma = strchr (str, ',');
      কাঠামো প্রতীক_মান ধ্রুবক *entry;

      জন্য (entry = table;
           ! (operand_matches (str, entry->প্রতীক, ',') && entry->মান);
           entry++)
        {
          যদি (! entry->প্রতীক[0])
            {
              idx_t slen = strcomma ? strcomma - str : strlen (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      যদি (exclusive)
        মান = entry->মান;
      অন্যথা
        মান |= entry->মান;
      যদি (!strcomma)
        বিরতি;
      str = strcomma + 1;
    }

  ফেরত মান;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

স্থির intmax_t
parse_integer (অক্ষর ধ্রুবক *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  পূর্ণসংখ্যা indeterminate = 0;
  স্থির অক্ষর ধ্রুবক suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t ফলাফল = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  যখন (true)
    {
      uintmax_t n = indeterminate;
      অক্ষর *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      যদি ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          যদি (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      যদি (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      যদি ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          যদি (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              ফেরত indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      যদি (n == 0)
        ফলাফল = 0;
      অন্যথা যদি (ফলাফল != 0)
        {
          intmax_t product;
          যদি ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, ফলাফল, n))
            overflow = true;
          অন্যথা
            ফলাফল = product;
        }

      যদি (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      যদি (! multiply)
        বিরতি;
      str = suffix + 1;
    }

  যদি (ফলাফল == 0)
    {
      যদি (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  অন্যথা যদি (overflow)
    {
      e = LONGINT_OVERFLOW;
      ফলাফল = INTMAX_MAX;
    }

  *invalid = e;
  ফেরত ফলাফল;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
স্থির bool
operand_is (অক্ষর ধ্রুবক *operand, অক্ষর ধ্রুবক *name)
{
  ফেরত operand_matches (operand, name, '=');
}

স্থির শূন্য
scanargs (পূর্ণসংখ্যা argc, অক্ষর *ধ্রুবক *argv)
{
  idx_t blocksize = 0;
  intmax_t গুনতি = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool গুনতি_B = false, skip_B = false, seek_B = false;

  জন্য (পূর্ণসংখ্যা i = optind; i < argc; i++)
    {
      অক্ষর ধ্রুবক *name = argv[i];
      অক্ষর ধ্রুবক *val = strchr (name, '=');

      যদি (val == ফাঁকা)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      যদি (operand_is (name, "if"))
        প্রবেশ_file = val;
      অন্যথা যদি (operand_is (name, "of"))
        output_file = val;
      অন্যথা যদি (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      অন্যথা যদি (operand_is (name, "iflag"))
        প্রবেশ_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      অন্যথা যদি (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      অন্যথা যদি (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      অন্যথা
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ফাঁকা;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          যদি (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &প্রবেশ_blocksize;
            }
          অন্যথা যদি (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          অন্যথা যদি (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          অন্যথা যদি (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          অন্যথা যদি (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          অন্যথা যদি (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          অন্যথা যদি (operand_is (name, "count"))
            {
              গুনতি = n;
              গুনতি_B = has_B;
            }
          অন্যথা
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          যদি (n < n_min)
            invalid = LONGINT_INVALID;
          অন্যথা যদি (n_max < n)
            invalid = LONGINT_OVERFLOW;

          যদি (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          অন্যথা যদি (converted_idx)
            *converted_idx = n;
        }
    }

  যদি (blocksize)
    প্রবেশ_blocksize = output_blocksize = blocksize;
  অন্যথা
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  যদি (প্রবেশ_blocksize == 0)
    প্রবেশ_blocksize = DEFAULT_BLOCKSIZE;
  যদি (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  যদি (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  যদি (প্রবেশ_flags & (O_DSYNC | O_SYNC))
    প্রবেশ_flags |= O_RSYNC;

  যদি (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  যদি (skip_B)
    প্রবেশ_flags |= O_SKIP_BYTES;
  যদি (প্রবেশ_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / প্রবেশ_blocksize;
      skip_bytes = skip % প্রবেশ_blocksize;
    }
  অন্যথা যদি (skip != 0)
    skip_records = skip;

  যদি (গুনতি_B)
    প্রবেশ_flags |= O_গুনতি_BYTES;
  যদি (প্রবেশ_flags & O_গুনতি_BYTES && গুনতি != INTMAX_MAX)
    {
      max_records = গুনতি / প্রবেশ_blocksize;
      max_bytes = গুনতি % প্রবেশ_blocksize;
    }
  অন্যথা যদি (গুনতি != INTMAX_MAX)
    max_records = গুনতি;

  যদি (seek_B)
    output_flags |= O_SEEK_BYTES;
  যদি (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  অন্যথা যদি (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (প্রবেশ_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (প্রবেশ_flags | output_flags) & O_DIRECT));

  iread_fnc = ((প্রবেশ_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  প্রবেশ_flags &= ~O_FULLBLOCK;

  যদি (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  যদি (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  যদি (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  যদি (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  যদি (multiple_bits_set (প্রবেশ_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  যদি (প্রবেশ_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      প্রবেশ_flags &= ~O_NOCACHE;
    }
  যদি (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_records == 0 && max_bytes == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

স্থির শূন্য
apply_translations (শূন্য)
{
  যদি (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  যদি (conversions_mask & C_UCASE)
    {
      জন্য (পূর্ণসংখ্যা i = 0; i < 256; i++)
        trans_table[i] = toupper (trans_table[i]);
      translation_needed = true;
    }
  অন্যথা যদি (conversions_mask & C_LCASE)
    {
      জন্য (পূর্ণসংখ্যা i = 0; i < 256; i++)
        trans_table[i] = tolower (trans_table[i]);
      translation_needed = true;
    }

  যদি (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  অন্যথা যদি (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

স্থির শূন্য
translate_বাফার (অক্ষর *buf, idx_t nread)
{
  অক্ষর *cp = buf;
  জন্য (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

স্থির অক্ষর *
swab_বাফার (অক্ষর *buf, idx_t *nread, পূর্ণসংখ্যা *saved_byte)
{
  যদি (*nread == 0)
    ফেরত buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  পূর্ণসংখ্যা পূর্ব_saved = *saved_byte;
  যদি ((পূর্ব_saved < 0) == (*nread & 1))
    {
      unsigned অক্ষর c = buf[--*nread];
      *saved_byte = c;
    }
  অন্যথা
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  জন্য (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  যদি (পূর্ব_saved < 0)
    ফেরত buf + 1;

  buf[1] = পূর্ব_saved;
  ++*nread;
  ফেরত buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

স্থির শূন্য
advance_প্রবেশ_offset (intmax_t offset)
{
  যদি (0 <= প্রবেশ_offset && ckd_add (&প্রবেশ_offset, প্রবেশ_offset, offset))
    প্রবেশ_offset = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

স্থির intmax_t
skip (পূর্ণসংখ্যা fdesc, অক্ষর ধ্রুবক *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t offset;
  যদি (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, SEEK_CUR))
    {
      যদি (fdesc == STDIN_FILENO)
        {
           কাঠামো stat st;
           যদি (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           যদি (usable_st_আকার (&st) && 0 < st.st_আকার && 0 <= প্রবেশ_offset
               && st.st_আকার - প্রবেশ_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_আকার ) / blocksize;
               offset = st.st_আকার - প্রবেশ_offset;
             }
           অন্যথা
             records = 0;
           advance_প্রবেশ_offset (offset);
        }
      অন্যথা
        {
          records = 0;
          *bytes = 0;
        }
      ফেরত records;
    }
  অন্যথা
    {
      পূর্ণসংখ্যা lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      যদি (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          যদি (!lseek_errno)
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

      অক্ষর *buf;
      যদি (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      অন্যথা
        {
          alloc_obuf ();
          buf = obuf;
        }

      করো
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          যদি (nread < 0)
            {
              যদি (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  যদি (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              অন্যথা
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          অন্যথা যদি (nread == 0)
            বিরতি;
          অন্যথা যদি (fdesc == STDIN_FILENO)
            advance_প্রবেশ_offset (nread);

          যদি (records != 0)
            records--;
          অন্যথা
            *bytes = 0;
        }
      যখন (records || *bytes);

      ফেরত records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

স্থির bool
advance_প্রবেশ_after_read_error (idx_t nbytes)
{
  যদি (! প্রবেশ_seekable)
    {
      যদি (প্রবেশ_seek_errno == ESPIPE)
        ফেরত true;
      errno = প্রবেশ_seek_errno;
    }
  অন্যথা
    {
      off_t offset;
      advance_প্রবেশ_offset (nbytes);
      যদি (প্রবেশ_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (প্রবেশ_file));
          ফেরত false;
        }
      offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
      যদি (0 <= offset)
        {
          off_t diff;
          যদি (offset == প্রবেশ_offset)
            ফেরত true;
          diff = প্রবেশ_offset - offset;
          যদি (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          যদি (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            ফেরত true;
          যদি (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (প্রবেশ_file));
  ফেরত false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

স্থির শূন্য
copy_simple (অক্ষর ধ্রুবক *buf, idx_t nread)
{
  অক্ষর ধ্রুবক *start = buf;	/* First uncopied char in BUF.  */

  করো
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      memcpy (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      যদি (oc >= output_blocksize)
        write_output ();
    }
  যখন (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

স্থির শূন্য
copy_with_block (অক্ষর ধ্রুবক *buf, idx_t nread)
{
  জন্য (idx_t i = nread; i; i--, buf++)
    {
      যদি (*buf == newline_character)
        {
          যদি (col < conversion_blocksize)
            {
              জন্য (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      অন্যথা
        {
          যদি (col == conversion_blocksize)
            r_truncate++;
          অন্যথা যদি (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

স্থির শূন্য
copy_with_unblock (অক্ষর ধ্রুবক *buf, idx_t nread)
{
  স্থির idx_t pending_spaces = 0;

  জন্য (idx_t i = 0; i < nread; i++)
    {
      অক্ষর c = buf[i];

      যদি (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      অন্যথা যদি (c == space_character)
        pending_spaces++;
      অন্যথা
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          যখন (pending_spaces)
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

স্থির শূন্য
set_fd_flags (পূর্ণসংখ্যা fd, পূর্ণসংখ্যা add_flags, অক্ষর ধ্রুবক *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  যদি (add_flags)
    {
      পূর্ণসংখ্যা old_flags = fcntl (fd, F_GETFL);
      পূর্ণসংখ্যা new_flags = old_flags | add_flags;
      bool ok = true;
      যদি (old_flags < 0)
        ok = false;
      অন্যথা যদি (old_flags != new_flags)
        {
          যদি (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              কাঠামো stat st;
              যদি (ifstat (fd, &st) != 0)
                ok = false;
              অন্যথা যদি ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              অন্যথা যদি ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          যদি (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      যদি (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

স্থির পূর্ণসংখ্যা
dd_copy (শূন্য)
{
  অক্ষর *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  পূর্ণসংখ্যা exit_status = EXIT_SUCCESS;
  idx_t n_bytes_read;

  যদি (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, প্রবেশ_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t প্রবেশ_offset0 = প্রবেশ_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, প্রবেশ_file,
                                 skip_records, প্রবেশ_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      যদি ((us_blocks
           || (0 <= প্রবেশ_offset
               && (us_bytes_overflow
                   || us_bytes != প্রবেশ_offset - প্রবেশ_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (প্রবেশ_file));
        }
    }

  যদি (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      যদি (write_records != 0 || bytes != 0)
        {
          memset (obuf, 0, write_records ? output_blocksize : bytes);

          করো
            {
              idx_t আকার_2 = write_records ? output_blocksize : bytes;
              যদি (iwrite (STDOUT_FILENO, obuf, আকার_2) != আকার_2)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              যদি (write_records != 0)
                write_records--;
              অন্যথা
                bytes = 0;
            }
          যখন (write_records || bytes);
        }
    }

  যদি (max_records == 0 && max_bytes == 0)
    ফেরত exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  পূর্ণসংখ্যা saved_byte = -1;

  যখন (true)
    {
      যদি (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          যদি (পরবর্তী_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              পরবর্তী_time += XTIME_PRECISION;
            }
        }

      যদি (r_partial + r_full >= max_records + !!max_bytes)
        বিরতি;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      যদি ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        memset (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                প্রবেশ_blocksize);

      যদি (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      অন্যথা
        nread = iread_fnc (STDIN_FILENO, ibuf, প্রবেশ_blocksize);

      যদি (nread > 0)
        {
          advance_প্রবেশ_offset (nread);
          যদি (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      অন্যথা যদি (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          বিরতি;			/* EOF.  */
        }
      অন্যথা
        {
          যদি (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (প্রবেশ_file));

          যদি (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = প্রবেশ_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              যদি (!advance_প্রবেশ_after_read_error (bad_portion))
                {
                  exit_status = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  প্রবেশ_seekable = false;
                  প্রবেশ_seek_errno = ESPIPE;
                }
              যদি ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              অন্যথা
                চালু;
            }
          অন্যথা
            {
              /* Write any partial block. */
              exit_status = EXIT_FAILURE;
              বিরতি;
            }
        }

      n_bytes_read = nread;

      যদি (n_bytes_read < প্রবেশ_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          যদি (conversions_mask & C_SYNC)
            {
              যদি (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        প্রবেশ_blocksize - n_bytes_read);
              n_bytes_read = প্রবেশ_blocksize;
            }
        }
      অন্যথা
        {
          r_full++;
          partread = 0;
        }

      যদি (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          যদি (nwritten != n_bytes_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              যদি (nwritten != 0)
                w_partial++;
              ফেরত EXIT_FAILURE;
            }
          অন্যথা যদি (n_bytes_read == প্রবেশ_blocksize)
            w_full++;
          অন্যথা
            w_partial++;
          চালু;
        }

      /* Do any translations on the whole buffer at once.  */

      যদি (translation_needed)
        translate_বাফার (ibuf, n_bytes_read);

      যদি (conversions_mask & C_SWAB)
        bufstart = swab_বাফার (ibuf, &n_bytes_read, &saved_byte);
      অন্যথা
        bufstart = ibuf;

      যদি (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      অন্যথা যদি (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      অন্যথা
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  যদি (0 <= saved_byte)
    {
      অক্ষর saved_char = saved_byte;
      যদি (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      অন্যথা যদি (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      অন্যথা
        output_char (saved_char);
    }

  যদি ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      জন্য (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  যদি (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  যদি (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      যদি (nwritten != 0)
        w_partial++;
      যদি (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          ফেরত EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  যদি (final_op_was_seek)
    {
      কাঠামো stat stdout_stat;
      যদি (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          ফেরত EXIT_FAILURE;
        }
      যদি (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          যদি (0 <= output_offset && stdout_stat.st_আকার < output_offset)
            {
              যদি (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  ফেরত EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  যদি (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  ফেরত exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

স্থির পূর্ণসংখ্যা
synchronize_output (শূন্য)
{
  পূর্ণসংখ্যা exit_status = 0;
  পূর্ণসংখ্যা mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  যদি ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      যদি (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = EXIT_FAILURE;
        }
      mask |= C_FSYNC;
    }

  যদি ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      ফেরত EXIT_FAILURE;
    }

  ফেরত exit_status;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  off_t offset;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  page_আকার = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE, Version,
                                   true, usage, AUTHORS,
                                   (অক্ষর ধ্রুবক *) ফাঁকা);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  জন্য (পূর্ণসংখ্যা i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  যদি (প্রবেশ_file == ফাঁকা)
    {
      প্রবেশ_file = _("standard input");
      set_fd_flags (STDIN_FILENO, প্রবেশ_flags, প্রবেশ_file);
    }
  অন্যথা
    {
      যদি (ifd_reopen (STDIN_FILENO, প্রবেশ_file, O_RDONLY | প্রবেশ_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (প্রবেশ_file));
    }

  offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  প্রবেশ_seekable = (0 <= offset);
  প্রবেশ_offset = MAX (0, offset);
  প্রবেশ_seek_errno = errno;

  যদি (output_file == ফাঁকা)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  অন্যথা
    {
      mode_t perms = MODE_RW_UGO;
      পূর্ণসংখ্যা opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t আকার_2;
      যদি ((ckd_mul (&আকার_2, seek_records, output_blocksize)
           || ckd_add (&আকার_2, seek_bytes, আকার_2))
          && !(conversions_mask & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_records, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      যদি ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      যদি (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          যদি (iftruncate (STDOUT_FILENO, আকার_2) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              পূর্ণসংখ্যা ftruncate_errno = errno;
              কাঠামো stat stdout_stat;
              যদি (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              অন্যথা যদি (S_ISREG (stdout_stat.st_mode)
                       || S_ISDIR (stdout_stat.st_mode)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = আকার_2;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  start_time = gethrxtime ();
  পরবর্তী_time = start_time + XTIME_PRECISION;

  পূর্ণসংখ্যা copy_status = dd_copy ();
  পূর্ণসংখ্যা sync_status = synchronize_output ();
  পূর্ণসংখ্যা exit_status = copy_status | sync_status;

  যদি (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      যদি (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (প্রবেশ_file));
          exit_status = EXIT_FAILURE;
        }
      যদি (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = EXIT_FAILURE;
        }
    }
  অন্যথা
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      যদি (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      যদি (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
