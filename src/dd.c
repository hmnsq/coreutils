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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "alignalloc.h"
#ଅନ୍ତର୍ଭାଵ "close-stream.h"
#ଅନ୍ତର୍ଭାଵ "fd-reopen.h"
#ଅନ୍ତର୍ଭାଵ "gethrxtime.h"
#ଅନ୍ତର୍ଭାଵ "human.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "long-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "xtime.h"

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
  କର						\
    {						\
      obuf[oc++] = (c);				\
      ଯଦି (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  ଯେତେବେଳେ (0)

/* Default input and output blocksize. */
#define DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
ଗଣନା
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
ଗଣନା
  {
    STATUS_NONE = 1,
    STATUS_NOXFER = 2,
    STATUS_DEFAULT = 3,
    STATUS_PROGRESS = 4
  };

/* The name of the input file, or NULL for the standard input. */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ନିବେଶ_file = ଖାଲି;

/* The name of the output file, or NULL for the standard output. */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *output_file = ଖାଲି;

/* The page size on this host.  */
ସ୍ଥିର idx_t page_size;

/* The number of bytes in which atomic reads are done. */
ସ୍ଥିର idx_t ନିବେଶ_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ସ୍ଥିର idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ସ୍ଥିର idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ସ୍ଥିର intmax_t skip_records = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ସ୍ଥିର idx_t skip_bytes = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ସ୍ଥିର intmax_t seek_records = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ସ୍ଥିର intmax_t seek_bytes = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ସ୍ଥିର bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ସ୍ଥିର intmax_t max_records = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ସ୍ଥିର idx_t max_bytes = 0;

/* Bit vector of conversions to apply. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା conversions_mask = 0;

/* Open flags for the input and output files.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_flags = 0;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_flags = 0;

/* Status flags for what is printed to stderr.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା status_level = STATUS_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ସ୍ଥିର bool translation_needed = false;

/* Number of partial blocks written. */
ସ୍ଥିର intmax_t w_partial = 0;

/* Number of full blocks written. */
ସ୍ଥିର intmax_t w_full = 0;

/* Number of partial blocks read. */
ସ୍ଥିର intmax_t r_partial = 0;

/* Number of full blocks read. */
ସ୍ଥିର intmax_t r_full = 0;

/* Number of bytes written.  */
ସ୍ଥିର intmax_t w_bytes = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ସ୍ଥିର intmax_t reported_w_bytes = -1;

/* Time that dd started.  */
ସ୍ଥିର xtime_t start_time;

/* Next time to report periodic progress.  */
ସ୍ଥିର xtime_t next_time;

/* If positive, the number of bytes output in the current progress line.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା progress_len;

/* True if input is seekable.  */
ସ୍ଥିର bool ନିବେଶ_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ସ୍ଥିର off_t ନିବେଶ_offset;

/* True if a partial read should be diagnosed.  */
ସ୍ଥିର bool warn_partial_read;

/* Records truncated by conv=block. */
ସ୍ଥିର intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
ସ୍ଥିର ଅକ୍ଷର newline_character = '\n';
ସ୍ଥିର ଅକ୍ଷର space_character = ' ';

/* I/O buffers.  */
ସ୍ଥିର ଅକ୍ଷର *ibuf;
ସ୍ଥିର ଅକ୍ଷର *obuf;

/* Current index into 'obuf'. */
ସ୍ଥିର idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
ସ୍ଥିର idx_t col = 0;

/* The set of signals that are caught.  */
ସ୍ଥିର sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
ସ୍ଥିର sig_atomic_t ଅସ୍ଥିର interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
ସ୍ଥିର sig_atomic_t ଅସ୍ଥିର info_signal_ଗଣନା;

/* Whether to discard cache for input or output.  */
ସ୍ଥିର bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ସ୍ଥିର bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ସ୍ଥିର ssize_t (*iread_fnc) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buf, idx_t size);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_ପ୍ରତୀକ "count_bytes"

/* A symbol and the corresponding integer value.  */
ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ
{
  ଅକ୍ଷର ପ୍ରତୀକ[ଆକାର LONGEST_ପ୍ରତୀକ];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
};

/* Conversion symbols, for conv="...".  */
ସ୍ଥିର ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ ଅଚଳ conversions[] =
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
ଗଣନା
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ଦ୍ୱିମିକ
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

    O_ଗଣନା_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_ଗଣନା_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_ଗଣନା_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_ଗଣନା_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
ସ୍ଥିର ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ ଅଚଳ flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ଦ୍ୱିମିକ},
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
  {"count_bytes", O_ଗଣନା_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
ସ୍ଥିର ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ ଅଚଳ statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ସ୍ଥିର unsigned ଅକ୍ଷର trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ ascii_to_ebcdic[] =
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ ascii_to_ibm[] =
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ ebcdic_to_ascii[] =
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
ସ୍ଥିର bool close_stdout_required = true;

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
ସ୍ଥିର ଶୂନ୍ୟ
maybe_close_stdout (ଶୂନ୍ୟ)
{
  ଯଦି (close_stdout_required)
    close_stdout ();
  ଅନ୍ୟଥା ଯଦି (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ସ୍ଥିର ଶୂନ୍ୟ
diagnose (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errnum, ଅକ୍ଷର ଅଚଳ *fmt, ...)
{
  ଯଦି (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  va_list ap;
  va_start (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_end (ap);
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  bs=BYTES        ପଠ and ଲିଖ up to BYTES bytes at a time (ପୂର୍ବନିର୍ଦ୍ଧାରିତ: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a time\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated ପ୍ରତୀକ list\n\
"));
      oputs (_("\
  ଗଣନା_2=N         copy only N ନିବେଶ blocks\n\
"));
      oputs (_("\
  ibs=BYTES       ପଠ up to BYTES bytes at a time (ପୂର୍ବନିର୍ଦ୍ଧାରିତ: 512)\n\
"));
      oputs (_("\
  ଯଦି=FILE         ପଠ from FILE instead of standard ନିବେଶ\n\
"));
      oputs (_("\
  iflag=FLAGS     ପଠ as per the comma separated ପ୍ରତୀକ list\n\
"));
      oputs (_("\
  obs=BYTES       ଲିଖ BYTES bytes at a time (ପୂର୍ବନିର୍ଦ୍ଧାରିତ: 512)\n\
"));
      oputs (_("\
  of=FILE         ଲିଖ to FILE instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     ଲିଖ as per the comma separated ପ୍ରତୀକ list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized ନିବେଶ blocks\n\
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
GB=1000*1000*1000, G=1024*1024*1024, and so on ପାଇଁ T, P, E, Z, Y, R, Q.\n\
ଦ୍ୱିମିକ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV ପ୍ରତୀକ may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-size\n\
  unblock   replace trailing spaces in cbs-size records with newline\n\
  lcase     change upper ସ୍ଥିତି to lower ସ୍ଥିତି\n\
  ucase     change lower ସ୍ଥିତି to upper ସ୍ଥିତି\n\
  sparse    try to seek rather than ଲିଖ all-NUL output blocks\n\
  swab      swap every pair of ନିବେଶ bytes\n\
  sync      pad every ନିବେଶ block with NULs to ibs-size; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail ଯଦି the output file already exists\n\
  nocreat   କର not create the output file\n\
  notrunc   କର not truncate the output file\n\
  noerror   ଜାରି after ପଠ errors\n\
  fdatasync  physically ଲିଖ output file data before finishing\n\
  fsync     likewise, but also ଲିଖ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG ପ୍ରତୀକ may be:\n\
\n\
  append    append mode (makes sense only ପାଇଁ output; conv=notrunc suggested)\n\
"), stdout);
      ଯଦି (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      ଯଦି (O_DIRECT)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      ଯଦି (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      ଯଦି (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      ଯଦି (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      ଯଦି (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#ଯଦି HAVE_POSIX_FADVISE
      ଯଦି (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#endif
      ଯଦି (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      ଯଦି (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      ଯଦି (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      ଯଦି (O_ଦ୍ୱିମିକ)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      ଯଦି (O_TEXT)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        printf (_("\
\n\
Sending a %s ସଙ୍କେତ to a running 'dd' process makes it\n\
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

ଗଣନା { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ସ୍ଥିର ଶୂନ୍ୟ
alloc_ibuf (ଶୂନ୍ୟ)
{
  ଯଦି (ibuf)
    ଫେରନ୍ତୁ;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_size, ନିବେଶ_blocksize + extra_byte_for_swab);
  ଯଦି (!ibuf)
    {
      ଅକ୍ଷର hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             ନିବେଶ_blocksize,
             human_readable (ନିବେଶ_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ସ୍ଥିର ଶୂନ୍ୟ
alloc_obuf (ଶୂନ୍ୟ)
{
  ଯଦି (obuf)
    ଫେରନ୍ତୁ;

  ଯଦି (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_size, output_blocksize);
      ଯଦି (!obuf)
        {
          ଅକ୍ଷର hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  ଅନ୍ୟଥା
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
translate_charset (ଅକ୍ଷର ଅଚଳ *new_trans)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
multiple_bits_set (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
  ଫେରନ୍ତୁ MULTIPLE_BITS_SET (i);
}

ସ୍ଥିର bool
abbreviation_lacks_prefix (ଅକ୍ଷର ଅଚଳ *message)
{
  ଫେରନ୍ତୁ message[strlen (message) - 2] == ' ';
}

/* Print transfer statistics.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ slash_s[] = "/s";
  ଅକ୍ଷର hbuf[3][LONGEST_HUMAN_READABLE + ଆକାର slash_s];
  ଦ୍ୱିଗୁଣ delta_s;
  ଅକ୍ଷର ଅଚଳ *bytes_per_second;
  ଅକ୍ଷର ଅଚଳ *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  ଅକ୍ଷର ଅଚଳ *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  ଅକ୍ଷର *bpsbuf = hbuf[2];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bpsbufsize = ଆକାର hbuf[2];
  ଯଦି (start_time < now)
    {
      ଦ୍ୱିଗୁଣ XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  ଅନ୍ୟଥା
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  ଯଦି (progress_time)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  ଅକ୍ଷର delta_s_buf[24];
  snprintf (delta_s_buf, ଆକାର delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା stats_len
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

  ଯଦି (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      ଯଦି (0 <= stats_len && stats_len < progress_len)
        fprintf (stderr, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  ଅନ୍ୟଥା
    fputc ('\n', stderr);

  reported_w_bytes = w_bytes;
}

ସ୍ଥିର ଶୂନ୍ୟ
print_stats (ଶୂନ୍ୟ)
{
  ଯଦି (status_level == STATUS_NONE)
    ଫେରନ୍ତୁ;

  ଯଦି (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  ଯଦି (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  ଯଦି (status_level == STATUS_NOXFER)
    ଫେରନ୍ତୁ;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ସ୍ଥିର ଶୂନ୍ୟ
interrupt_handler (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  ଯଦି (! SA_RESETHAND)
    ସଙ୍କେତ (sig, SIG_DFL);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ସ୍ଥିର ଶୂନ୍ୟ
siginfo_handler (MAYBE_UNUSED ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  info_signal_ଗଣନା++;
}

/* Install the signal handlers.  */

ସ୍ଥିର ଶୂନ୍ୟ
install_signal_handlers (ଶୂନ୍ୟ)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  ସଂରଚନା sigaction act;
  sigemptyset (&caught_signals);
  ଯଦି (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (SIGINT, ଖାଲି, &act);
  ଯଦି (act.sa_handler != SIG_IGN)
    sigaddset (&caught_signals, SIGINT);
  act.sa_mask = caught_signals;

  ଯଦି (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ଖାଲି);
    }

  ଯଦି (sigismember (&caught_signals, SIGINT))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ଖାଲି);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
iclose (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ଯଦି (close (fd) != 0)
    କର
      ଯଦି (errno != EINTR)
        ଫେରନ୍ତୁ -1;
    ଯେତେବେଳେ (close (fd) != 0 && errno != EBADF);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା synchronize_output (ଶୂନ୍ୟ);

ସ୍ଥିର ଶୂନ୍ୟ
cleanup (ଶୂନ୍ୟ)
{
  ଯଦି (!interrupt_signal)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_status = synchronize_output ();
      ଯଦି (sync_status)
        exit (sync_status);
    }

  ଯଦି (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (ନିବେଶ_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  ଯଦି (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

ସ୍ଥିର ଶୂନ୍ୟ
process_signals (ଶୂନ୍ୟ)
{
  ଯେତେବେଳେ (interrupt_signal || info_signal_ଗଣନା)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା interrupt;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_ଗଣନା;

      ଯଦି (infos)
        info_signal_ଗଣନା = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ଖାଲି);

      ଯଦି (interrupt)
        cleanup ();
      print_stats ();
      ଯଦି (interrupt)
        ଉଠାଅ (interrupt);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
finish_up (ଶୂନ୍ୟ)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_signals ();
  cleanup ();
  print_stats ();
}

ସ୍ଥିର ଶୂନ୍ୟ
quit (ପୂର୍ଣ୍ଣସଂଖ୍ୟା code)
{
  finish_up ();
  exit (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ସ୍ଥିର off_t
cache_round (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t len)
{
  ସ୍ଥିର off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  ଯଦି (len)
    {
      intmax_t c_pending;
      ଯଦି (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      ଯଦି (c_pending > *pending)
        len = c_pending - *pending;
      ଅନ୍ୟଥା
        len = 0;
    }
  ଅନ୍ୟଥା
    len = *pending;

  ଫେରନ୍ତୁ len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

ସ୍ଥିର bool
invalidate_cache (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t len)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  ଯଦି (len && !clen)
    ଫେରନ୍ତୁ true; /* Don't advise this time.  */
  ଅନ୍ୟଥା ଯଦି (! len && ! clen && ! nocache_eof)
    ଫେରନ୍ତୁ true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  ଯଦି (fd == STDIN_FILENO)
    {
      ଯଦି (ନିବେଶ_seekable)
        offset = ନିବେଶ_offset;
      ଅନ୍ୟଥା
        {
          offset = -1;
          errno = ESPIPE;
        }
    }
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର off_t output_offset = -2;

      ଯଦି (output_offset != -1)
        {
          ଯଦି (output_offset < 0)
            output_offset = lseek (fd, 0, SEEK_CUR);
          ଅନ୍ୟଥା ଯଦି (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  ଯଦି (0 <= offset)
   {
     ଯଦି (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#ଯଦି HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     ଯଦି (clen == 0)
       offset -= offset % page_size;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#ଅନ୍ୟଥା
     errno = ENOTSUP;
#endif
   }

  ଫେରନ୍ତୁ adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ସ୍ଥିର ssize_t
iread (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buf, idx_t size)
{
  ssize_t nread;
  ସ୍ଥିର ssize_t prev_nread;

  କର
    {
      process_signals ();
      nread = ପଠ (fd, buf, size);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      ଯଦି (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < size
          && (ନିବେଶ_flags & O_DIRECT))
        {
          errno = 0;
          nread = 0;
        }
    }
  ଯେତେବେଳେ (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  ଯଦି (0 < nread && nread < size)
    process_signals ();

  ଯଦି (0 < nread && warn_partial_read)
    {
      ଯଦି (0 < prev_nread && prev_nread < size)
        {
          idx_t prev = prev_nread;
          ଯଦି (status_level != STATUS_NONE)
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
  ଫେରନ୍ତୁ nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
ସ୍ଥିର ssize_t
iread_fullblock (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର *buf, idx_t size)
{
  ssize_t nread = 0;

  ଯେତେବେଳେ (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      ଯଦି (ncurr < 0)
        ଫେରନ୍ତୁ ncurr;
      ଯଦି (ncurr == 0)
        ଭାଙ୍ଗ;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  ଫେରନ୍ତୁ nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ସ୍ଥିର idx_t
iwrite (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅକ୍ଷର ଅଚଳ *buf, idx_t size)
{
  idx_t total_written = 0;

  ଯଦି ((output_flags & O_DIRECT) && size < output_blocksize)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      ଯଦି (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
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

  ଯେତେବେଳେ (total_written < size)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      ଯଦି ((conversions_mask & C_SPARSE) && is_nul (buf, size))
        {
          ଯଦି (lseek (fd, size, SEEK_CUR) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          ଅନ୍ୟଥା
            {
              final_op_was_seek = true;
              nwritten = size;
            }
        }

      ଯଦି (!nwritten)
        nwritten = ଲିଖ (fd, buf + total_written, size - total_written);

      ଯଦି (nwritten < 0)
        {
          ଯଦି (errno != EINTR)
            ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା ଯଦି (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          ଭାଙ୍ଗ;
        }
      ଅନ୍ୟଥା
        total_written += nwritten;
    }

  ଯଦି (o_nocache && total_written)
    invalidate_cache (fd, total_written);

  ଫେରନ୍ତୁ total_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ସ୍ଥିର ଶୂନ୍ୟ
write_output (ଶୂନ୍ୟ)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  ଯଦି (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      ଯଦି (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  ଅନ୍ୟଥା
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ifdatasync (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  କର
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  ଯେତେବେଳେ (ret < 0 && errno == EINTR);

  ଫେରନ୍ତୁ ret;
}

/* Restart on EINTR from fd_reopen.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ifd_reopen (ପୂର୍ଣ୍ଣସଂଖ୍ୟା desired_fd, ଅକ୍ଷର ଅଚଳ *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag, mode_t mode)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  କର
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  ଯେତେବେଳେ (ret < 0 && errno == EINTR);

  ଫେରନ୍ତୁ ret;
}

/* Restart on EINTR from fstat.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ifstat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat *st)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  କର
    {
      process_signals ();
      ret = fstat (fd, st);
    }
  ଯେତେବେଳେ (ret < 0 && errno == EINTR);

  ଫେରନ୍ତୁ ret;
}

/* Restart on EINTR from fsync.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ifsync (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  କର
    {
      process_signals ();
      ret = fsync (fd);
    }
  ଯେତେବେଳେ (ret < 0 && errno == EINTR);

  ଫେରନ୍ତୁ ret;
}

/* Restart on EINTR from ftruncate.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
iftruncate (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t length)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  କର
    {
      process_signals ();
      ret = ftruncate (fd, length);
    }
  ଯେତେବେଳେ (ret < 0 && errno == EINTR);

  ଫେରନ୍ତୁ ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
operand_matches (ଅକ୍ଷର ଅଚଳ *str, ଅକ୍ଷର ଅଚଳ *pattern, ଅକ୍ଷର delim)
{
  ଯେତେବେଳେ (*pattern)
    ଯଦି (*str++ != *pattern++)
      ଫେରନ୍ତୁ false;
  ଫେରନ୍ତୁ !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_symbols (ଅକ୍ଷର ଅଚଳ *str, ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ ଅଚଳ *table,
               bool exclusive, ଅକ୍ଷର ଅଚଳ *error_msgid)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = 0;

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର ଅଚଳ *strcomma = strchr (str, ',');
      ସଂରଚନା ପ୍ରତୀକ_ମୂଲ୍ୟ ଅଚଳ *entry;

      ପାଇଁ (entry = table;
           ! (operand_matches (str, entry->ପ୍ରତୀକ, ',') && entry->ମୂଲ୍ୟ);
           entry++)
        {
          ଯଦି (! entry->ପ୍ରତୀକ[0])
            {
              idx_t slen = strcomma ? strcomma - str : strlen (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      ଯଦି (exclusive)
        ମୂଲ୍ୟ = entry->ମୂଲ୍ୟ;
      ଅନ୍ୟଥା
        ମୂଲ୍ୟ |= entry->ମୂଲ୍ୟ;
      ଯଦି (!strcomma)
        ଭାଙ୍ଗ;
      str = strcomma + 1;
    }

  ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

ସ୍ଥିର intmax_t
parse_integer (ଅକ୍ଷର ଅଚଳ *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା indeterminate = 0;
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t result = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  ଯେତେବେଳେ (true)
    {
      uintmax_t n = indeterminate;
      ଅକ୍ଷର *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      ଯଦି ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          ଯଦି (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      ଯଦି (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      ଯଦି ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          ଯଦି (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              ଫେରନ୍ତୁ indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      ଯଦି (n == 0)
        result = 0;
      ଅନ୍ୟଥା ଯଦି (result != 0)
        {
          intmax_t product;
          ଯଦି ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, result, n))
            overflow = true;
          ଅନ୍ୟଥା
            result = product;
        }

      ଯଦି (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      ଯଦି (! multiply)
        ଭାଙ୍ଗ;
      str = suffix + 1;
    }

  ଯଦି (result == 0)
    {
      ଯଦି (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  ଅନ୍ୟଥା ଯଦି (overflow)
    {
      e = LONGINT_OVERFLOW;
      result = INTMAX_MAX;
    }

  *invalid = e;
  ଫେରନ୍ତୁ result;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
ସ୍ଥିର bool
operand_is (ଅକ୍ଷର ଅଚଳ *operand, ଅକ୍ଷର ଅଚଳ *name)
{
  ଫେରନ୍ତୁ operand_matches (operand, name, '=');
}

ସ୍ଥିର ଶୂନ୍ୟ
scanargs (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର *ଅଚଳ *argv)
{
  idx_t blocksize = 0;
  intmax_t ଗଣନା_2 = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool ଗଣନା_B = false, skip_B = false, seek_B = false;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; i++)
    {
      ଅକ୍ଷର ଅଚଳ *name = argv[i];
      ଅକ୍ଷର ଅଚଳ *val = strchr (name, '=');

      ଯଦି (val == ଖାଲି)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      ଯଦି (operand_is (name, "if"))
        ନିବେଶ_file = val;
      ଅନ୍ୟଥା ଯଦି (operand_is (name, "of"))
        output_file = val;
      ଅନ୍ୟଥା ଯଦି (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      ଅନ୍ୟଥା ଯଦି (operand_is (name, "iflag"))
        ନିବେଶ_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      ଅନ୍ୟଥା ଯଦି (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      ଅନ୍ୟଥା ଯଦି (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      ଅନ୍ୟଥା
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ଖାଲି;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          ଯଦି (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &ନିବେଶ_blocksize;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          ଅନ୍ୟଥା ଯଦି (operand_is (name, "count"))
            {
              ଗଣନା_2 = n;
              ଗଣନା_B = has_B;
            }
          ଅନ୍ୟଥା
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          ଯଦି (n < n_min)
            invalid = LONGINT_INVALID;
          ଅନ୍ୟଥା ଯଦି (n_max < n)
            invalid = LONGINT_OVERFLOW;

          ଯଦି (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          ଅନ୍ୟଥା ଯଦି (converted_idx)
            *converted_idx = n;
        }
    }

  ଯଦି (blocksize)
    ନିବେଶ_blocksize = output_blocksize = blocksize;
  ଅନ୍ୟଥା
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  ଯଦି (ନିବେଶ_blocksize == 0)
    ନିବେଶ_blocksize = DEFAULT_BLOCKSIZE;
  ଯଦି (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  ଯଦି (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  ଯଦି (ନିବେଶ_flags & (O_DSYNC | O_SYNC))
    ନିବେଶ_flags |= O_RSYNC;

  ଯଦି (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (skip_B)
    ନିବେଶ_flags |= O_SKIP_BYTES;
  ଯଦି (ନିବେଶ_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / ନିବେଶ_blocksize;
      skip_bytes = skip % ନିବେଶ_blocksize;
    }
  ଅନ୍ୟଥା ଯଦି (skip != 0)
    skip_records = skip;

  ଯଦି (ଗଣନା_B)
    ନିବେଶ_flags |= O_ଗଣନା_BYTES;
  ଯଦି (ନିବେଶ_flags & O_ଗଣନା_BYTES && ଗଣନା_2 != INTMAX_MAX)
    {
      max_records = ଗଣନା_2 / ନିବେଶ_blocksize;
      max_bytes = ଗଣନା_2 % ନିବେଶ_blocksize;
    }
  ଅନ୍ୟଥା ଯଦି (ଗଣନା_2 != INTMAX_MAX)
    max_records = ଗଣନା_2;

  ଯଦି (seek_B)
    output_flags |= O_SEEK_BYTES;
  ଯଦି (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  ଅନ୍ୟଥା ଯଦି (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (ନିବେଶ_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (ନିବେଶ_flags | output_flags) & O_DIRECT));

  iread_fnc = ((ନିବେଶ_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  ନିବେଶ_flags &= ~O_FULLBLOCK;

  ଯଦି (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  ଯଦି (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  ଯଦି (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  ଯଦି (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  ଯଦି (multiple_bits_set (ନିବେଶ_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  ଯଦି (ନିବେଶ_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      ନିବେଶ_flags &= ~O_NOCACHE;
    }
  ଯଦି (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_records == 0 && max_bytes == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ସ୍ଥିର ଶୂନ୍ୟ
apply_translations (ଶୂନ୍ୟ)
{
  ଯଦି (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  ଯଦି (conversions_mask & C_UCASE)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++)
        trans_table[i] = toupper (trans_table[i]);
      translation_needed = true;
    }
  ଅନ୍ୟଥା ଯଦି (conversions_mask & C_LCASE)
    {
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++)
        trans_table[i] = tolower (trans_table[i]);
      translation_needed = true;
    }

  ଯଦି (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  ଅନ୍ୟଥା ଯଦି (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ସ୍ଥିର ଶୂନ୍ୟ
translate_buffer (ଅକ୍ଷର *buf, idx_t nread)
{
  ଅକ୍ଷର *cp = buf;
  ପାଇଁ (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ସ୍ଥିର ଅକ୍ଷର *
swab_buffer (ଅକ୍ଷର *buf, idx_t *nread, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *saved_byte)
{
  ଯଦି (*nread == 0)
    ଫେରନ୍ତୁ buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_saved = *saved_byte;
  ଯଦି ((prev_saved < 0) == (*nread & 1))
    {
      unsigned ଅକ୍ଷର c = buf[--*nread];
      *saved_byte = c;
    }
  ଅନ୍ୟଥା
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  ପାଇଁ (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  ଯଦି (prev_saved < 0)
    ଫେରନ୍ତୁ buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  ଫେରନ୍ତୁ buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

ସ୍ଥିର ଶୂନ୍ୟ
advance_ନିବେଶ_offset (intmax_t offset)
{
  ଯଦି (0 <= ନିବେଶ_offset && ckd_add (&ନିବେଶ_offset, ନିବେଶ_offset, offset))
    ନିବେଶ_offset = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

ସ୍ଥିର intmax_t
skip (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdesc, ଅକ୍ଷର ଅଚଳ *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t offset;
  ଯଦି (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, SEEK_CUR))
    {
      ଯଦି (fdesc == STDIN_FILENO)
        {
           ସଂରଚନା stat st;
           ଯଦି (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           ଯଦି (usable_st_size (&st) && 0 < st.st_size && 0 <= ନିବେଶ_offset
               && st.st_size - ନିବେଶ_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_size ) / blocksize;
               offset = st.st_size - ନିବେଶ_offset;
             }
           ଅନ୍ୟଥା
             records = 0;
           advance_ନିବେଶ_offset (offset);
        }
      ଅନ୍ୟଥା
        {
          records = 0;
          *bytes = 0;
        }
      ଫେରନ୍ତୁ records;
    }
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      ଯଦି (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          ଯଦି (!lseek_errno)
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

      ଅକ୍ଷର *buf;
      ଯଦି (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      ଅନ୍ୟଥା
        {
          alloc_obuf ();
          buf = obuf;
        }

      କର
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          ଯଦି (nread < 0)
            {
              ଯଦି (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  ଯଦି (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              ଅନ୍ୟଥା
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          ଅନ୍ୟଥା ଯଦି (nread == 0)
            ଭାଙ୍ଗ;
          ଅନ୍ୟଥା ଯଦି (fdesc == STDIN_FILENO)
            advance_ନିବେଶ_offset (nread);

          ଯଦି (records != 0)
            records--;
          ଅନ୍ୟଥା
            *bytes = 0;
        }
      ଯେତେବେଳେ (records || *bytes);

      ଫେରନ୍ତୁ records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ସ୍ଥିର bool
advance_ନିବେଶ_after_read_error (idx_t nbytes)
{
  ଯଦି (! ନିବେଶ_seekable)
    {
      ଯଦି (ନିବେଶ_seek_errno == ESPIPE)
        ଫେରନ୍ତୁ true;
      errno = ନିବେଶ_seek_errno;
    }
  ଅନ୍ୟଥା
    {
      off_t offset;
      advance_ନିବେଶ_offset (nbytes);
      ଯଦି (ନିବେଶ_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (ନିବେଶ_file));
          ଫେରନ୍ତୁ false;
        }
      offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
      ଯଦି (0 <= offset)
        {
          off_t diff;
          ଯଦି (offset == ନିବେଶ_offset)
            ଫେରନ୍ତୁ true;
          diff = ନିବେଶ_offset - offset;
          ଯଦି (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          ଯଦି (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            ଫେରନ୍ତୁ true;
          ଯଦି (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (ନିବେଶ_file));
  ଫେରନ୍ତୁ false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ସ୍ଥିର ଶୂନ୍ୟ
copy_simple (ଅକ୍ଷର ଅଚଳ *buf, idx_t nread)
{
  ଅକ୍ଷର ଅଚଳ *start = buf;	/* First uncopied char in BUF.  */

  କର
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      memcpy (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      ଯଦି (oc >= output_blocksize)
        write_output ();
    }
  ଯେତେବେଳେ (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

ସ୍ଥିର ଶୂନ୍ୟ
copy_with_block (ଅକ୍ଷର ଅଚଳ *buf, idx_t nread)
{
  ପାଇଁ (idx_t i = nread; i; i--, buf++)
    {
      ଯଦି (*buf == newline_character)
        {
          ଯଦି (col < conversion_blocksize)
            {
              ପାଇଁ (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (col == conversion_blocksize)
            r_truncate++;
          ଅନ୍ୟଥା ଯଦି (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

ସ୍ଥିର ଶୂନ୍ୟ
copy_with_unblock (ଅକ୍ଷର ଅଚଳ *buf, idx_t nread)
{
  ସ୍ଥିର idx_t pending_spaces = 0;

  ପାଇଁ (idx_t i = 0; i < nread; i++)
    {
      ଅକ୍ଷର c = buf[i];

      ଯଦି (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      ଅନ୍ୟଥା ଯଦି (c == space_character)
        pending_spaces++;
      ଅନ୍ୟଥା
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          ଯେତେବେଳେ (pending_spaces)
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

ସ୍ଥିର ଶୂନ୍ୟ
set_fd_flags (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_flags, ଅକ୍ଷର ଅଚଳ *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  ଯଦି (add_flags)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_flags = fcntl (fd, F_GETFL);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_flags = old_flags | add_flags;
      bool ok = true;
      ଯଦି (old_flags < 0)
        ok = false;
      ଅନ୍ୟଥା ଯଦି (old_flags != new_flags)
        {
          ଯଦି (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              ସଂରଚନା stat st;
              ଯଦି (ifstat (fd, &st) != 0)
                ok = false;
              ଅନ୍ୟଥା ଯଦି ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              ଅନ୍ୟଥା ଯଦି ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          ଯଦି (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      ଯଦି (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
dd_copy (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = EXIT_SUCCESS;
  idx_t n_bytes_read;

  ଯଦି (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, ନିବେଶ_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t ନିବେଶ_offset0 = ନିବେଶ_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, ନିବେଶ_file,
                                 skip_records, ନିବେଶ_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      ଯଦି ((us_blocks
           || (0 <= ନିବେଶ_offset
               && (us_bytes_overflow
                   || us_bytes != ନିବେଶ_offset - ନିବେଶ_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (ନିବେଶ_file));
        }
    }

  ଯଦି (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      ଯଦି (write_records != 0 || bytes != 0)
        {
          memset (obuf, 0, write_records ? output_blocksize : bytes);

          କର
            {
              idx_t size = write_records ? output_blocksize : bytes;
              ଯଦି (iwrite (STDOUT_FILENO, obuf, size) != size)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              ଯଦି (write_records != 0)
                write_records--;
              ଅନ୍ୟଥା
                bytes = 0;
            }
          ଯେତେବେଳେ (write_records || bytes);
        }
    }

  ଯଦି (max_records == 0 && max_bytes == 0)
    ଫେରନ୍ତୁ exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_byte = -1;

  ଯେତେବେଳେ (true)
    {
      ଯଦି (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          ଯଦି (next_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              next_time += XTIME_PRECISION;
            }
        }

      ଯଦି (r_partial + r_full >= max_records + !!max_bytes)
        ଭାଙ୍ଗ;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      ଯଦି ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        memset (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                ନିବେଶ_blocksize);

      ଯଦି (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      ଅନ୍ୟଥା
        nread = iread_fnc (STDIN_FILENO, ibuf, ନିବେଶ_blocksize);

      ଯଦି (nread > 0)
        {
          advance_ନିବେଶ_offset (nread);
          ଯଦି (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      ଅନ୍ୟଥା ଯଦି (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          ଭାଙ୍ଗ;			/* EOF.  */
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (ନିବେଶ_file));

          ଯଦି (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = ନିବେଶ_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              ଯଦି (!advance_ନିବେଶ_after_read_error (bad_portion))
                {
                  exit_status = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  ନିବେଶ_seekable = false;
                  ନିବେଶ_seek_errno = ESPIPE;
                }
              ଯଦି ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              ଅନ୍ୟଥା
                ଜାରି;
            }
          ଅନ୍ୟଥା
            {
              /* Write any partial block. */
              exit_status = EXIT_FAILURE;
              ଭାଙ୍ଗ;
            }
        }

      n_bytes_read = nread;

      ଯଦି (n_bytes_read < ନିବେଶ_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          ଯଦି (conversions_mask & C_SYNC)
            {
              ଯଦି (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        ନିବେଶ_blocksize - n_bytes_read);
              n_bytes_read = ନିବେଶ_blocksize;
            }
        }
      ଅନ୍ୟଥା
        {
          r_full++;
          partread = 0;
        }

      ଯଦି (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          ଯଦି (nwritten != n_bytes_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              ଯଦି (nwritten != 0)
                w_partial++;
              ଫେରନ୍ତୁ EXIT_FAILURE;
            }
          ଅନ୍ୟଥା ଯଦି (n_bytes_read == ନିବେଶ_blocksize)
            w_full++;
          ଅନ୍ୟଥା
            w_partial++;
          ଜାରି;
        }

      /* Do any translations on the whole buffer at once.  */

      ଯଦି (translation_needed)
        translate_buffer (ibuf, n_bytes_read);

      ଯଦି (conversions_mask & C_SWAB)
        bufstart = swab_buffer (ibuf, &n_bytes_read, &saved_byte);
      ଅନ୍ୟଥା
        bufstart = ibuf;

      ଯଦି (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      ଅନ୍ୟଥା ଯଦି (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      ଅନ୍ୟଥା
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  ଯଦି (0 <= saved_byte)
    {
      ଅକ୍ଷର saved_char = saved_byte;
      ଯଦି (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      ଅନ୍ୟଥା ଯଦି (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      ଅନ୍ୟଥା
        output_char (saved_char);
    }

  ଯଦି ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      ପାଇଁ (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  ଯଦି (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  ଯଦି (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      ଯଦି (nwritten != 0)
        w_partial++;
      ଯଦି (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          ଫେରନ୍ତୁ EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  ଯଦି (final_op_was_seek)
    {
      ସଂରଚନା stat stdout_stat;
      ଯଦି (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          ଫେରନ୍ତୁ EXIT_FAILURE;
        }
      ଯଦି (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          ଯଦି (0 <= output_offset && stdout_stat.st_size < output_offset)
            {
              ଯଦି (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  ଫେରନ୍ତୁ EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  ଯଦି (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  ଫେରନ୍ତୁ exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
synchronize_output (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  ଯଦି ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      ଯଦି (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = EXIT_FAILURE;
        }
      mask |= C_FSYNC;
    }

  ଯଦି ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      ଫେରନ୍ତୁ EXIT_FAILURE;
    }

  ଫେରନ୍ତୁ exit_status;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
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

  page_size = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE, Version,
                                   true, usage, AUTHORS,
                                   (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  ଯଦି (ନିବେଶ_file == ଖାଲି)
    {
      ନିବେଶ_file = _("standard input");
      set_fd_flags (STDIN_FILENO, ନିବେଶ_flags, ନିବେଶ_file);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (ifd_reopen (STDIN_FILENO, ନିବେଶ_file, O_RDONLY | ନିବେଶ_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (ନିବେଶ_file));
    }

  offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ନିବେଶ_seekable = (0 <= offset);
  ନିବେଶ_offset = MAX (0, offset);
  ନିବେଶ_seek_errno = errno;

  ଯଦି (output_file == ଖାଲି)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  ଅନ୍ୟଥା
    {
      mode_t perms = MODE_RW_UGO;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t size;
      ଯଦି ((ckd_mul (&size, seek_records, output_blocksize)
           || ckd_add (&size, seek_bytes, size))
          && !(conversions_mask & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_records, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      ଯଦି ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      ଯଦି (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          ଯଦି (iftruncate (STDOUT_FILENO, size) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା ftruncate_errno = errno;
              ସଂରଚନା stat stdout_stat;
              ଯଦି (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              ଅନ୍ୟଥା ଯଦି (S_ISREG (stdout_stat.st_mode)
                       || S_ISDIR (stdout_stat.st_mode)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = size;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  start_time = gethrxtime ();
  next_time = start_time + XTIME_PRECISION;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_status = dd_copy ();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_status = synchronize_output ();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = copy_status | sync_status;

  ଯଦି (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      ଯଦି (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (ନିବେଶ_file));
          exit_status = EXIT_FAILURE;
        }
      ଯଦି (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = EXIT_FAILURE;
        }
    }
  ଅନ୍ୟଥା
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      ଯଦି (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      ଯଦି (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
