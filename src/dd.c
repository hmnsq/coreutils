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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "alignalloc.h"
#അന്തര്ഭാവ "close-stream.h"
#അന്തര്ഭാവ "fd-reopen.h"
#അന്തര്ഭാവ "gethrxtime.h"
#അന്തര്ഭാവ "human.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "long-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xstrtol.h"
#അന്തര്ഭാവ "xtime.h"

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
  ചെയ്യുക						\
    {						\
      obuf[oc++] = (c);				\
      എങ്കിൽ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  ആകുമ്പോൾ (0)

/* Default input and output blocksize. */
#define DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
എണ്ണൽ
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
എണ്ണൽ
  {
    STATUS_NONE = 1,
    STATUS_NOXFER = 2,
    STATUS_DEFAULT = 3,
    STATUS_PROGRESS = 4
  };

/* The name of the input file, or NULL for the standard input. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *നിവേശം_file = ശൂന്യ;

/* The name of the output file, or NULL for the standard output. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *output_file = ശൂന്യ;

/* The page size on this host.  */
സ്ഥിതം idx_t page_size;

/* The number of bytes in which atomic reads are done. */
സ്ഥിതം idx_t നിവേശം_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
സ്ഥിതം idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
സ്ഥിതം idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
സ്ഥിതം intmax_t skip_records = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
സ്ഥിതം idx_t skip_bytes = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
സ്ഥിതം intmax_t seek_records = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
സ്ഥിതം intmax_t seek_bytes = 0;

/* Whether the final output was done with a seek (rather than a write).  */
സ്ഥിതം bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
സ്ഥിതം intmax_t max_records = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
സ്ഥിതം idx_t max_bytes = 0;

/* Bit vector of conversions to apply. */
സ്ഥിതം പൂർണ്ണസംഖ്യ conversions_mask = 0;

/* Open flags for the input and output files.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ നിവേശം_flags = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ output_flags = 0;

/* Status flags for what is printed to stderr.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ status_level = STATUS_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
സ്ഥിതം bool translation_needed = false;

/* Number of partial blocks written. */
സ്ഥിതം intmax_t w_partial = 0;

/* Number of full blocks written. */
സ്ഥിതം intmax_t w_full = 0;

/* Number of partial blocks read. */
സ്ഥിതം intmax_t r_partial = 0;

/* Number of full blocks read. */
സ്ഥിതം intmax_t r_full = 0;

/* Number of bytes written.  */
സ്ഥിതം intmax_t w_bytes = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
സ്ഥിതം intmax_t reported_w_bytes = -1;

/* Time that dd started.  */
സ്ഥിതം xtime_t start_time;

/* Next time to report periodic progress.  */
സ്ഥിതം xtime_t next_time;

/* If positive, the number of bytes output in the current progress line.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ progress_len;

/* True if input is seekable.  */
സ്ഥിതം bool നിവേശം_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ നിവേശം_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
സ്ഥിതം off_t നിവേശം_offset;

/* True if a partial read should be diagnosed.  */
സ്ഥിതം bool warn_partial_read;

/* Records truncated by conv=block. */
സ്ഥിതം intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
സ്ഥിതം അക്ഷരം newline_character = '\n';
സ്ഥിതം അക്ഷരം space_character = ' ';

/* I/O buffers.  */
സ്ഥിതം അക്ഷരം *ibuf;
സ്ഥിതം അക്ഷരം *obuf;

/* Current index into 'obuf'. */
സ്ഥിതം idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
സ്ഥിതം idx_t col = 0;

/* The set of signals that are caught.  */
സ്ഥിതം sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
സ്ഥിതം sig_atomic_t അസ്ഥിര interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
സ്ഥിതം sig_atomic_t അസ്ഥിര info_signal_എണ്ണം;

/* Whether to discard cache for input or output.  */
സ്ഥിതം bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
സ്ഥിതം bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
സ്ഥിതം ssize_t (*iread_fnc) (പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buf, idx_t size);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_ചിഹ്നം "count_bytes"

/* A symbol and the corresponding integer value.  */
ഘടന ചിഹ്നം_മൂല്യം
{
  അക്ഷരം ചിഹ്നം[വലുപ്പം LONGEST_ചിഹ്നം];
  പൂർണ്ണസംഖ്യ മൂല്യം;
};

/* Conversion symbols, for conv="...".  */
സ്ഥിതം ഘടന ചിഹ്നം_മൂല്യം സ്ഥിരം conversions[] =
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
എണ്ണൽ
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ദ്വിമാന
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

    O_എണ്ണം_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_എണ്ണം_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_എണ്ണം_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_എണ്ണം_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
സ്ഥിതം ഘടന ചിഹ്നം_മൂല്യം സ്ഥിരം flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ദ്വിമാന},
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
  {"count_bytes", O_എണ്ണം_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
സ്ഥിതം ഘടന ചിഹ്നം_മൂല്യം സ്ഥിരം statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
സ്ഥിതം unsigned അക്ഷരം trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം ascii_to_ebcdic[] =
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

സ്ഥിതം അക്ഷരം സ്ഥിരം ascii_to_ibm[] =
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

സ്ഥിതം അക്ഷരം സ്ഥിരം ebcdic_to_ascii[] =
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
സ്ഥിതം bool close_stdout_required = true;

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
സ്ഥിതം ശൂന്യം
maybe_close_stdout (ശൂന്യം)
{
  എങ്കിൽ (close_stdout_required)
    close_stdout ();
  അല്ലെങ്കിൽ എങ്കിൽ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
സ്ഥിതം ശൂന്യം
diagnose (പൂർണ്ണസംഖ്യ errnum, അക്ഷരം സ്ഥിരം *fmt, ...)
{
  എങ്കിൽ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  va_list ap;
  va_start (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_end (ap);
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  bs=BYTES        പഠ and ലിഖ up to BYTES bytes at a time (സ്ഥിരസ്ഥിതി: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a time\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated ചിഹ്നം list\n\
"));
      oputs (_("\
  എണ്ണം=N         copy only N നിവേശം blocks\n\
"));
      oputs (_("\
  ibs=BYTES       പഠ up to BYTES bytes at a time (സ്ഥിരസ്ഥിതി: 512)\n\
"));
      oputs (_("\
  എങ്കിൽ=FILE         പഠ from FILE instead of standard നിവേശം\n\
"));
      oputs (_("\
  iflag=FLAGS     പഠ as per the comma separated ചിഹ്നം list\n\
"));
      oputs (_("\
  obs=BYTES       ലിഖ BYTES bytes at a time (സ്ഥിരസ്ഥിതി: 512)\n\
"));
      oputs (_("\
  of=FILE         ലിഖ to FILE instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     ലിഖ as per the comma separated ചിഹ്നം list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized നിവേശം blocks\n\
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
GB=1000*1000*1000, G=1024*1024*1024, and so on വേണ്ടി T, P, E, Z, Y, R, Q.\n\
ദ്വിമാന_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV ചിഹ്നം may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-size\n\
  unblock   replace trailing spaces in cbs-size records with newline\n\
  lcase     change upper അവസ്ഥ to lower അവസ്ഥ\n\
  ucase     change lower അവസ്ഥ to upper അവസ്ഥ\n\
  sparse    try to seek rather than ലിഖ all-NUL output blocks\n\
  swab      swap every pair of നിവേശം bytes\n\
  sync      pad every നിവേശം block with NULs to ibs-size; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail എങ്കിൽ the output file already exists\n\
  nocreat   ചെയ്യുക not create the output file\n\
  notrunc   ചെയ്യുക not truncate the output file\n\
  noerror   തുടരുക after പഠ errors\n\
  fdatasync  physically ലിഖ output file data before finishing\n\
  fsync     likewise, but also ലിഖ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG ചിഹ്നം may be:\n\
\n\
  append    append mode (makes sense only വേണ്ടി output; conv=notrunc suggested)\n\
"), stdout);
      എങ്കിൽ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      എങ്കിൽ (O_DIRECT)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      എങ്കിൽ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      എങ്കിൽ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      എങ്കിൽ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      എങ്കിൽ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#എങ്കിൽ HAVE_POSIX_FADVISE
      എങ്കിൽ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#endif
      എങ്കിൽ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      എങ്കിൽ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      എങ്കിൽ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      എങ്കിൽ (O_ദ്വിമാന)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      എങ്കിൽ (O_TEXT)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        printf (_("\
\n\
Sending a %s സൂചന to a running 'dd' process makes it\n\
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

എണ്ണൽ { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

സ്ഥിതം ശൂന്യം
alloc_ibuf (ശൂന്യം)
{
  എങ്കിൽ (ibuf)
    തിരികെ;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_size, നിവേശം_blocksize + extra_byte_for_swab);
  എങ്കിൽ (!ibuf)
    {
      അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             നിവേശം_blocksize,
             human_readable (നിവേശം_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

സ്ഥിതം ശൂന്യം
alloc_obuf (ശൂന്യം)
{
  എങ്കിൽ (obuf)
    തിരികെ;

  എങ്കിൽ (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_size, output_blocksize);
      എങ്കിൽ (!obuf)
        {
          അക്ഷരം hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  അല്ലെങ്കിൽ
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

സ്ഥിതം ശൂന്യം
translate_charset (അക്ഷരം സ്ഥിരം *new_trans)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

സ്ഥിതം വരിയിൽ bool
multiple_bits_set (പൂർണ്ണസംഖ്യ i)
{
  തിരികെ MULTIPLE_BITS_SET (i);
}

സ്ഥിതം bool
abbreviation_lacks_prefix (അക്ഷരം സ്ഥിരം *message)
{
  തിരികെ message[strlen (message) - 2] == ' ';
}

/* Print transfer statistics.  */

സ്ഥിതം ശൂന്യം
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  സ്ഥിതം അക്ഷരം സ്ഥിരം slash_s[] = "/s";
  അക്ഷരം hbuf[3][LONGEST_HUMAN_READABLE + വലുപ്പം slash_s];
  ഇരട്ട delta_s;
  അക്ഷരം സ്ഥിരം *bytes_per_second;
  അക്ഷരം സ്ഥിരം *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  അക്ഷരം സ്ഥിരം *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  അക്ഷരം *bpsbuf = hbuf[2];
  പൂർണ്ണസംഖ്യ bpsbufsize = വലുപ്പം hbuf[2];
  എങ്കിൽ (start_time < now)
    {
      ഇരട്ട XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  അല്ലെങ്കിൽ
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  എങ്കിൽ (progress_time)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  അക്ഷരം delta_s_buf[24];
  snprintf (delta_s_buf, വലുപ്പം delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  പൂർണ്ണസംഖ്യ stats_len
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

  എങ്കിൽ (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      എങ്കിൽ (0 <= stats_len && stats_len < progress_len)
        fprintf (stderr, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  അല്ലെങ്കിൽ
    fputc ('\n', stderr);

  reported_w_bytes = w_bytes;
}

സ്ഥിതം ശൂന്യം
print_stats (ശൂന്യം)
{
  എങ്കിൽ (status_level == STATUS_NONE)
    തിരികെ;

  എങ്കിൽ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  എങ്കിൽ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  എങ്കിൽ (status_level == STATUS_NOXFER)
    തിരികെ;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

സ്ഥിതം ശൂന്യം
interrupt_handler (പൂർണ്ണസംഖ്യ sig)
{
  എങ്കിൽ (! SA_RESETHAND)
    സൂചന (sig, SIG_DFL);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

സ്ഥിതം ശൂന്യം
siginfo_handler (MAYBE_UNUSED പൂർണ്ണസംഖ്യ sig)
{
  info_signal_എണ്ണം++;
}

/* Install the signal handlers.  */

സ്ഥിതം ശൂന്യം
install_signal_handlers (ശൂന്യം)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  ഘടന sigaction act;
  sigemptyset (&caught_signals);
  എങ്കിൽ (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (SIGINT, ശൂന്യ, &act);
  എങ്കിൽ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_signals, SIGINT);
  act.sa_mask = caught_signals;

  എങ്കിൽ (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ശൂന്യ);
    }

  എങ്കിൽ (sigismember (&caught_signals, SIGINT))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ശൂന്യ);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
iclose (പൂർണ്ണസംഖ്യ fd)
{
  എങ്കിൽ (close (fd) != 0)
    ചെയ്യുക
      എങ്കിൽ (errno != EINTR)
        തിരികെ -1;
    ആകുമ്പോൾ (close (fd) != 0 && errno != EBADF);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ synchronize_output (ശൂന്യം);

സ്ഥിതം ശൂന്യം
cleanup (ശൂന്യം)
{
  എങ്കിൽ (!interrupt_signal)
    {
      പൂർണ്ണസംഖ്യ sync_status = synchronize_output ();
      എങ്കിൽ (sync_status)
        exit (sync_status);
    }

  എങ്കിൽ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (നിവേശം_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  എങ്കിൽ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

സ്ഥിതം ശൂന്യം
process_signals (ശൂന്യം)
{
  ആകുമ്പോൾ (interrupt_signal || info_signal_എണ്ണം)
    {
      പൂർണ്ണസംഖ്യ interrupt;
      പൂർണ്ണസംഖ്യ infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_എണ്ണം;

      എങ്കിൽ (infos)
        info_signal_എണ്ണം = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ശൂന്യ);

      എങ്കിൽ (interrupt)
        cleanup ();
      print_stats ();
      എങ്കിൽ (interrupt)
        ഉയർത്തുക (interrupt);
    }
}

സ്ഥിതം ശൂന്യം
finish_up (ശൂന്യം)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_signals ();
  cleanup ();
  print_stats ();
}

സ്ഥിതം ശൂന്യം
quit (പൂർണ്ണസംഖ്യ code)
{
  finish_up ();
  exit (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

സ്ഥിതം off_t
cache_round (പൂർണ്ണസംഖ്യ fd, off_t len)
{
  സ്ഥിതം off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  എങ്കിൽ (len)
    {
      intmax_t c_pending;
      എങ്കിൽ (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      എങ്കിൽ (c_pending > *pending)
        len = c_pending - *pending;
      അല്ലെങ്കിൽ
        len = 0;
    }
  അല്ലെങ്കിൽ
    len = *pending;

  തിരികെ len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

സ്ഥിതം bool
invalidate_cache (പൂർണ്ണസംഖ്യ fd, off_t len)
{
  പൂർണ്ണസംഖ്യ adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  എങ്കിൽ (len && !clen)
    തിരികെ true; /* Don't advise this time.  */
  അല്ലെങ്കിൽ എങ്കിൽ (! len && ! clen && ! nocache_eof)
    തിരികെ true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  എങ്കിൽ (fd == STDIN_FILENO)
    {
      എങ്കിൽ (നിവേശം_seekable)
        offset = നിവേശം_offset;
      അല്ലെങ്കിൽ
        {
          offset = -1;
          errno = ESPIPE;
        }
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം off_t output_offset = -2;

      എങ്കിൽ (output_offset != -1)
        {
          എങ്കിൽ (output_offset < 0)
            output_offset = lseek (fd, 0, SEEK_CUR);
          അല്ലെങ്കിൽ എങ്കിൽ (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  എങ്കിൽ (0 <= offset)
   {
     എങ്കിൽ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#എങ്കിൽ HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     എങ്കിൽ (clen == 0)
       offset -= offset % page_size;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#അല്ലെങ്കിൽ
     errno = ENOTSUP;
#endif
   }

  തിരികെ adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

സ്ഥിതം ssize_t
iread (പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buf, idx_t size)
{
  ssize_t nread;
  സ്ഥിതം ssize_t prev_nread;

  ചെയ്യുക
    {
      process_signals ();
      nread = പഠ (fd, buf, size);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      എങ്കിൽ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < size
          && (നിവേശം_flags & O_DIRECT))
        {
          errno = 0;
          nread = 0;
        }
    }
  ആകുമ്പോൾ (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  എങ്കിൽ (0 < nread && nread < size)
    process_signals ();

  എങ്കിൽ (0 < nread && warn_partial_read)
    {
      എങ്കിൽ (0 < prev_nread && prev_nread < size)
        {
          idx_t prev = prev_nread;
          എങ്കിൽ (status_level != STATUS_NONE)
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
  തിരികെ nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
സ്ഥിതം ssize_t
iread_fullblock (പൂർണ്ണസംഖ്യ fd, അക്ഷരം *buf, idx_t size)
{
  ssize_t nread = 0;

  ആകുമ്പോൾ (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      എങ്കിൽ (ncurr < 0)
        തിരികെ ncurr;
      എങ്കിൽ (ncurr == 0)
        നിർത്തുക;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  തിരികെ nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

സ്ഥിതം idx_t
iwrite (പൂർണ്ണസംഖ്യ fd, അക്ഷരം സ്ഥിരം *buf, idx_t size)
{
  idx_t total_written = 0;

  എങ്കിൽ ((output_flags & O_DIRECT) && size < output_blocksize)
    {
      പൂർണ്ണസംഖ്യ old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      എങ്കിൽ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
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

  ആകുമ്പോൾ (total_written < size)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      എങ്കിൽ ((conversions_mask & C_SPARSE) && is_nul (buf, size))
        {
          എങ്കിൽ (lseek (fd, size, SEEK_CUR) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          അല്ലെങ്കിൽ
            {
              final_op_was_seek = true;
              nwritten = size;
            }
        }

      എങ്കിൽ (!nwritten)
        nwritten = ലിഖ (fd, buf + total_written, size - total_written);

      എങ്കിൽ (nwritten < 0)
        {
          എങ്കിൽ (errno != EINTR)
            നിർത്തുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          നിർത്തുക;
        }
      അല്ലെങ്കിൽ
        total_written += nwritten;
    }

  എങ്കിൽ (o_nocache && total_written)
    invalidate_cache (fd, total_written);

  തിരികെ total_written;
}

/* Write, then empty, the output buffer 'obuf'. */

സ്ഥിതം ശൂന്യം
write_output (ശൂന്യം)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  എങ്കിൽ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      എങ്കിൽ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  അല്ലെങ്കിൽ
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
ifdatasync (പൂർണ്ണസംഖ്യ fd)
{
  പൂർണ്ണസംഖ്യ ret;

  ചെയ്യുക
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  ആകുമ്പോൾ (ret < 0 && errno == EINTR);

  തിരികെ ret;
}

/* Restart on EINTR from fd_reopen.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
ifd_reopen (പൂർണ്ണസംഖ്യ desired_fd, അക്ഷരം സ്ഥിരം *file, പൂർണ്ണസംഖ്യ flag, mode_t mode)
{
  പൂർണ്ണസംഖ്യ ret;

  ചെയ്യുക
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  ആകുമ്പോൾ (ret < 0 && errno == EINTR);

  തിരികെ ret;
}

/* Restart on EINTR from fstat.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
ifstat (പൂർണ്ണസംഖ്യ fd, ഘടന stat *st)
{
  പൂർണ്ണസംഖ്യ ret;

  ചെയ്യുക
    {
      process_signals ();
      ret = fstat (fd, st);
    }
  ആകുമ്പോൾ (ret < 0 && errno == EINTR);

  തിരികെ ret;
}

/* Restart on EINTR from fsync.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
ifsync (പൂർണ്ണസംഖ്യ fd)
{
  പൂർണ്ണസംഖ്യ ret;

  ചെയ്യുക
    {
      process_signals ();
      ret = fsync (fd);
    }
  ആകുമ്പോൾ (ret < 0 && errno == EINTR);

  തിരികെ ret;
}

/* Restart on EINTR from ftruncate.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
iftruncate (പൂർണ്ണസംഖ്യ fd, off_t length)
{
  പൂർണ്ണസംഖ്യ ret;

  ചെയ്യുക
    {
      process_signals ();
      ret = ftruncate (fd, length);
    }
  ആകുമ്പോൾ (ret < 0 && errno == EINTR);

  തിരികെ ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
സ്ഥിതം bool
operand_matches (അക്ഷരം സ്ഥിരം *str, അക്ഷരം സ്ഥിരം *pattern, അക്ഷരം delim)
{
  ആകുമ്പോൾ (*pattern)
    എങ്കിൽ (*str++ != *pattern++)
      തിരികെ false;
  തിരികെ !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
parse_symbols (അക്ഷരം സ്ഥിരം *str, ഘടന ചിഹ്നം_മൂല്യം സ്ഥിരം *table,
               bool exclusive, അക്ഷരം സ്ഥിരം *error_msgid)
{
  പൂർണ്ണസംഖ്യ മൂല്യം = 0;

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം സ്ഥിരം *strcomma = strchr (str, ',');
      ഘടന ചിഹ്നം_മൂല്യം സ്ഥിരം *entry;

      വേണ്ടി (entry = table;
           ! (operand_matches (str, entry->ചിഹ്നം, ',') && entry->മൂല്യം);
           entry++)
        {
          എങ്കിൽ (! entry->ചിഹ്നം[0])
            {
              idx_t slen = strcomma ? strcomma - str : strlen (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      എങ്കിൽ (exclusive)
        മൂല്യം = entry->മൂല്യം;
      അല്ലെങ്കിൽ
        മൂല്യം |= entry->മൂല്യം;
      എങ്കിൽ (!strcomma)
        നിർത്തുക;
      str = strcomma + 1;
    }

  തിരികെ മൂല്യം;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

സ്ഥിതം intmax_t
parse_integer (അക്ഷരം സ്ഥിരം *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  പൂർണ്ണസംഖ്യ indeterminate = 0;
  സ്ഥിതം അക്ഷരം സ്ഥിരം suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t result = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  ആകുമ്പോൾ (true)
    {
      uintmax_t n = indeterminate;
      അക്ഷരം *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      എങ്കിൽ ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          എങ്കിൽ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      എങ്കിൽ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      എങ്കിൽ ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          എങ്കിൽ (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              തിരികെ indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      എങ്കിൽ (n == 0)
        result = 0;
      അല്ലെങ്കിൽ എങ്കിൽ (result != 0)
        {
          intmax_t product;
          എങ്കിൽ ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, result, n))
            overflow = true;
          അല്ലെങ്കിൽ
            result = product;
        }

      എങ്കിൽ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      എങ്കിൽ (! multiply)
        നിർത്തുക;
      str = suffix + 1;
    }

  എങ്കിൽ (result == 0)
    {
      എങ്കിൽ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (overflow)
    {
      e = LONGINT_OVERFLOW;
      result = INTMAX_MAX;
    }

  *invalid = e;
  തിരികെ result;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
സ്ഥിതം bool
operand_is (അക്ഷരം സ്ഥിരം *operand, അക്ഷരം സ്ഥിരം *name)
{
  തിരികെ operand_matches (operand, name, '=');
}

സ്ഥിതം ശൂന്യം
scanargs (പൂർണ്ണസംഖ്യ argc, അക്ഷരം *സ്ഥിരം *argv)
{
  idx_t blocksize = 0;
  intmax_t എണ്ണം = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool എണ്ണം_B = false, skip_B = false, seek_B = false;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; i++)
    {
      അക്ഷരം സ്ഥിരം *name = argv[i];
      അക്ഷരം സ്ഥിരം *val = strchr (name, '=');

      എങ്കിൽ (val == ശൂന്യ)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      എങ്കിൽ (operand_is (name, "if"))
        നിവേശം_file = val;
      അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "of"))
        output_file = val;
      അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "iflag"))
        നിവേശം_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      അല്ലെങ്കിൽ
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ശൂന്യ;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          എങ്കിൽ (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &നിവേശം_blocksize;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (operand_is (name, "count"))
            {
              എണ്ണം = n;
              എണ്ണം_B = has_B;
            }
          അല്ലെങ്കിൽ
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          എങ്കിൽ (n < n_min)
            invalid = LONGINT_INVALID;
          അല്ലെങ്കിൽ എങ്കിൽ (n_max < n)
            invalid = LONGINT_OVERFLOW;

          എങ്കിൽ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          അല്ലെങ്കിൽ എങ്കിൽ (converted_idx)
            *converted_idx = n;
        }
    }

  എങ്കിൽ (blocksize)
    നിവേശം_blocksize = output_blocksize = blocksize;
  അല്ലെങ്കിൽ
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  എങ്കിൽ (നിവേശം_blocksize == 0)
    നിവേശം_blocksize = DEFAULT_BLOCKSIZE;
  എങ്കിൽ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  എങ്കിൽ (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  എങ്കിൽ (നിവേശം_flags & (O_DSYNC | O_SYNC))
    നിവേശം_flags |= O_RSYNC;

  എങ്കിൽ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (skip_B)
    നിവേശം_flags |= O_SKIP_BYTES;
  എങ്കിൽ (നിവേശം_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / നിവേശം_blocksize;
      skip_bytes = skip % നിവേശം_blocksize;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (skip != 0)
    skip_records = skip;

  എങ്കിൽ (എണ്ണം_B)
    നിവേശം_flags |= O_എണ്ണം_BYTES;
  എങ്കിൽ (നിവേശം_flags & O_എണ്ണം_BYTES && എണ്ണം != INTMAX_MAX)
    {
      max_records = എണ്ണം / നിവേശം_blocksize;
      max_bytes = എണ്ണം % നിവേശം_blocksize;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (എണ്ണം != INTMAX_MAX)
    max_records = എണ്ണം;

  എങ്കിൽ (seek_B)
    output_flags |= O_SEEK_BYTES;
  എങ്കിൽ (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (നിവേശം_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (നിവേശം_flags | output_flags) & O_DIRECT));

  iread_fnc = ((നിവേശം_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  നിവേശം_flags &= ~O_FULLBLOCK;

  എങ്കിൽ (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  എങ്കിൽ (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  എങ്കിൽ (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  എങ്കിൽ (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  എങ്കിൽ (multiple_bits_set (നിവേശം_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  എങ്കിൽ (നിവേശം_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      നിവേശം_flags &= ~O_NOCACHE;
    }
  എങ്കിൽ (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_records == 0 && max_bytes == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

സ്ഥിതം ശൂന്യം
apply_translations (ശൂന്യം)
{
  എങ്കിൽ (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  എങ്കിൽ (conversions_mask & C_UCASE)
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
        trans_table[i] = toupper (trans_table[i]);
      translation_needed = true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (conversions_mask & C_LCASE)
    {
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
        trans_table[i] = tolower (trans_table[i]);
      translation_needed = true;
    }

  എങ്കിൽ (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  അല്ലെങ്കിൽ എങ്കിൽ (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

സ്ഥിതം ശൂന്യം
translate_buffer (അക്ഷരം *buf, idx_t nread)
{
  അക്ഷരം *cp = buf;
  വേണ്ടി (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

സ്ഥിതം അക്ഷരം *
swab_buffer (അക്ഷരം *buf, idx_t *nread, പൂർണ്ണസംഖ്യ *saved_byte)
{
  എങ്കിൽ (*nread == 0)
    തിരികെ buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  പൂർണ്ണസംഖ്യ prev_saved = *saved_byte;
  എങ്കിൽ ((prev_saved < 0) == (*nread & 1))
    {
      unsigned അക്ഷരം c = buf[--*nread];
      *saved_byte = c;
    }
  അല്ലെങ്കിൽ
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  വേണ്ടി (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  എങ്കിൽ (prev_saved < 0)
    തിരികെ buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  തിരികെ buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

സ്ഥിതം ശൂന്യം
advance_നിവേശം_offset (intmax_t offset)
{
  എങ്കിൽ (0 <= നിവേശം_offset && ckd_add (&നിവേശം_offset, നിവേശം_offset, offset))
    നിവേശം_offset = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

സ്ഥിതം intmax_t
skip (പൂർണ്ണസംഖ്യ fdesc, അക്ഷരം സ്ഥിരം *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t offset;
  എങ്കിൽ (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, SEEK_CUR))
    {
      എങ്കിൽ (fdesc == STDIN_FILENO)
        {
           ഘടന stat st;
           എങ്കിൽ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           എങ്കിൽ (usable_st_size (&st) && 0 < st.st_size && 0 <= നിവേശം_offset
               && st.st_size - നിവേശം_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_size ) / blocksize;
               offset = st.st_size - നിവേശം_offset;
             }
           അല്ലെങ്കിൽ
             records = 0;
           advance_നിവേശം_offset (offset);
        }
      അല്ലെങ്കിൽ
        {
          records = 0;
          *bytes = 0;
        }
      തിരികെ records;
    }
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      എങ്കിൽ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          എങ്കിൽ (!lseek_errno)
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

      അക്ഷരം *buf;
      എങ്കിൽ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      അല്ലെങ്കിൽ
        {
          alloc_obuf ();
          buf = obuf;
        }

      ചെയ്യുക
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          എങ്കിൽ (nread < 0)
            {
              എങ്കിൽ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  എങ്കിൽ (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              അല്ലെങ്കിൽ
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          അല്ലെങ്കിൽ എങ്കിൽ (nread == 0)
            നിർത്തുക;
          അല്ലെങ്കിൽ എങ്കിൽ (fdesc == STDIN_FILENO)
            advance_നിവേശം_offset (nread);

          എങ്കിൽ (records != 0)
            records--;
          അല്ലെങ്കിൽ
            *bytes = 0;
        }
      ആകുമ്പോൾ (records || *bytes);

      തിരികെ records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

സ്ഥിതം bool
advance_നിവേശം_after_read_error (idx_t nbytes)
{
  എങ്കിൽ (! നിവേശം_seekable)
    {
      എങ്കിൽ (നിവേശം_seek_errno == ESPIPE)
        തിരികെ true;
      errno = നിവേശം_seek_errno;
    }
  അല്ലെങ്കിൽ
    {
      off_t offset;
      advance_നിവേശം_offset (nbytes);
      എങ്കിൽ (നിവേശം_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (നിവേശം_file));
          തിരികെ false;
        }
      offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
      എങ്കിൽ (0 <= offset)
        {
          off_t diff;
          എങ്കിൽ (offset == നിവേശം_offset)
            തിരികെ true;
          diff = നിവേശം_offset - offset;
          എങ്കിൽ (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          എങ്കിൽ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            തിരികെ true;
          എങ്കിൽ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (നിവേശം_file));
  തിരികെ false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

സ്ഥിതം ശൂന്യം
copy_simple (അക്ഷരം സ്ഥിരം *buf, idx_t nread)
{
  അക്ഷരം സ്ഥിരം *start = buf;	/* First uncopied char in BUF.  */

  ചെയ്യുക
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      memcpy (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      എങ്കിൽ (oc >= output_blocksize)
        write_output ();
    }
  ആകുമ്പോൾ (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

സ്ഥിതം ശൂന്യം
copy_with_block (അക്ഷരം സ്ഥിരം *buf, idx_t nread)
{
  വേണ്ടി (idx_t i = nread; i; i--, buf++)
    {
      എങ്കിൽ (*buf == newline_character)
        {
          എങ്കിൽ (col < conversion_blocksize)
            {
              വേണ്ടി (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (col == conversion_blocksize)
            r_truncate++;
          അല്ലെങ്കിൽ എങ്കിൽ (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

സ്ഥിതം ശൂന്യം
copy_with_unblock (അക്ഷരം സ്ഥിരം *buf, idx_t nread)
{
  സ്ഥിതം idx_t pending_spaces = 0;

  വേണ്ടി (idx_t i = 0; i < nread; i++)
    {
      അക്ഷരം c = buf[i];

      എങ്കിൽ (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c == space_character)
        pending_spaces++;
      അല്ലെങ്കിൽ
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          ആകുമ്പോൾ (pending_spaces)
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

സ്ഥിതം ശൂന്യം
set_fd_flags (പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ add_flags, അക്ഷരം സ്ഥിരം *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  എങ്കിൽ (add_flags)
    {
      പൂർണ്ണസംഖ്യ old_flags = fcntl (fd, F_GETFL);
      പൂർണ്ണസംഖ്യ new_flags = old_flags | add_flags;
      bool ok = true;
      എങ്കിൽ (old_flags < 0)
        ok = false;
      അല്ലെങ്കിൽ എങ്കിൽ (old_flags != new_flags)
        {
          എങ്കിൽ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              ഘടന stat st;
              എങ്കിൽ (ifstat (fd, &st) != 0)
                ok = false;
              അല്ലെങ്കിൽ എങ്കിൽ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              അല്ലെങ്കിൽ എങ്കിൽ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          എങ്കിൽ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      എങ്കിൽ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
dd_copy (ശൂന്യം)
{
  അക്ഷരം *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  പൂർണ്ണസംഖ്യ exit_status = EXIT_SUCCESS;
  idx_t n_bytes_read;

  എങ്കിൽ (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, നിവേശം_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t നിവേശം_offset0 = നിവേശം_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, നിവേശം_file,
                                 skip_records, നിവേശം_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      എങ്കിൽ ((us_blocks
           || (0 <= നിവേശം_offset
               && (us_bytes_overflow
                   || us_bytes != നിവേശം_offset - നിവേശം_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (നിവേശം_file));
        }
    }

  എങ്കിൽ (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      എങ്കിൽ (write_records != 0 || bytes != 0)
        {
          memset (obuf, 0, write_records ? output_blocksize : bytes);

          ചെയ്യുക
            {
              idx_t size = write_records ? output_blocksize : bytes;
              എങ്കിൽ (iwrite (STDOUT_FILENO, obuf, size) != size)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              എങ്കിൽ (write_records != 0)
                write_records--;
              അല്ലെങ്കിൽ
                bytes = 0;
            }
          ആകുമ്പോൾ (write_records || bytes);
        }
    }

  എങ്കിൽ (max_records == 0 && max_bytes == 0)
    തിരികെ exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  പൂർണ്ണസംഖ്യ saved_byte = -1;

  ആകുമ്പോൾ (true)
    {
      എങ്കിൽ (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          എങ്കിൽ (next_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              next_time += XTIME_PRECISION;
            }
        }

      എങ്കിൽ (r_partial + r_full >= max_records + !!max_bytes)
        നിർത്തുക;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      എങ്കിൽ ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        memset (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                നിവേശം_blocksize);

      എങ്കിൽ (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      അല്ലെങ്കിൽ
        nread = iread_fnc (STDIN_FILENO, ibuf, നിവേശം_blocksize);

      എങ്കിൽ (nread > 0)
        {
          advance_നിവേശം_offset (nread);
          എങ്കിൽ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          നിർത്തുക;			/* EOF.  */
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (നിവേശം_file));

          എങ്കിൽ (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = നിവേശം_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              എങ്കിൽ (!advance_നിവേശം_after_read_error (bad_portion))
                {
                  exit_status = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  നിവേശം_seekable = false;
                  നിവേശം_seek_errno = ESPIPE;
                }
              എങ്കിൽ ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              അല്ലെങ്കിൽ
                തുടരുക;
            }
          അല്ലെങ്കിൽ
            {
              /* Write any partial block. */
              exit_status = EXIT_FAILURE;
              നിർത്തുക;
            }
        }

      n_bytes_read = nread;

      എങ്കിൽ (n_bytes_read < നിവേശം_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          എങ്കിൽ (conversions_mask & C_SYNC)
            {
              എങ്കിൽ (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        നിവേശം_blocksize - n_bytes_read);
              n_bytes_read = നിവേശം_blocksize;
            }
        }
      അല്ലെങ്കിൽ
        {
          r_full++;
          partread = 0;
        }

      എങ്കിൽ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          എങ്കിൽ (nwritten != n_bytes_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              എങ്കിൽ (nwritten != 0)
                w_partial++;
              തിരികെ EXIT_FAILURE;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (n_bytes_read == നിവേശം_blocksize)
            w_full++;
          അല്ലെങ്കിൽ
            w_partial++;
          തുടരുക;
        }

      /* Do any translations on the whole buffer at once.  */

      എങ്കിൽ (translation_needed)
        translate_buffer (ibuf, n_bytes_read);

      എങ്കിൽ (conversions_mask & C_SWAB)
        bufstart = swab_buffer (ibuf, &n_bytes_read, &saved_byte);
      അല്ലെങ്കിൽ
        bufstart = ibuf;

      എങ്കിൽ (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      അല്ലെങ്കിൽ എങ്കിൽ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      അല്ലെങ്കിൽ
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  എങ്കിൽ (0 <= saved_byte)
    {
      അക്ഷരം saved_char = saved_byte;
      എങ്കിൽ (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      അല്ലെങ്കിൽ എങ്കിൽ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      അല്ലെങ്കിൽ
        output_char (saved_char);
    }

  എങ്കിൽ ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      വേണ്ടി (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  എങ്കിൽ (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  എങ്കിൽ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      എങ്കിൽ (nwritten != 0)
        w_partial++;
      എങ്കിൽ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          തിരികെ EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  എങ്കിൽ (final_op_was_seek)
    {
      ഘടന stat stdout_stat;
      എങ്കിൽ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          തിരികെ EXIT_FAILURE;
        }
      എങ്കിൽ (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          എങ്കിൽ (0 <= output_offset && stdout_stat.st_size < output_offset)
            {
              എങ്കിൽ (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  തിരികെ EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  എങ്കിൽ (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  തിരികെ exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
synchronize_output (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ exit_status = 0;
  പൂർണ്ണസംഖ്യ mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  എങ്കിൽ ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      എങ്കിൽ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = EXIT_FAILURE;
        }
      mask |= C_FSYNC;
    }

  എങ്കിൽ ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      തിരികെ EXIT_FAILURE;
    }

  തിരികെ exit_status;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
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
                                   (അക്ഷരം സ്ഥിരം *) ശൂന്യ);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  എങ്കിൽ (നിവേശം_file == ശൂന്യ)
    {
      നിവേശം_file = _("standard input");
      set_fd_flags (STDIN_FILENO, നിവേശം_flags, നിവേശം_file);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (ifd_reopen (STDIN_FILENO, നിവേശം_file, O_RDONLY | നിവേശം_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (നിവേശം_file));
    }

  offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  നിവേശം_seekable = (0 <= offset);
  നിവേശം_offset = MAX (0, offset);
  നിവേശം_seek_errno = errno;

  എങ്കിൽ (output_file == ശൂന്യ)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  അല്ലെങ്കിൽ
    {
      mode_t perms = MODE_RW_UGO;
      പൂർണ്ണസംഖ്യ opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t size;
      എങ്കിൽ ((ckd_mul (&size, seek_records, output_blocksize)
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
      എങ്കിൽ ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      എങ്കിൽ (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          എങ്കിൽ (iftruncate (STDOUT_FILENO, size) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              പൂർണ്ണസംഖ്യ ftruncate_errno = errno;
              ഘടന stat stdout_stat;
              എങ്കിൽ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              അല്ലെങ്കിൽ എങ്കിൽ (S_ISREG (stdout_stat.st_mode)
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

  പൂർണ്ണസംഖ്യ copy_status = dd_copy ();
  പൂർണ്ണസംഖ്യ sync_status = synchronize_output ();
  പൂർണ്ണസംഖ്യ exit_status = copy_status | sync_status;

  എങ്കിൽ (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      എങ്കിൽ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (നിവേശം_file));
          exit_status = EXIT_FAILURE;
        }
      എങ്കിൽ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = EXIT_FAILURE;
        }
    }
  അല്ലെങ്കിൽ
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      എങ്കിൽ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      എങ്കിൽ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
