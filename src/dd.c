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

#include <config.h>

#include <sys/types.h>
#include <signal.h>

#include "system.h"
#include "alignalloc.h"
#include "close-stream.h"
#include "fd-reopen.h"
#include "gethrxtime.h"
#include "human.h"
#include "ioblksize.h"
#include "long-options.h"
#include "quote.h"
#include "xstrtol.h"
#include "xtime.h"

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
  do						\
    {						\
      obuf[oc++] = (c);				\
      ꦪꦼꦤ꧀ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  ꦱꦼꦢꦼꦭ (0)

/* Default input and output blocksize. */
#define DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
ga_nanaa
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
ga_nanaa
  {
    STATUS_NONE = 1,
    STATUS_NOXFER = 2,
    STATUS_DEFAULT = 3,
    STATUS_PROGRESS = 4
  };

/* The name of the input file, or NULL for the standard input. */
static var_na const *input_file = NULL;

/* The name of the output file, or NULL for the standard output. */
static var_na const *output_file = NULL;

/* The page size on this host.  */
static idx_t page_size;

/* The number of bytes in which atomic reads are done. */
static idx_t input_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
static idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
static idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
static intmax_t skip_records = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
static idx_t skip_bytes = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
static intmax_t seek_records = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
static intmax_t seek_bytes = 0;

/* Whether the final output was done with a seek (rather than a write).  */
static bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
static intmax_t max_records = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
static idx_t max_bytes = 0;

/* Bit vector of conversions to apply. */
static puur_naa_nka conversions_mask = 0;

/* Open flags for the input and output files.  */
static puur_naa_nka input_flags = 0;
static puur_naa_nka output_flags = 0;

/* Status flags for what is printed to stderr.  */
static puur_naa_nka status_level = STATUS_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
static bool translation_needed = false;

/* Number of partial blocks written. */
static intmax_t w_partial = 0;

/* Number of full blocks written. */
static intmax_t w_full = 0;

/* Number of partial blocks read. */
static intmax_t r_partial = 0;

/* Number of full blocks read. */
static intmax_t r_full = 0;

/* Number of bytes written.  */
static intmax_t w_bytes = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
static intmax_t reported_w_bytes = -1;

/* Time that dd started.  */
static xtime_t start_time;

/* Next time to report periodic progress.  */
static xtime_t next_time;

/* If positive, the number of bytes output in the current progress line.  */
static puur_naa_nka progress_len;

/* True if input is seekable.  */
static bool input_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
static puur_naa_nka input_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
static off_t input_offset;

/* True if a partial read should be diagnosed.  */
static bool warn_partial_read;

/* Records truncated by conv=block. */
static intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
static var_na newline_character = '\n';
static var_na space_character = ' ';

/* I/O buffers.  */
static var_na *ibuf;
static var_na *obuf;

/* Current index into 'obuf'. */
static idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
static idx_t col = 0;

/* The set of signals that are caught.  */
static sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
static sig_atomic_t ca_ncala interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
static sig_atomic_t ca_ncala info_signal_count;

/* Whether to discard cache for input or output.  */
static bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
static bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
static ssize_t (*iread_fnc) (puur_naa_nka fd, var_na *buf, idx_t size);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_SYMBOL "count_bytes"

/* A symbol and the corresponding integer value.  */
sa_nracanaa symbol_value
{
  var_na symbol[parimaa_na LONGEST_SYMBOL];
  puur_naa_nka value;
};

/* Conversion symbols, for conv="...".  */
static sa_nracanaa symbol_value const conversions[] =
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
ga_nanaa
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_BINARY
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

    O_COUNT_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_COUNT_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_COUNT_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_COUNT_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
static sa_nracanaa symbol_value const flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_BINARY},
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
  {"count_bytes", O_COUNT_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
static sa_nracanaa symbol_value const statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
static unsigned var_na trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

static var_na const ascii_to_ebcdic[] =
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

static var_na const ascii_to_ibm[] =
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

static var_na const ebcdic_to_ascii[] =
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
static bool close_stdout_required = true;

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
static void
maybe_close_stdout (void)
{
  ꦪꦼꦤ꧀ (close_stdout_required)
    close_stdout ();
  ꦭꦶꦪ ꦪꦼꦤ꧀ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
static void
diagnose (puur_naa_nka errnum, var_na const *fmt, ...)
{
  ꦪꦼꦤ꧀ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  va_list ap;
  va_start (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_end (ap);
}

void
usage (puur_naa_nka status)
{
  ꦪꦼꦤ꧀ (status != EXIT_SUCCESS)
    emit_try_help ();
  ꦭꦶꦪ
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
  bs=BYTES        read and write up to BYTES bytes at a time (default: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a time\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated symbol list\n\
"));
      oputs (_("\
  count=N         copy only N input blocks\n\
"));
      oputs (_("\
  ibs=BYTES       read up to BYTES bytes at a time (default: 512)\n\
"));
      oputs (_("\
  ꦪꦼꦤ꧀=FILE         read from FILE instead of standard input\n\
"));
      oputs (_("\
  iflag=FLAGS     read as per the comma separated symbol list\n\
"));
      oputs (_("\
  obs=BYTES       write BYTES bytes at a time (default: 512)\n\
"));
      oputs (_("\
  of=FILE         write to FILE instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     write as per the comma separated symbol list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized input blocks\n\
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
GB=1000*1000*1000, G=1024*1024*1024, and so on ꦏꦁꦒꦺꦴ T, P, E, Z, Y, R, Q.\n\
Binary prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV symbol may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-size\n\
  unblock   replace trailing spaces in cbs-size records with newline\n\
  lcase     change upper pak_sa to lower pak_sa\n\
  ucase     change lower pak_sa to upper pak_sa\n\
  sparse    try to seek rather than write all-NUL output blocks\n\
  swab      swap every pair of input bytes\n\
  sync      pad every input block with NULs to ibs-size; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail ꦪꦼꦤ꧀ the output file already exists\n\
  nocreat   do not create the output file\n\
  notrunc   do not truncate the output file\n\
  noerror   continue after read errors\n\
  fdatasync  physically write output file data before finishing\n\
  fsync     likewise, but also write metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG symbol may be:\n\
\n\
  append    append mode (makes sense only ꦏꦁꦒꦺꦴ output; conv=notrunc suggested)\n\
"), stdout);
      ꦪꦼꦤ꧀ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      ꦪꦼꦤ꧀ (O_DIRECT)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      ꦪꦼꦤ꧀ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      ꦪꦼꦤ꧀ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      ꦪꦼꦤ꧀ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      ꦪꦼꦤ꧀ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#ꦪꦼꦤ꧀ HAVE_POSIX_FADVISE
      ꦪꦼꦤ꧀ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#endif
      ꦪꦼꦤ꧀ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      ꦪꦼꦤ꧀ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      ꦪꦼꦤ꧀ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      ꦪꦼꦤ꧀ (O_BINARY)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      ꦪꦼꦤ꧀ (O_TEXT)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        printf (_("\
\n\
Sending a %s sa_nkeeta to a running 'dd' process makes it\n\
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

ga_nanaa { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

static void
alloc_ibuf (void)
{
  ꦪꦼꦤ꧀ (ibuf)
    ꦮꦭꦶ;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_size, input_blocksize + extra_byte_for_swab);
  ꦪꦼꦤ꧀ (!ibuf)
    {
      var_na hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             input_blocksize,
             human_readable (input_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

static void
alloc_obuf (void)
{
  ꦪꦼꦤ꧀ (obuf)
    ꦮꦭꦶ;

  ꦪꦼꦤ꧀ (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_size, output_blocksize);
      ꦪꦼꦤ꧀ (!obuf)
        {
          var_na hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  ꦭꦶꦪ
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

static void
translate_charset (var_na const *new_trans)
{
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

static anta_hstha bool
multiple_bits_set (puur_naa_nka i)
{
  ꦮꦭꦶ MULTIPLE_BITS_SET (i);
}

static bool
abbreviation_lacks_prefix (var_na const *message)
{
  ꦮꦭꦶ message[strlen (message) - 2] == ' ';
}

/* Print transfer statistics.  */

static void
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  static var_na const slash_s[] = "/s";
  var_na hbuf[3][LONGEST_HUMAN_READABLE + parimaa_na slash_s];
  dviguu_na delta_s;
  var_na const *bytes_per_second;
  var_na const *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  var_na const *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  var_na *bpsbuf = hbuf[2];
  puur_naa_nka bpsbufsize = parimaa_na hbuf[2];
  ꦪꦼꦤ꧀ (start_time < now)
    {
      dviguu_na XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  ꦭꦶꦪ
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  ꦪꦼꦤ꧀ (progress_time)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  var_na delta_s_buf[24];
  snprintf (delta_s_buf, parimaa_na delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  puur_naa_nka stats_len
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

  ꦪꦼꦤ꧀ (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      ꦪꦼꦤ꧀ (0 <= stats_len && stats_len < progress_len)
        fprintf (stderr, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  ꦭꦶꦪ
    fputc ('\n', stderr);

  reported_w_bytes = w_bytes;
}

static void
print_stats (void)
{
  ꦪꦼꦤ꧀ (status_level == STATUS_NONE)
    ꦮꦭꦶ;

  ꦪꦼꦤ꧀ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  ꦪꦼꦤ꧀ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  ꦪꦼꦤ꧀ (status_level == STATUS_NOXFER)
    ꦮꦭꦶ;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

static void
interrupt_handler (puur_naa_nka sig)
{
  ꦪꦼꦤ꧀ (! SA_RESETHAND)
    sa_nkeeta (sig, SIG_DFL);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

static void
siginfo_handler (MAYBE_UNUSED puur_naa_nka sig)
{
  info_signal_count++;
}

/* Install the signal handlers.  */

static void
install_signal_handlers (void)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  sa_nracanaa sigaction act;
  sigemptyset (&caught_signals);
  ꦪꦼꦤ꧀ (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (SIGINT, NULL, &act);
  ꦪꦼꦤ꧀ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_signals, SIGINT);
  act.sa_mask = caught_signals;

  ꦪꦼꦤ꧀ (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, NULL);
    }

  ꦪꦼꦤ꧀ (sigismember (&caught_signals, SIGINT))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, NULL);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
static puur_naa_nka
iclose (puur_naa_nka fd)
{
  ꦪꦼꦤ꧀ (close (fd) != 0)
    do
      ꦪꦼꦤ꧀ (errno != EINTR)
        ꦮꦭꦶ -1;
    ꦱꦼꦢꦼꦭ (close (fd) != 0 && errno != EBADF);

  ꦮꦭꦶ 0;
}

static puur_naa_nka synchronize_output (void);

static void
cleanup (void)
{
  ꦪꦼꦤ꧀ (!interrupt_signal)
    {
      puur_naa_nka sync_status = synchronize_output ();
      ꦪꦼꦤ꧀ (sync_status)
        exit (sync_status);
    }

  ꦪꦼꦤ꧀ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (input_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  ꦪꦼꦤ꧀ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

static void
process_signals (void)
{
  ꦱꦼꦢꦼꦭ (interrupt_signal || info_signal_count)
    {
      puur_naa_nka interrupt;
      puur_naa_nka infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_count;

      ꦪꦼꦤ꧀ (infos)
        info_signal_count = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, NULL);

      ꦪꦼꦤ꧀ (interrupt)
        cleanup ();
      print_stats ();
      ꦪꦼꦤ꧀ (interrupt)
        raise (interrupt);
    }
}

static void
finish_up (void)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_signals ();
  cleanup ();
  print_stats ();
}

static void
quit (puur_naa_nka code)
{
  finish_up ();
  exit (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

static off_t
cache_round (puur_naa_nka fd, off_t len)
{
  static off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  ꦪꦼꦤ꧀ (len)
    {
      intmax_t c_pending;
      ꦪꦼꦤ꧀ (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      ꦪꦼꦤ꧀ (c_pending > *pending)
        len = c_pending - *pending;
      ꦭꦶꦪ
        len = 0;
    }
  ꦭꦶꦪ
    len = *pending;

  ꦮꦭꦶ len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

static bool
invalidate_cache (puur_naa_nka fd, off_t len)
{
  puur_naa_nka adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  ꦪꦼꦤ꧀ (len && !clen)
    ꦮꦭꦶ true; /* Don't advise this time.  */
  ꦭꦶꦪ ꦪꦼꦤ꧀ (! len && ! clen && ! nocache_eof)
    ꦮꦭꦶ true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  ꦪꦼꦤ꧀ (fd == STDIN_FILENO)
    {
      ꦪꦼꦤ꧀ (input_seekable)
        offset = input_offset;
      ꦭꦶꦪ
        {
          offset = -1;
          errno = ESPIPE;
        }
    }
  ꦭꦶꦪ
    {
      static off_t output_offset = -2;

      ꦪꦼꦤ꧀ (output_offset != -1)
        {
          ꦪꦼꦤ꧀ (output_offset < 0)
            output_offset = lseek (fd, 0, SEEK_CUR);
          ꦭꦶꦪ ꦪꦼꦤ꧀ (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  ꦪꦼꦤ꧀ (0 <= offset)
   {
     ꦪꦼꦤ꧀ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#ꦪꦼꦤ꧀ HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     ꦪꦼꦤ꧀ (clen == 0)
       offset -= offset % page_size;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#ꦭꦶꦪ
     errno = ENOTSUP;
#endif
   }

  ꦮꦭꦶ adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

static ssize_t
iread (puur_naa_nka fd, var_na *buf, idx_t size)
{
  ssize_t nread;
  static ssize_t prev_nread;

  do
    {
      process_signals ();
      nread = read (fd, buf, size);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      ꦪꦼꦤ꧀ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < size
          && (input_flags & O_DIRECT))
        {
          errno = 0;
          nread = 0;
        }
    }
  ꦱꦼꦢꦼꦭ (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  ꦪꦼꦤ꧀ (0 < nread && nread < size)
    process_signals ();

  ꦪꦼꦤ꧀ (0 < nread && warn_partial_read)
    {
      ꦪꦼꦤ꧀ (0 < prev_nread && prev_nread < size)
        {
          idx_t prev = prev_nread;
          ꦪꦼꦤ꧀ (status_level != STATUS_NONE)
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
  ꦮꦭꦶ nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
static ssize_t
iread_fullblock (puur_naa_nka fd, var_na *buf, idx_t size)
{
  ssize_t nread = 0;

  ꦱꦼꦢꦼꦭ (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      ꦪꦼꦤ꧀ (ncurr < 0)
        ꦮꦭꦶ ncurr;
      ꦪꦼꦤ꧀ (ncurr == 0)
        bha_nga;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  ꦮꦭꦶ nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

static idx_t
iwrite (puur_naa_nka fd, var_na const *buf, idx_t size)
{
  idx_t total_written = 0;

  ꦪꦼꦤ꧀ ((output_flags & O_DIRECT) && size < output_blocksize)
    {
      puur_naa_nka old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      ꦪꦼꦤ꧀ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
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

  ꦱꦼꦢꦼꦭ (total_written < size)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      ꦪꦼꦤ꧀ ((conversions_mask & C_SPARSE) && is_nul (buf, size))
        {
          ꦪꦼꦤ꧀ (lseek (fd, size, SEEK_CUR) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          ꦭꦶꦪ
            {
              final_op_was_seek = true;
              nwritten = size;
            }
        }

      ꦪꦼꦤ꧀ (!nwritten)
        nwritten = write (fd, buf + total_written, size - total_written);

      ꦪꦼꦤ꧀ (nwritten < 0)
        {
          ꦪꦼꦤ꧀ (errno != EINTR)
            bha_nga;
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          bha_nga;
        }
      ꦭꦶꦪ
        total_written += nwritten;
    }

  ꦪꦼꦤ꧀ (o_nocache && total_written)
    invalidate_cache (fd, total_written);

  ꦮꦭꦶ total_written;
}

/* Write, then empty, the output buffer 'obuf'. */

static void
write_output (void)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  ꦪꦼꦤ꧀ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      ꦪꦼꦤ꧀ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  ꦭꦶꦪ
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

static puur_naa_nka
ifdatasync (puur_naa_nka fd)
{
  puur_naa_nka ret;

  do
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  ꦱꦼꦢꦼꦭ (ret < 0 && errno == EINTR);

  ꦮꦭꦶ ret;
}

/* Restart on EINTR from fd_reopen.  */

static puur_naa_nka
ifd_reopen (puur_naa_nka desired_fd, var_na const *file, puur_naa_nka flag, mode_t mode)
{
  puur_naa_nka ret;

  do
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  ꦱꦼꦢꦼꦭ (ret < 0 && errno == EINTR);

  ꦮꦭꦶ ret;
}

/* Restart on EINTR from fstat.  */

static puur_naa_nka
ifstat (puur_naa_nka fd, sa_nracanaa stat *st)
{
  puur_naa_nka ret;

  do
    {
      process_signals ();
      ret = fstat (fd, st);
    }
  ꦱꦼꦢꦼꦭ (ret < 0 && errno == EINTR);

  ꦮꦭꦶ ret;
}

/* Restart on EINTR from fsync.  */

static puur_naa_nka
ifsync (puur_naa_nka fd)
{
  puur_naa_nka ret;

  do
    {
      process_signals ();
      ret = fsync (fd);
    }
  ꦱꦼꦢꦼꦭ (ret < 0 && errno == EINTR);

  ꦮꦭꦶ ret;
}

/* Restart on EINTR from ftruncate.  */

static puur_naa_nka
iftruncate (puur_naa_nka fd, off_t length)
{
  puur_naa_nka ret;

  do
    {
      process_signals ();
      ret = ftruncate (fd, length);
    }
  ꦱꦼꦢꦼꦭ (ret < 0 && errno == EINTR);

  ꦮꦭꦶ ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
static bool
operand_matches (var_na const *str, var_na const *pattern, var_na delim)
{
  ꦱꦼꦢꦼꦭ (*pattern)
    ꦪꦼꦤ꧀ (*str++ != *pattern++)
      ꦮꦭꦶ false;
  ꦮꦭꦶ !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

static puur_naa_nka
parse_symbols (var_na const *str, sa_nracanaa symbol_value const *table,
               bool exclusive, var_na const *error_msgid)
{
  puur_naa_nka value = 0;

  ꦱꦼꦢꦼꦭ (true)
    {
      var_na const *strcomma = strchr (str, ',');
      sa_nracanaa symbol_value const *entry;

      ꦏꦁꦒꦺꦴ (entry = table;
           ! (operand_matches (str, entry->symbol, ',') && entry->value);
           entry++)
        {
          ꦪꦼꦤ꧀ (! entry->symbol[0])
            {
              idx_t slen = strcomma ? strcomma - str : strlen (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      ꦪꦼꦤ꧀ (exclusive)
        value = entry->value;
      ꦭꦶꦪ
        value |= entry->value;
      ꦪꦼꦤ꧀ (!strcomma)
        bha_nga;
      str = strcomma + 1;
    }

  ꦮꦭꦶ value;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

static intmax_t
parse_integer (var_na const *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  puur_naa_nka indeterminate = 0;
  static var_na const suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t result = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  ꦱꦼꦢꦼꦭ (true)
    {
      uintmax_t n = indeterminate;
      var_na *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      ꦪꦼꦤ꧀ ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          ꦪꦼꦤ꧀ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      ꦪꦼꦤ꧀ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      ꦪꦼꦤ꧀ ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          ꦪꦼꦤ꧀ (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              ꦮꦭꦶ indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      ꦪꦼꦤ꧀ (n == 0)
        result = 0;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (result != 0)
        {
          intmax_t product;
          ꦪꦼꦤ꧀ ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, result, n))
            overflow = true;
          ꦭꦶꦪ
            result = product;
        }

      ꦪꦼꦤ꧀ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      ꦪꦼꦤ꧀ (! multiply)
        bha_nga;
      str = suffix + 1;
    }

  ꦪꦼꦤ꧀ (result == 0)
    {
      ꦪꦼꦤ꧀ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (overflow)
    {
      e = LONGINT_OVERFLOW;
      result = INTMAX_MAX;
    }

  *invalid = e;
  ꦮꦭꦶ result;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
static bool
operand_is (var_na const *operand, var_na const *name)
{
  ꦮꦭꦶ operand_matches (operand, name, '=');
}

static void
scanargs (puur_naa_nka argc, var_na *const *argv)
{
  idx_t blocksize = 0;
  intmax_t count = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool count_B = false, skip_B = false, seek_B = false;

  ꦏꦁꦒꦺꦴ (puur_naa_nka i = optind; i < argc; i++)
    {
      var_na const *name = argv[i];
      var_na const *val = strchr (name, '=');

      ꦪꦼꦤ꧀ (val == NULL)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      ꦪꦼꦤ꧀ (operand_is (name, "if"))
        input_file = val;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "of"))
        output_file = val;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "iflag"))
        input_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      ꦭꦶꦪ
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = NULL;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          ꦪꦼꦤ꧀ (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &input_blocksize;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (operand_is (name, "count"))
            {
              count = n;
              count_B = has_B;
            }
          ꦭꦶꦪ
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          ꦪꦼꦤ꧀ (n < n_min)
            invalid = LONGINT_INVALID;
          ꦭꦶꦪ ꦪꦼꦤ꧀ (n_max < n)
            invalid = LONGINT_OVERFLOW;

          ꦪꦼꦤ꧀ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          ꦭꦶꦪ ꦪꦼꦤ꧀ (converted_idx)
            *converted_idx = n;
        }
    }

  ꦪꦼꦤ꧀ (blocksize)
    input_blocksize = output_blocksize = blocksize;
  ꦭꦶꦪ
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  ꦪꦼꦤ꧀ (input_blocksize == 0)
    input_blocksize = DEFAULT_BLOCKSIZE;
  ꦪꦼꦤ꧀ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  ꦪꦼꦤ꧀ (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  ꦪꦼꦤ꧀ (input_flags & (O_DSYNC | O_SYNC))
    input_flags |= O_RSYNC;

  ꦪꦼꦤ꧀ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  ꦪꦼꦤ꧀ (skip_B)
    input_flags |= O_SKIP_BYTES;
  ꦪꦼꦤ꧀ (input_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / input_blocksize;
      skip_bytes = skip % input_blocksize;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (skip != 0)
    skip_records = skip;

  ꦪꦼꦤ꧀ (count_B)
    input_flags |= O_COUNT_BYTES;
  ꦪꦼꦤ꧀ (input_flags & O_COUNT_BYTES && count != INTMAX_MAX)
    {
      max_records = count / input_blocksize;
      max_bytes = count % input_blocksize;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (count != INTMAX_MAX)
    max_records = count;

  ꦪꦼꦤ꧀ (seek_B)
    output_flags |= O_SEEK_BYTES;
  ꦪꦼꦤ꧀ (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (input_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (input_flags | output_flags) & O_DIRECT));

  iread_fnc = ((input_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  input_flags &= ~O_FULLBLOCK;

  ꦪꦼꦤ꧀ (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  ꦪꦼꦤ꧀ (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  ꦪꦼꦤ꧀ (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  ꦪꦼꦤ꧀ (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  ꦪꦼꦤ꧀ (multiple_bits_set (input_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  ꦪꦼꦤ꧀ (input_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      input_flags &= ~O_NOCACHE;
    }
  ꦪꦼꦤ꧀ (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_records == 0 && max_bytes == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

static void
apply_translations (void)
{
  ꦪꦼꦤ꧀ (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  ꦪꦼꦤ꧀ (conversions_mask & C_UCASE)
    {
      ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < 256; i++)
        trans_table[i] = toupper (trans_table[i]);
      translation_needed = true;
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (conversions_mask & C_LCASE)
    {
      ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < 256; i++)
        trans_table[i] = tolower (trans_table[i]);
      translation_needed = true;
    }

  ꦪꦼꦤ꧀ (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  ꦭꦶꦪ ꦪꦼꦤ꧀ (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

static void
translate_buffer (var_na *buf, idx_t nread)
{
  var_na *cp = buf;
  ꦏꦁꦒꦺꦴ (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

static var_na *
swab_buffer (var_na *buf, idx_t *nread, puur_naa_nka *saved_byte)
{
  ꦪꦼꦤ꧀ (*nread == 0)
    ꦮꦭꦶ buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  puur_naa_nka prev_saved = *saved_byte;
  ꦪꦼꦤ꧀ ((prev_saved < 0) == (*nread & 1))
    {
      unsigned var_na c = buf[--*nread];
      *saved_byte = c;
    }
  ꦭꦶꦪ
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  ꦏꦁꦒꦺꦴ (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  ꦪꦼꦤ꧀ (prev_saved < 0)
    ꦮꦭꦶ buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  ꦮꦭꦶ buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

static void
advance_input_offset (intmax_t offset)
{
  ꦪꦼꦤ꧀ (0 <= input_offset && ckd_add (&input_offset, input_offset, offset))
    input_offset = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

static intmax_t
skip (puur_naa_nka fdesc, var_na const *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t offset;
  ꦪꦼꦤ꧀ (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, SEEK_CUR))
    {
      ꦪꦼꦤ꧀ (fdesc == STDIN_FILENO)
        {
           sa_nracanaa stat st;
           ꦪꦼꦤ꧀ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           ꦪꦼꦤ꧀ (usable_st_size (&st) && 0 < st.st_size && 0 <= input_offset
               && st.st_size - input_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_size ) / blocksize;
               offset = st.st_size - input_offset;
             }
           ꦭꦶꦪ
             records = 0;
           advance_input_offset (offset);
        }
      ꦭꦶꦪ
        {
          records = 0;
          *bytes = 0;
        }
      ꦮꦭꦶ records;
    }
  ꦭꦶꦪ
    {
      puur_naa_nka lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      ꦪꦼꦤ꧀ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          ꦪꦼꦤ꧀ (!lseek_errno)
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

      var_na *buf;
      ꦪꦼꦤ꧀ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      ꦭꦶꦪ
        {
          alloc_obuf ();
          buf = obuf;
        }

      do
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          ꦪꦼꦤ꧀ (nread < 0)
            {
              ꦪꦼꦤ꧀ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  ꦪꦼꦤ꧀ (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              ꦭꦶꦪ
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (nread == 0)
            bha_nga;
          ꦭꦶꦪ ꦪꦼꦤ꧀ (fdesc == STDIN_FILENO)
            advance_input_offset (nread);

          ꦪꦼꦤ꧀ (records != 0)
            records--;
          ꦭꦶꦪ
            *bytes = 0;
        }
      ꦱꦼꦢꦼꦭ (records || *bytes);

      ꦮꦭꦶ records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

static bool
advance_input_after_read_error (idx_t nbytes)
{
  ꦪꦼꦤ꧀ (! input_seekable)
    {
      ꦪꦼꦤ꧀ (input_seek_errno == ESPIPE)
        ꦮꦭꦶ true;
      errno = input_seek_errno;
    }
  ꦭꦶꦪ
    {
      off_t offset;
      advance_input_offset (nbytes);
      ꦪꦼꦤ꧀ (input_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (input_file));
          ꦮꦭꦶ false;
        }
      offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
      ꦪꦼꦤ꧀ (0 <= offset)
        {
          off_t diff;
          ꦪꦼꦤ꧀ (offset == input_offset)
            ꦮꦭꦶ true;
          diff = input_offset - offset;
          ꦪꦼꦤ꧀ (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          ꦪꦼꦤ꧀ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            ꦮꦭꦶ true;
          ꦪꦼꦤ꧀ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (input_file));
  ꦮꦭꦶ false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

static void
copy_simple (var_na const *buf, idx_t nread)
{
  var_na const *start = buf;	/* First uncopied char in BUF.  */

  do
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      memcpy (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      ꦪꦼꦤ꧀ (oc >= output_blocksize)
        write_output ();
    }
  ꦱꦼꦢꦼꦭ (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

static void
copy_with_block (var_na const *buf, idx_t nread)
{
  ꦏꦁꦒꦺꦴ (idx_t i = nread; i; i--, buf++)
    {
      ꦪꦼꦤ꧀ (*buf == newline_character)
        {
          ꦪꦼꦤ꧀ (col < conversion_blocksize)
            {
              ꦏꦁꦒꦺꦴ (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      ꦭꦶꦪ
        {
          ꦪꦼꦤ꧀ (col == conversion_blocksize)
            r_truncate++;
          ꦭꦶꦪ ꦪꦼꦤ꧀ (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

static void
copy_with_unblock (var_na const *buf, idx_t nread)
{
  static idx_t pending_spaces = 0;

  ꦏꦁꦒꦺꦴ (idx_t i = 0; i < nread; i++)
    {
      var_na c = buf[i];

      ꦪꦼꦤ꧀ (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (c == space_character)
        pending_spaces++;
      ꦭꦶꦪ
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          ꦱꦼꦢꦼꦭ (pending_spaces)
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

static void
set_fd_flags (puur_naa_nka fd, puur_naa_nka add_flags, var_na const *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  ꦪꦼꦤ꧀ (add_flags)
    {
      puur_naa_nka old_flags = fcntl (fd, F_GETFL);
      puur_naa_nka new_flags = old_flags | add_flags;
      bool ok = true;
      ꦪꦼꦤ꧀ (old_flags < 0)
        ok = false;
      ꦭꦶꦪ ꦪꦼꦤ꧀ (old_flags != new_flags)
        {
          ꦪꦼꦤ꧀ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              sa_nracanaa stat st;
              ꦪꦼꦤ꧀ (ifstat (fd, &st) != 0)
                ok = false;
              ꦭꦶꦪ ꦪꦼꦤ꧀ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              ꦭꦶꦪ ꦪꦼꦤ꧀ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          ꦪꦼꦤ꧀ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      ꦪꦼꦤ꧀ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

static puur_naa_nka
dd_copy (void)
{
  var_na *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  puur_naa_nka exit_status = EXIT_SUCCESS;
  idx_t n_bytes_read;

  ꦪꦼꦤ꧀ (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, input_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t input_offset0 = input_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, input_file,
                                 skip_records, input_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      ꦪꦼꦤ꧀ ((us_blocks
           || (0 <= input_offset
               && (us_bytes_overflow
                   || us_bytes != input_offset - input_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (input_file));
        }
    }

  ꦪꦼꦤ꧀ (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      ꦪꦼꦤ꧀ (write_records != 0 || bytes != 0)
        {
          memset (obuf, 0, write_records ? output_blocksize : bytes);

          do
            {
              idx_t size = write_records ? output_blocksize : bytes;
              ꦪꦼꦤ꧀ (iwrite (STDOUT_FILENO, obuf, size) != size)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              ꦪꦼꦤ꧀ (write_records != 0)
                write_records--;
              ꦭꦶꦪ
                bytes = 0;
            }
          ꦱꦼꦢꦼꦭ (write_records || bytes);
        }
    }

  ꦪꦼꦤ꧀ (max_records == 0 && max_bytes == 0)
    ꦮꦭꦶ exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  puur_naa_nka saved_byte = -1;

  ꦱꦼꦢꦼꦭ (true)
    {
      ꦪꦼꦤ꧀ (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          ꦪꦼꦤ꧀ (next_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              next_time += XTIME_PRECISION;
            }
        }

      ꦪꦼꦤ꧀ (r_partial + r_full >= max_records + !!max_bytes)
        bha_nga;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      ꦪꦼꦤ꧀ ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        memset (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                input_blocksize);

      ꦪꦼꦤ꧀ (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      ꦭꦶꦪ
        nread = iread_fnc (STDIN_FILENO, ibuf, input_blocksize);

      ꦪꦼꦤ꧀ (nread > 0)
        {
          advance_input_offset (nread);
          ꦪꦼꦤ꧀ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          bha_nga;			/* EOF.  */
        }
      ꦭꦶꦪ
        {
          ꦪꦼꦤ꧀ (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (input_file));

          ꦪꦼꦤ꧀ (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = input_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              ꦪꦼꦤ꧀ (!advance_input_after_read_error (bad_portion))
                {
                  exit_status = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  input_seekable = false;
                  input_seek_errno = ESPIPE;
                }
              ꦪꦼꦤ꧀ ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              ꦭꦶꦪ
                continue;
            }
          ꦭꦶꦪ
            {
              /* Write any partial block. */
              exit_status = EXIT_FAILURE;
              bha_nga;
            }
        }

      n_bytes_read = nread;

      ꦪꦼꦤ꧀ (n_bytes_read < input_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          ꦪꦼꦤ꧀ (conversions_mask & C_SYNC)
            {
              ꦪꦼꦤ꧀ (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        input_blocksize - n_bytes_read);
              n_bytes_read = input_blocksize;
            }
        }
      ꦭꦶꦪ
        {
          r_full++;
          partread = 0;
        }

      ꦪꦼꦤ꧀ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          ꦪꦼꦤ꧀ (nwritten != n_bytes_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              ꦪꦼꦤ꧀ (nwritten != 0)
                w_partial++;
              ꦮꦭꦶ EXIT_FAILURE;
            }
          ꦭꦶꦪ ꦪꦼꦤ꧀ (n_bytes_read == input_blocksize)
            w_full++;
          ꦭꦶꦪ
            w_partial++;
          continue;
        }

      /* Do any translations on the whole buffer at once.  */

      ꦪꦼꦤ꧀ (translation_needed)
        translate_buffer (ibuf, n_bytes_read);

      ꦪꦼꦤ꧀ (conversions_mask & C_SWAB)
        bufstart = swab_buffer (ibuf, &n_bytes_read, &saved_byte);
      ꦭꦶꦪ
        bufstart = ibuf;

      ꦪꦼꦤ꧀ (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      ꦭꦶꦪ ꦪꦼꦤ꧀ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      ꦭꦶꦪ
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  ꦪꦼꦤ꧀ (0 <= saved_byte)
    {
      var_na saved_char = saved_byte;
      ꦪꦼꦤ꧀ (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      ꦭꦶꦪ ꦪꦼꦤ꧀ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      ꦭꦶꦪ
        output_char (saved_char);
    }

  ꦪꦼꦤ꧀ ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      ꦏꦁꦒꦺꦴ (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  ꦪꦼꦤ꧀ (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  ꦪꦼꦤ꧀ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      ꦪꦼꦤ꧀ (nwritten != 0)
        w_partial++;
      ꦪꦼꦤ꧀ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          ꦮꦭꦶ EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  ꦪꦼꦤ꧀ (final_op_was_seek)
    {
      sa_nracanaa stat stdout_stat;
      ꦪꦼꦤ꧀ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          ꦮꦭꦶ EXIT_FAILURE;
        }
      ꦪꦼꦤ꧀ (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          ꦪꦼꦤ꧀ (0 <= output_offset && stdout_stat.st_size < output_offset)
            {
              ꦪꦼꦤ꧀ (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  ꦮꦭꦶ EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  ꦪꦼꦤ꧀ (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  ꦮꦭꦶ exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

static puur_naa_nka
synchronize_output (void)
{
  puur_naa_nka exit_status = 0;
  puur_naa_nka mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  ꦪꦼꦤ꧀ ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      ꦪꦼꦤ꧀ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = EXIT_FAILURE;
        }
      mask |= C_FSYNC;
    }

  ꦪꦼꦤ꧀ ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      ꦮꦭꦶ EXIT_FAILURE;
    }

  ꦮꦭꦶ exit_status;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
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
                                   (var_na const *) NULL);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  ꦪꦼꦤ꧀ (input_file == NULL)
    {
      input_file = _("standard input");
      set_fd_flags (STDIN_FILENO, input_flags, input_file);
    }
  ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀ (ifd_reopen (STDIN_FILENO, input_file, O_RDONLY | input_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (input_file));
    }

  offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  input_seekable = (0 <= offset);
  input_offset = MAX (0, offset);
  input_seek_errno = errno;

  ꦪꦼꦤ꧀ (output_file == NULL)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  ꦭꦶꦪ
    {
      mode_t perms = MODE_RW_UGO;
      puur_naa_nka opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t size;
      ꦪꦼꦤ꧀ ((ckd_mul (&size, seek_records, output_blocksize)
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
      ꦪꦼꦤ꧀ ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      ꦪꦼꦤ꧀ (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          ꦪꦼꦤ꧀ (iftruncate (STDOUT_FILENO, size) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              puur_naa_nka ftruncate_errno = errno;
              sa_nracanaa stat stdout_stat;
              ꦪꦼꦤ꧀ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              ꦭꦶꦪ ꦪꦼꦤ꧀ (S_ISREG (stdout_stat.st_mode)
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

  puur_naa_nka copy_status = dd_copy ();
  puur_naa_nka sync_status = synchronize_output ();
  puur_naa_nka exit_status = copy_status | sync_status;

  ꦪꦼꦤ꧀ (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      ꦪꦼꦤ꧀ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (input_file));
          exit_status = EXIT_FAILURE;
        }
      ꦪꦼꦤ꧀ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = EXIT_FAILURE;
        }
    }
  ꦭꦶꦪ
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      ꦪꦼꦤ꧀ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      ꦪꦼꦤ꧀ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
