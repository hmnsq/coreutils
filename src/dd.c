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

#अन्तर्भाव <config.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "alignalloc.h"
#अन्तर्भाव "close-stream.h"
#अन्तर्भाव "fd-reopen.h"
#अन्तर्भाव "gethrxtime.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xtime.h"

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
  करा						\
    {						\
      obuf[oc++] = (c);				\
      जर (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  पर्यंत (0)

/* Default input and output blocksize. */
#define DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
गणना
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
गणना
  {
    STATUS_NONE = 1,
    STATUS_NOXFER = 2,
    STATUS_DEFAULT = 3,
    STATUS_PROGRESS = 4
  };

/* The name of the input file, or NULL for the standard input. */
स्थिर अक्षर स्थिरांक *आदान_file = रिक्त;

/* The name of the output file, or NULL for the standard output. */
स्थिर अक्षर स्थिरांक *output_file = रिक्त;

/* The page size on this host.  */
स्थिर idx_t page_size;

/* The number of bytes in which atomic reads are done. */
स्थिर idx_t आदान_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
स्थिर idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
स्थिर idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
स्थिर intmax_t skip_records = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
स्थिर idx_t skip_bytes = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
स्थिर intmax_t seek_records = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
स्थिर intmax_t seek_bytes = 0;

/* Whether the final output was done with a seek (rather than a write).  */
स्थिर bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
स्थिर intmax_t max_records = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
स्थिर idx_t max_bytes = 0;

/* Bit vector of conversions to apply. */
स्थिर पूर्णांक conversions_mask = 0;

/* Open flags for the input and output files.  */
स्थिर पूर्णांक आदान_flags = 0;
स्थिर पूर्णांक output_flags = 0;

/* Status flags for what is printed to stderr.  */
स्थिर पूर्णांक status_level = STATUS_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
स्थिर bool translation_needed = false;

/* Number of partial blocks written. */
स्थिर intmax_t w_partial = 0;

/* Number of full blocks written. */
स्थिर intmax_t w_full = 0;

/* Number of partial blocks read. */
स्थिर intmax_t r_partial = 0;

/* Number of full blocks read. */
स्थिर intmax_t r_full = 0;

/* Number of bytes written.  */
स्थिर intmax_t w_bytes = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
स्थिर intmax_t reported_w_bytes = -1;

/* Time that dd started.  */
स्थिर xtime_t start_time;

/* Next time to report periodic progress.  */
स्थिर xtime_t next_time;

/* If positive, the number of bytes output in the current progress line.  */
स्थिर पूर्णांक progress_len;

/* True if input is seekable.  */
स्थिर bool आदान_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
स्थिर पूर्णांक आदान_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
स्थिर off_t आदान_offset;

/* True if a partial read should be diagnosed.  */
स्थिर bool warn_partial_read;

/* Records truncated by conv=block. */
स्थिर intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
स्थिर अक्षर newline_character = '\n';
स्थिर अक्षर space_character = ' ';

/* I/O buffers.  */
स्थिर अक्षर *ibuf;
स्थिर अक्षर *obuf;

/* Current index into 'obuf'. */
स्थिर idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
स्थिर idx_t col = 0;

/* The set of signals that are caught.  */
स्थिर sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
स्थिर संक_पूर्ण_प्रकार अस्थिर interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
स्थिर संक_पूर्ण_प्रकार अस्थिर info_signal_मोजणी;

/* Whether to discard cache for input or output.  */
स्थिर bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
स्थिर bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
स्थिर ssize_t (*iread_fnc) (पूर्णांक fd, अक्षर *buf, idx_t size);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_चिन्ह "count_bytes"

/* A symbol and the corresponding integer value.  */
संरचना चिन्ह_मूल्य
{
  अक्षर चिन्ह[आकार LONGEST_चिन्ह];
  पूर्णांक मूल्य;
};

/* Conversion symbols, for conv="...".  */
स्थिर संरचना चिन्ह_मूल्य स्थिरांक conversions[] =
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
गणना
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_द्विमान
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

    O_मोजणी_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_मोजणी_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_मोजणी_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_मोजणी_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
स्थिर संरचना चिन्ह_मूल्य स्थिरांक flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_द्विमान},
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
  {"count_bytes", O_मोजणी_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
स्थिर संरचना चिन्ह_मूल्य स्थिरांक statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
स्थिर अचिन्हित अक्षर trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

स्थिर अक्षर स्थिरांक ascii_to_ebcdic[] =
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

स्थिर अक्षर स्थिरांक ascii_to_ibm[] =
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

स्थिर अक्षर स्थिरांक ebcdic_to_ascii[] =
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
स्थिर bool close_stdout_required = true;

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
स्थिर शून्य
maybe_close_stdout (शून्य)
{
  जर (close_stdout_required)
    close_stdout ();
  अन्यथा जर (close_stream (मानक_त्रुटि) != 0)
    _exit (निकास_त्रुटि);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
स्थिर शून्य
diagnose (पूर्णांक errnum, अक्षर स्थिरांक *fmt, ...)
{
  जर (0 < progress_len)
    {
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
      progress_len = 0;
    }

  बहु_सूची ap;
  बहु_शुरू (ap, fmt);
  verror (0, errnum, fmt, ap);
  बहु_पूर्ण (ap);
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPERAND]...\n\
  or:  %s OPTION\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Copy a file, converting and formatting according to the operands.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  bs=BYTES        पठ and लिख up to BYTES bytes at a समय (पूर्वनिर्धारित: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a समय\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated चिन्ह list\n\
"));
      oputs (_("\
  मोजणी=N         copy only N आदान blocks\n\
"));
      oputs (_("\
  ibs=BYTES       पठ up to BYTES bytes at a समय (पूर्वनिर्धारित: 512)\n\
"));
      oputs (_("\
  जर=खाता         पठ from खाता instead of standard आदान\n\
"));
      oputs (_("\
  iflag=FLAGS     पठ as per the comma separated चिन्ह list\n\
"));
      oputs (_("\
  obs=BYTES       लिख BYTES bytes at a समय (पूर्वनिर्धारित: 512)\n\
"));
      oputs (_("\
  of=खाता         लिख to खाता instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     लिख as per the comma separated चिन्ह list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized आदान blocks\n\
"));
      oputs (_("\
  status=LEVEL    The LEVEL of information to print to standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      ख_माला_दो (_("\
\n\
N and BYTES may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on साठी T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV चिन्ह may be:\n\
\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-size\n\
  unblock   replace trailing spaces in cbs-size records with newline\n\
  lcase     change upper स्थिती to lower स्थिती\n\
  ucase     change lower स्थिती to upper स्थिती\n\
  sparse    try to seek rather than लिख all-NUL output blocks\n\
  swab      swap every pair of आदान bytes\n\
  sync      pad every आदान block with NULs to ibs-size; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), मानक_निकास);
      ख_माला_दो (_("\
  excl      fail जर the output file already exists\n\
  nocreat   करा not create the output file\n\
  notrunc   करा not truncate the output file\n\
  noerror   सुरूठेवा after पठ errors\n\
  fdatasync  physically लिख output file data before finishing\n\
  fsync     likewise, but also लिख metadata\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
Each FLAG चिन्ह may be:\n\
\n\
  append    append mode (makes sense only साठी output; conv=notrunc suggested)\n\
"), मानक_निकास);
      जर (O_CIO)
        ख_माला_दो (_("  cio       use concurrent I/O for data\n"), मानक_निकास);
      जर (O_DIRECT)
        ख_माला_दो (_("  direct    use direct I/O for data\n"), मानक_निकास);
      ख_माला_दो (_("  directory  fail unless a directory\n"), मानक_निकास);
      जर (O_DSYNC)
        ख_माला_दो (_("  dsync     use synchronized I/O for data\n"), मानक_निकास);
      जर (O_SYNC)
        ख_माला_दो (_("  sync      likewise, but also for metadata\n"), मानक_निकास);
      ख_माला_दो (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             मानक_निकास);
      जर (O_NONBLOCK)
        ख_माला_दो (_("  nonblock  use non-blocking I/O\n"), मानक_निकास);
      जर (O_NOATIME)
        ख_माला_दो (_("  noatime   do not update access time\n"), मानक_निकास);
#जर HAVE_POSIX_FADVISE
      जर (O_NOCACHE)
        ख_माला_दो (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               मानक_निकास);
#endif
      जर (O_NOCTTY)
        ख_माला_दो (_("  noctty    do not assign controlling terminal from file\n"),
               मानक_निकास);
      जर (HAVE_WORKING_O_NOFOLLOW)
        ख_माला_दो (_("  nofollow  do not follow symlinks\n"), मानक_निकास);
      जर (O_NOLINKS)
        ख_माला_दो (_("  nolinks   fail if multiply-linked\n"), मानक_निकास);
      जर (O_द्विमान)
        ख_माला_दो (_("  binary    use binary I/O for data\n"), मानक_निकास);
      जर (O_TEXT)
        ख_माला_दो (_("  text      use text I/O for data\n"), मानक_निकास);

      {
        म_लिखो (_("\
\n\
Sending a %s संकेत to a running 'dd' process makes it\n\
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
  निकास (status);
}

/* Common options to use when displaying sizes and rates.  */

गणना { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

स्थिर शून्य
alloc_ibuf (शून्य)
{
  जर (ibuf)
    परत;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_size, आदान_blocksize + extra_byte_for_swab);
  जर (!ibuf)
    {
      अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
      error (निकास_त्रुटि, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             आदान_blocksize,
             human_readable (आदान_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

स्थिर शून्य
alloc_obuf (शून्य)
{
  जर (obuf)
    परत;

  जर (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_size, output_blocksize);
      जर (!obuf)
        {
          अक्षर hbuf[LONGEST_HUMAN_READABLE + 1];
          error (निकास_त्रुटि, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  अन्यथा
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

स्थिर शून्य
translate_charset (अक्षर स्थिरांक *new_trans)
{
  साठी (पूर्णांक i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

स्थिर ओळीत bool
multiple_bits_set (पूर्णांक i)
{
  परत MULTIPLE_BITS_SET (i);
}

स्थिर bool
abbreviation_lacks_prefix (अक्षर स्थिरांक *message)
{
  परत message[म_माप (message) - 2] == ' ';
}

/* Print transfer statistics.  */

स्थिर शून्य
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  स्थिर अक्षर स्थिरांक slash_s[] = "/s";
  अक्षर hbuf[3][LONGEST_HUMAN_READABLE + आकार slash_s];
  द्विगुण delta_s;
  अक्षर स्थिरांक *bytes_per_second;
  अक्षर स्थिरांक *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  अक्षर स्थिरांक *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  अक्षर *bpsbuf = hbuf[2];
  पूर्णांक bpsbufsize = आकार hbuf[2];
  जर (start_time < now)
    {
      द्विगुण XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      म_जोड़ो (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  अन्यथा
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  जर (progress_time)
    ख_अक्षर_दो ('\r', मानक_त्रुटि);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  अक्षर delta_s_buf[24];
  snprintf (delta_s_buf, आकार delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  पूर्णांक stats_len
    = (abbreviation_lacks_prefix (si)
       ? ख_लिखो (मानक_त्रुटि,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_bytes)),
                  w_bytes, delta_s_buf, bytes_per_second)
       : abbreviation_lacks_prefix (iec)
       ? ख_लिखो (मानक_त्रुटि,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_bytes, si, delta_s_buf, bytes_per_second)
       : ख_लिखो (मानक_त्रुटि,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_bytes, si, iec, delta_s_buf, bytes_per_second));

  जर (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      जर (0 <= stats_len && stats_len < progress_len)
        ख_लिखो (मानक_त्रुटि, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  अन्यथा
    ख_अक्षर_दो ('\n', मानक_त्रुटि);

  reported_w_bytes = w_bytes;
}

स्थिर शून्य
print_stats (शून्य)
{
  जर (status_level == STATUS_NONE)
    परत;

  जर (0 < progress_len)
    {
      ख_अक्षर_दो ('\n', मानक_त्रुटि);
      progress_len = 0;
    }

  ख_लिखो (मानक_त्रुटि,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  जर (r_truncate != 0)
    ख_लिखो (मानक_त्रुटि,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  जर (status_level == STATUS_NOXFER)
    परत;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

स्थिर शून्य
interrupt_handler (पूर्णांक sig)
{
  जर (! SA_RESETHAND)
    संकेत (sig, संक_पूर्व);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

स्थिर शून्य
siginfo_handler (MAYBE_UNUSED पूर्णांक sig)
{
  info_signal_मोजणी++;
}

/* Install the signal handlers.  */

स्थिर शून्य
install_signal_handlers (शून्य)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && दो_पर्या ("POSIXLY_CORRECT"));

  संरचना sigaction act;
  sigemptyset (&caught_signals);
  जर (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (संक_विघ्न, रिक्त, &act);
  जर (act.sa_handler != संक_छोड़ो)
    sigaddset (&caught_signals, संक_विघ्न);
  act.sa_mask = caught_signals;

  जर (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, रिक्त);
    }

  जर (sigismember (&caught_signals, संक_विघ्न))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (संक_विघ्न, &act, रिक्त);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
स्थिर पूर्णांक
iclose (पूर्णांक fd)
{
  जर (बंद (fd) != 0)
    करा
      जर (त्रुटि_सं != EINTR)
        परत -1;
    पर्यंत (बंद (fd) != 0 && त्रुटि_सं != EBADF);

  परत 0;
}

स्थिर पूर्णांक synchronize_output (शून्य);

स्थिर शून्य
cleanup (शून्य)
{
  जर (!interrupt_signal)
    {
      पूर्णांक sync_status = synchronize_output ();
      जर (sync_status)
        निकास (sync_status);
    }

  जर (iclose (STDIN_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("closing input file %s"),
           quoteaf (आदान_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  जर (iclose (STDOUT_FILENO) != 0)
    error (निकास_त्रुटि, त्रुटि_सं,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

स्थिर शून्य
process_signals (शून्य)
{
  पर्यंत (interrupt_signal || info_signal_मोजणी)
    {
      पूर्णांक interrupt;
      पूर्णांक infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_मोजणी;

      जर (infos)
        info_signal_मोजणी = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, रिक्त);

      जर (interrupt)
        cleanup ();
      print_stats ();
      जर (interrupt)
        उठवा (interrupt);
    }
}

स्थिर शून्य
finish_up (शून्य)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_signals ();
  cleanup ();
  print_stats ();
}

स्थिर शून्य
quit (पूर्णांक code)
{
  finish_up ();
  निकास (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

स्थिर off_t
cache_round (पूर्णांक fd, off_t len)
{
  स्थिर off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  जर (len)
    {
      intmax_t c_pending;
      जर (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      जर (c_pending > *pending)
        len = c_pending - *pending;
      अन्यथा
        len = 0;
    }
  अन्यथा
    len = *pending;

  परत len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

स्थिर bool
invalidate_cache (पूर्णांक fd, off_t len)
{
  पूर्णांक adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  जर (len && !clen)
    परत true; /* Don't advise this time.  */
  अन्यथा जर (! len && ! clen && ! nocache_eof)
    परत true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  जर (fd == STDIN_FILENO)
    {
      जर (आदान_seekable)
        offset = आदान_offset;
      अन्यथा
        {
          offset = -1;
          त्रुटि_सं = ESPIPE;
        }
    }
  अन्यथा
    {
      स्थिर off_t output_offset = -2;

      जर (output_offset != -1)
        {
          जर (output_offset < 0)
            output_offset = lseek (fd, 0, प्रस्तुत_से);
          अन्यथा जर (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  जर (0 <= offset)
   {
     जर (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#जर HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     जर (clen == 0)
       offset -= offset % page_size;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     त्रुटि_सं = adv_ret;
#अन्यथा
     त्रुटि_सं = ENOTSUP;
#endif
   }

  परत adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

स्थिर ssize_t
iread (पूर्णांक fd, अक्षर *buf, idx_t size)
{
  ssize_t nread;
  स्थिर ssize_t prev_nread;

  करा
    {
      process_signals ();
      nread = पठ (fd, buf, size);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      जर (nread == -1 && त्रुटि_सं == EINVAL
          && 0 < prev_nread && prev_nread < size
          && (आदान_flags & O_DIRECT))
        {
          त्रुटि_सं = 0;
          nread = 0;
        }
    }
  पर्यंत (nread < 0 && त्रुटि_सं == EINTR);

  /* Short read may be due to received signal.  */
  जर (0 < nread && nread < size)
    process_signals ();

  जर (0 < nread && warn_partial_read)
    {
      जर (0 < prev_nread && prev_nread < size)
        {
          idx_t prev = prev_nread;
          जर (status_level != STATUS_NONE)
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
  परत nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
स्थिर ssize_t
iread_fullblock (पूर्णांक fd, अक्षर *buf, idx_t size)
{
  ssize_t nread = 0;

  पर्यंत (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      जर (ncurr < 0)
        परत ncurr;
      जर (ncurr == 0)
        थांबा;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  परत nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

स्थिर idx_t
iwrite (पूर्णांक fd, अक्षर स्थिरांक *buf, idx_t size)
{
  idx_t total_written = 0;

  जर ((output_flags & O_DIRECT) && size < output_blocksize)
    {
      पूर्णांक old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      जर (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
          && status_level != STATUS_NONE)
        diagnose (त्रुटि_सं, _("failed to turn off O_DIRECT: %s"),
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

  पर्यंत (total_written < size)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      जर ((conversions_mask & C_SPARSE) && is_nul (buf, size))
        {
          जर (lseek (fd, size, प्रस्तुत_से) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          अन्यथा
            {
              final_op_was_seek = true;
              nwritten = size;
            }
        }

      जर (!nwritten)
        nwritten = लिख (fd, buf + total_written, size - total_written);

      जर (nwritten < 0)
        {
          जर (त्रुटि_सं != EINTR)
            थांबा;
        }
      अन्यथा जर (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          त्रुटि_सं = ENOSPC;
          थांबा;
        }
      अन्यथा
        total_written += nwritten;
    }

  जर (o_nocache && total_written)
    invalidate_cache (fd, total_written);

  परत total_written;
}

/* Write, then empty, the output buffer 'obuf'. */

स्थिर शून्य
write_output (शून्य)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  जर (nwritten != output_blocksize)
    {
      diagnose (त्रुटि_सं, _("writing to %s"), quoteaf (output_file));
      जर (nwritten != 0)
        w_partial++;
      quit (निकास_त्रुटि);
    }
  अन्यथा
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

स्थिर पूर्णांक
ifdatasync (पूर्णांक fd)
{
  पूर्णांक ret;

  करा
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  पर्यंत (ret < 0 && त्रुटि_सं == EINTR);

  परत ret;
}

/* Restart on EINTR from fd_reopen.  */

स्थिर पूर्णांक
ifd_reopen (पूर्णांक desired_fd, अक्षर स्थिरांक *file, पूर्णांक flag, mode_t mode)
{
  पूर्णांक ret;

  करा
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  पर्यंत (ret < 0 && त्रुटि_सं == EINTR);

  परत ret;
}

/* Restart on EINTR from fstat.  */

स्थिर पूर्णांक
ifstat (पूर्णांक fd, संरचना stat *st)
{
  पूर्णांक ret;

  करा
    {
      process_signals ();
      ret = ख_स्थिति (fd, st);
    }
  पर्यंत (ret < 0 && त्रुटि_सं == EINTR);

  परत ret;
}

/* Restart on EINTR from fsync.  */

स्थिर पूर्णांक
ifsync (पूर्णांक fd)
{
  पूर्णांक ret;

  करा
    {
      process_signals ();
      ret = fsync (fd);
    }
  पर्यंत (ret < 0 && त्रुटि_सं == EINTR);

  परत ret;
}

/* Restart on EINTR from ftruncate.  */

स्थिर पूर्णांक
iftruncate (पूर्णांक fd, off_t length)
{
  पूर्णांक ret;

  करा
    {
      process_signals ();
      ret = ftruncate (fd, length);
    }
  पर्यंत (ret < 0 && त्रुटि_सं == EINTR);

  परत ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
स्थिर bool
operand_matches (अक्षर स्थिरांक *str, अक्षर स्थिरांक *pattern, अक्षर delim)
{
  पर्यंत (*pattern)
    जर (*str++ != *pattern++)
      परत false;
  परत !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

स्थिर पूर्णांक
parse_symbols (अक्षर स्थिरांक *str, संरचना चिन्ह_मूल्य स्थिरांक *table,
               bool exclusive, अक्षर स्थिरांक *error_msgid)
{
  पूर्णांक मूल्य = 0;

  पर्यंत (true)
    {
      अक्षर स्थिरांक *strcomma = म_अक्षर (str, ',');
      संरचना चिन्ह_मूल्य स्थिरांक *entry;

      साठी (entry = table;
           ! (operand_matches (str, entry->चिन्ह, ',') && entry->मूल्य);
           entry++)
        {
          जर (! entry->चिन्ह[0])
            {
              idx_t slen = strcomma ? strcomma - str : म_माप (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (निकास_त्रुटि);
            }
        }

      जर (exclusive)
        मूल्य = entry->मूल्य;
      अन्यथा
        मूल्य |= entry->मूल्य;
      जर (!strcomma)
        थांबा;
      str = strcomma + 1;
    }

  परत मूल्य;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

स्थिर intmax_t
parse_integer (अक्षर स्थिरांक *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  पूर्णांक indeterminate = 0;
  स्थिर अक्षर स्थिरांक suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t result = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  पर्यंत (true)
    {
      uintmax_t n = indeterminate;
      अक्षर *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      जर ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          जर (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      जर (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      जर ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          जर (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              परत indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      जर (n == 0)
        result = 0;
      अन्यथा जर (result != 0)
        {
          intmax_t product;
          जर ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, result, n))
            overflow = true;
          अन्यथा
            result = product;
        }

      जर (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      जर (! multiply)
        थांबा;
      str = suffix + 1;
    }

  जर (result == 0)
    {
      जर (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  अन्यथा जर (overflow)
    {
      e = LONGINT_OVERFLOW;
      result = INTMAX_MAX;
    }

  *invalid = e;
  परत result;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
स्थिर bool
operand_is (अक्षर स्थिरांक *operand, अक्षर स्थिरांक *name)
{
  परत operand_matches (operand, name, '=');
}

स्थिर शून्य
scanargs (पूर्णांक argc, अक्षर *स्थिरांक *argv)
{
  idx_t blocksize = 0;
  intmax_t मोजणी = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool मोजणी_B = false, skip_B = false, seek_B = false;

  साठी (पूर्णांक i = optind; i < argc; i++)
    {
      अक्षर स्थिरांक *name = argv[i];
      अक्षर स्थिरांक *val = म_अक्षर (name, '=');

      जर (val == रिक्त)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (निकास_त्रुटि);
        }
      val++;

      जर (operand_is (name, "if"))
        आदान_file = val;
      अन्यथा जर (operand_is (name, "of"))
        output_file = val;
      अन्यथा जर (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      अन्यथा जर (operand_is (name, "iflag"))
        आदान_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      अन्यथा जर (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      अन्यथा जर (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      अन्यथा
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!म_अक्षर (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = रिक्त;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          जर (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &आदान_blocksize;
            }
          अन्यथा जर (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          अन्यथा जर (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          अन्यथा जर (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          अन्यथा जर (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          अन्यथा जर (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          अन्यथा जर (operand_is (name, "count"))
            {
              मोजणी = n;
              मोजणी_B = has_B;
            }
          अन्यथा
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (निकास_त्रुटि);
            }

          जर (n < n_min)
            invalid = LONGINT_INVALID;
          अन्यथा जर (n_max < n)
            invalid = LONGINT_OVERFLOW;

          जर (invalid != LONGINT_OK)
            error (निकास_त्रुटि, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          अन्यथा जर (converted_idx)
            *converted_idx = n;
        }
    }

  जर (blocksize)
    आदान_blocksize = output_blocksize = blocksize;
  अन्यथा
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  जर (आदान_blocksize == 0)
    आदान_blocksize = DEFAULT_BLOCKSIZE;
  जर (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  जर (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  जर (आदान_flags & (O_DSYNC | O_SYNC))
    आदान_flags |= O_RSYNC;

  जर (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (निकास_त्रुटि);
    }

  जर (skip_B)
    आदान_flags |= O_SKIP_BYTES;
  जर (आदान_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / आदान_blocksize;
      skip_bytes = skip % आदान_blocksize;
    }
  अन्यथा जर (skip != 0)
    skip_records = skip;

  जर (मोजणी_B)
    आदान_flags |= O_मोजणी_BYTES;
  जर (आदान_flags & O_मोजणी_BYTES && मोजणी != INTMAX_MAX)
    {
      max_records = मोजणी / आदान_blocksize;
      max_bytes = मोजणी % आदान_blocksize;
    }
  अन्यथा जर (मोजणी != INTMAX_MAX)
    max_records = मोजणी;

  जर (seek_B)
    output_flags |= O_SEEK_BYTES;
  जर (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  अन्यथा जर (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (आदान_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (आदान_flags | output_flags) & O_DIRECT));

  iread_fnc = ((आदान_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  आदान_flags &= ~O_FULLBLOCK;

  जर (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (निकास_त्रुटि, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  जर (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (निकास_त्रुटि, 0, _("cannot combine block and unblock"));
  जर (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (निकास_त्रुटि, 0, _("cannot combine lcase and ucase"));
  जर (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (निकास_त्रुटि, 0, _("cannot combine excl and nocreat"));
  जर (multiple_bits_set (आदान_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (निकास_त्रुटि, 0, _("cannot combine direct and nocache"));

  जर (आदान_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      आदान_flags &= ~O_NOCACHE;
    }
  जर (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_records == 0 && max_bytes == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

स्थिर शून्य
apply_translations (शून्य)
{
  जर (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  जर (conversions_mask & C_UCASE)
    {
      साठी (पूर्णांक i = 0; i < 256; i++)
        trans_table[i] = बड़े (trans_table[i]);
      translation_needed = true;
    }
  अन्यथा जर (conversions_mask & C_LCASE)
    {
      साठी (पूर्णांक i = 0; i < 256; i++)
        trans_table[i] = छोटे (trans_table[i]);
      translation_needed = true;
    }

  जर (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  अन्यथा जर (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

स्थिर शून्य
translate_buffer (अक्षर *buf, idx_t nread)
{
  अक्षर *cp = buf;
  साठी (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

स्थिर अक्षर *
swab_buffer (अक्षर *buf, idx_t *nread, पूर्णांक *saved_byte)
{
  जर (*nread == 0)
    परत buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  पूर्णांक prev_saved = *saved_byte;
  जर ((prev_saved < 0) == (*nread & 1))
    {
      अचिन्हित अक्षर c = buf[--*nread];
      *saved_byte = c;
    }
  अन्यथा
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  साठी (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  जर (prev_saved < 0)
    परत buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  परत buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

स्थिर शून्य
advance_आदान_offset (intmax_t offset)
{
  जर (0 <= आदान_offset && ckd_add (&आदान_offset, आदान_offset, offset))
    आदान_offset = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

स्थिर intmax_t
skip (पूर्णांक fdesc, अक्षर स्थिरांक *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  त्रुटि_सं = 0;
  off_t offset;
  जर (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, प्रस्तुत_से))
    {
      जर (fdesc == STDIN_FILENO)
        {
           संरचना stat st;
           जर (ifstat (STDIN_FILENO, &st) != 0)
             error (निकास_त्रुटि, त्रुटि_सं, _("cannot fstat %s"), quoteaf (file));
           जर (usable_st_size (&st) && 0 < st.st_size && 0 <= आदान_offset
               && st.st_size - आदान_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_size ) / blocksize;
               offset = st.st_size - आदान_offset;
             }
           अन्यथा
             records = 0;
           advance_आदान_offset (offset);
        }
      अन्यथा
        {
          records = 0;
          *bytes = 0;
        }
      परत records;
    }
  अन्यथा
    {
      पूर्णांक lseek_errno = त्रुटि_सं;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      जर (lseek (fdesc, 0, अंत_से) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          जर (!lseek_errno)
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
          quit (निकास_त्रुटि);
        }
      /* else file_size && offset > OFF_T_MAX or file ! seekable */

      अक्षर *buf;
      जर (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      अन्यथा
        {
          alloc_obuf ();
          buf = obuf;
        }

      करा
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          जर (nread < 0)
            {
              जर (fdesc == STDIN_FILENO)
                {
                  diagnose (त्रुटि_सं, _("error reading %s"), quoteaf (file));
                  जर (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              अन्यथा
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (निकास_त्रुटि);
            }
          अन्यथा जर (nread == 0)
            थांबा;
          अन्यथा जर (fdesc == STDIN_FILENO)
            advance_आदान_offset (nread);

          जर (records != 0)
            records--;
          अन्यथा
            *bytes = 0;
        }
      पर्यंत (records || *bytes);

      परत records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

स्थिर bool
advance_आदान_after_read_error (idx_t nbytes)
{
  जर (! आदान_seekable)
    {
      जर (आदान_seek_errno == ESPIPE)
        परत true;
      त्रुटि_सं = आदान_seek_errno;
    }
  अन्यथा
    {
      off_t offset;
      advance_आदान_offset (nbytes);
      जर (आदान_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (आदान_file));
          परत false;
        }
      offset = lseek (STDIN_FILENO, 0, प्रस्तुत_से);
      जर (0 <= offset)
        {
          off_t diff;
          जर (offset == आदान_offset)
            परत true;
          diff = आदान_offset - offset;
          जर (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          जर (0 <= lseek (STDIN_FILENO, diff, प्रस्तुत_से))
            परत true;
          जर (त्रुटि_सं == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (त्रुटि_सं, _("%s: cannot seek"), quotef (आदान_file));
  परत false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

स्थिर शून्य
copy_simple (अक्षर स्थिरांक *buf, idx_t nread)
{
  अक्षर स्थिरांक *start = buf;	/* First uncopied char in BUF.  */

  करा
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      स_नकल (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      जर (oc >= output_blocksize)
        write_output ();
    }
  पर्यंत (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

स्थिर शून्य
copy_with_block (अक्षर स्थिरांक *buf, idx_t nread)
{
  साठी (idx_t i = nread; i; i--, buf++)
    {
      जर (*buf == newline_character)
        {
          जर (col < conversion_blocksize)
            {
              साठी (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      अन्यथा
        {
          जर (col == conversion_blocksize)
            r_truncate++;
          अन्यथा जर (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

स्थिर शून्य
copy_with_unblock (अक्षर स्थिरांक *buf, idx_t nread)
{
  स्थिर idx_t pending_spaces = 0;

  साठी (idx_t i = 0; i < nread; i++)
    {
      अक्षर c = buf[i];

      जर (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      अन्यथा जर (c == space_character)
        pending_spaces++;
      अन्यथा
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          पर्यंत (pending_spaces)
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

स्थिर शून्य
set_fd_flags (पूर्णांक fd, पूर्णांक add_flags, अक्षर स्थिरांक *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  जर (add_flags)
    {
      पूर्णांक old_flags = fcntl (fd, F_GETFL);
      पूर्णांक new_flags = old_flags | add_flags;
      bool ok = true;
      जर (old_flags < 0)
        ok = false;
      अन्यथा जर (old_flags != new_flags)
        {
          जर (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              संरचना stat st;
              जर (ifstat (fd, &st) != 0)
                ok = false;
              अन्यथा जर ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  त्रुटि_सं = ENOTDIR;
                  ok = false;
                }
              अन्यथा जर ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  त्रुटि_सं = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          जर (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      जर (!ok)
        error (निकास_त्रुटि, त्रुटि_सं, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

स्थिर पूर्णांक
dd_copy (शून्य)
{
  अक्षर *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  पूर्णांक exit_status = निकास_सफल;
  idx_t n_bytes_read;

  जर (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, आदान_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t आदान_offset0 = आदान_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, आदान_file,
                                 skip_records, आदान_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      जर ((us_blocks
           || (0 <= आदान_offset
               && (us_bytes_overflow
                   || us_bytes != आदान_offset - आदान_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (आदान_file));
        }
    }

  जर (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      जर (write_records != 0 || bytes != 0)
        {
          स_रखो (obuf, 0, write_records ? output_blocksize : bytes);

          करा
            {
              idx_t size = write_records ? output_blocksize : bytes;
              जर (iwrite (STDOUT_FILENO, obuf, size) != size)
                {
                  diagnose (त्रुटि_सं, _("writing to %s"), quoteaf (output_file));
                  quit (निकास_त्रुटि);
                }

              जर (write_records != 0)
                write_records--;
              अन्यथा
                bytes = 0;
            }
          पर्यंत (write_records || bytes);
        }
    }

  जर (max_records == 0 && max_bytes == 0)
    परत exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  पूर्णांक saved_byte = -1;

  पर्यंत (true)
    {
      जर (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          जर (next_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              next_time += XTIME_PRECISION;
            }
        }

      जर (r_partial + r_full >= max_records + !!max_bytes)
        थांबा;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      जर ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        स_रखो (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                आदान_blocksize);

      जर (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      अन्यथा
        nread = iread_fnc (STDIN_FILENO, ibuf, आदान_blocksize);

      जर (nread > 0)
        {
          advance_आदान_offset (nread);
          जर (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      अन्यथा जर (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          थांबा;			/* EOF.  */
        }
      अन्यथा
        {
          जर (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (त्रुटि_सं, _("error reading %s"), quoteaf (आदान_file));

          जर (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = आदान_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              जर (!advance_आदान_after_read_error (bad_portion))
                {
                  exit_status = निकास_त्रुटि;

                  /* Suppress duplicate diagnostics.  */
                  आदान_seekable = false;
                  आदान_seek_errno = ESPIPE;
                }
              जर ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              अन्यथा
                सुरूठेवा;
            }
          अन्यथा
            {
              /* Write any partial block. */
              exit_status = निकास_त्रुटि;
              थांबा;
            }
        }

      n_bytes_read = nread;

      जर (n_bytes_read < आदान_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          जर (conversions_mask & C_SYNC)
            {
              जर (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                स_रखो (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        आदान_blocksize - n_bytes_read);
              n_bytes_read = आदान_blocksize;
            }
        }
      अन्यथा
        {
          r_full++;
          partread = 0;
        }

      जर (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          जर (nwritten != n_bytes_read)
            {
              diagnose (त्रुटि_सं, _("error writing %s"), quoteaf (output_file));
              जर (nwritten != 0)
                w_partial++;
              परत निकास_त्रुटि;
            }
          अन्यथा जर (n_bytes_read == आदान_blocksize)
            w_full++;
          अन्यथा
            w_partial++;
          सुरूठेवा;
        }

      /* Do any translations on the whole buffer at once.  */

      जर (translation_needed)
        translate_buffer (ibuf, n_bytes_read);

      जर (conversions_mask & C_SWAB)
        bufstart = swab_buffer (ibuf, &n_bytes_read, &saved_byte);
      अन्यथा
        bufstart = ibuf;

      जर (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      अन्यथा जर (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      अन्यथा
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  जर (0 <= saved_byte)
    {
      अक्षर saved_char = saved_byte;
      जर (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      अन्यथा जर (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      अन्यथा
        output_char (saved_char);
    }

  जर ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      साठी (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  जर (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  जर (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      जर (nwritten != 0)
        w_partial++;
      जर (nwritten != oc)
        {
          diagnose (त्रुटि_सं, _("error writing %s"), quoteaf (output_file));
          परत निकास_त्रुटि;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  जर (final_op_was_seek)
    {
      संरचना stat stdout_stat;
      जर (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (त्रुटि_सं, _("cannot fstat %s"), quoteaf (output_file));
          परत निकास_त्रुटि;
        }
      जर (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, प्रस्तुत_से);
          जर (0 <= output_offset && stdout_stat.st_size < output_offset)
            {
              जर (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (त्रुटि_सं, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  परत निकास_त्रुटि;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  जर (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  परत exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

स्थिर पूर्णांक
synchronize_output (शून्य)
{
  पूर्णांक exit_status = 0;
  पूर्णांक mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  जर ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      जर (त्रुटि_सं != ENOSYS && त्रुटि_सं != EINVAL)
        {
          diagnose (त्रुटि_सं, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = निकास_त्रुटि;
        }
      mask |= C_FSYNC;
    }

  जर ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (त्रुटि_सं, _("fsync failed for %s"), quoteaf (output_file));
      परत निकास_त्रुटि;
    }

  परत exit_status;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  off_t offset;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  निकास_पर (maybe_close_stdout);

  page_size = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE, Version,
                                   true, usage, AUTHORS,
                                   (अक्षर स्थिरांक *) रिक्त);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  साठी (पूर्णांक i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  जर (आदान_file == रिक्त)
    {
      आदान_file = _("standard input");
      set_fd_flags (STDIN_FILENO, आदान_flags, आदान_file);
    }
  अन्यथा
    {
      जर (ifd_reopen (STDIN_FILENO, आदान_file, O_RDONLY | आदान_flags, 0) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to open %s"),
               quoteaf (आदान_file));
    }

  offset = lseek (STDIN_FILENO, 0, प्रस्तुत_से);
  आदान_seekable = (0 <= offset);
  आदान_offset = MAX (0, offset);
  आदान_seek_errno = त्रुटि_सं;

  जर (output_file == रिक्त)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  अन्यथा
    {
      mode_t perms = MODE_RW_UGO;
      पूर्णांक opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t size;
      जर ((ckd_mul (&size, seek_records, output_blocksize)
           || ckd_add (&size, seek_bytes, size))
          && !(conversions_mask & C_NOTRUNC))
        error (निकास_त्रुटि, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_records, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      जर ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to open %s"),
               quoteaf (output_file));

      जर (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          जर (iftruncate (STDOUT_FILENO, size) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              पूर्णांक ftruncate_errno = त्रुटि_सं;
              संरचना stat stdout_stat;
              जर (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (निकास_त्रुटि, त्रुटि_सं, _("cannot fstat %s"),
                       quoteaf (output_file));
              अन्यथा जर (S_ISREG (stdout_stat.st_mode)
                       || S_ISDIR (stdout_stat.st_mode)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = size;
                  error (निकास_त्रुटि, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  start_time = gethrxtime ();
  next_time = start_time + XTIME_PRECISION;

  पूर्णांक copy_status = dd_copy ();
  पूर्णांक sync_status = synchronize_output ();
  पूर्णांक exit_status = copy_status | sync_status;

  जर (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      जर (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (त्रुटि_सं, _("failed to discard cache for: %s"),
                    quotef (आदान_file));
          exit_status = निकास_त्रुटि;
        }
      जर (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (त्रुटि_सं, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = निकास_त्रुटि;
        }
    }
  अन्यथा
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      जर (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      जर (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
