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
  करेहि						\
    {						\
      obuf[oc++] = (c);				\
      जइ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  जाव (0)

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
स्थिर वर्ण ध्रुव *निवेशः_file = रिक्त;

/* The name of the output file, or NULL for the standard output. */
स्थिर वर्ण ध्रुव *output_file = रिक्त;

/* The page size on this host.  */
स्थिर idx_t page_size;

/* The number of bytes in which atomic reads are done. */
स्थिर idx_t निवेशः_blocksize = 0;

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
स्थिर पूर्णाङ्क conversions_mask = 0;

/* Open flags for the input and output files.  */
स्थिर पूर्णाङ्क निवेशः_flags = 0;
स्थिर पूर्णाङ्क output_flags = 0;

/* Status flags for what is printed to stderr.  */
स्थिर पूर्णाङ्क status_level = STATUS_DEFAULT;

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
स्थिर पूर्णाङ्क progress_len;

/* True if input is seekable.  */
स्थिर bool निवेशः_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
स्थिर पूर्णाङ्क निवेशः_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
स्थिर off_t निवेशः_offset;

/* True if a partial read should be diagnosed.  */
स्थिर bool warn_partial_read;

/* Records truncated by conv=block. */
स्थिर intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
स्थिर वर्ण newline_character = '\n';
स्थिर वर्ण space_character = ' ';

/* I/O buffers.  */
स्थिर वर्ण *ibuf;
स्थिर वर्ण *obuf;

/* Current index into 'obuf'. */
स्थिर idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
स्थिर idx_t col = 0;

/* The set of signals that are caught.  */
स्थिर sigset_t caught_signals;

/* If nonzero, the value of the pending fatal signal.  */
स्थिर sig_atomic_t चञ्चल interrupt_signal;

/* A count of the number of pending info signals that have been received.  */
स्थिर sig_atomic_t चञ्चल info_signal_गणनम्;

/* Whether to discard cache for input or output.  */
स्थिर bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
स्थिर bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
स्थिर ssize_t (*iread_fnc) (पूर्णाङ्क fd, वर्ण *buf, idx_t size);

/* A longest symbol in the struct symbol_values tables below.  */
#define LONGEST_चिह्नम् "count_bytes"

/* A symbol and the corresponding integer value.  */
संरचना चिह्नम्_मानम्
{
  वर्ण चिह्नम्[परिमाण LONGEST_चिह्नम्];
  पूर्णाङ्क मानम्;
};

/* Conversion symbols, for conv="...".  */
स्थिर संरचना चिह्नम्_मानम् ध्रुव conversions[] =
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

    O_गणनम्_BYTES = FFS_MASK (v3),
    v4 = v3 ^ O_गणनम्_BYTES,

    O_SKIP_BYTES = FFS_MASK (v4),
    v5 = v4 ^ O_SKIP_BYTES,

    O_SEEK_BYTES = FFS_MASK (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_गणनम्_BYTES != 0);
static_assert (O_SKIP_BYTES != 0);
static_assert (O_SEEK_BYTES != 0);

#define MULTIPLE_BITS_SET(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! MULTIPLE_BITS_SET (O_FULLBLOCK));
static_assert ( ! MULTIPLE_BITS_SET (O_NOCACHE));
static_assert ( ! MULTIPLE_BITS_SET (O_गणनम्_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SKIP_BYTES));
static_assert ( ! MULTIPLE_BITS_SET (O_SEEK_BYTES));

/* Flags, for iflag="..." and oflag="...".  */
स्थिर संरचना चिह्नम्_मानम् ध्रुव flags[] =
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
  {"count_bytes", O_गणनम्_BYTES},
  {"skip_bytes",  O_SKIP_BYTES},
  {"seek_bytes",  O_SEEK_BYTES},
  {"",		0}
};

/* Status, for status="...".  */
स्थिर संरचना चिह्नम्_मानम् ध्रुव statuses[] =
{
  {"none",	STATUS_NONE},
  {"noxfer",	STATUS_NOXFER},
  {"progress",	STATUS_PROGRESS},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
स्थिर unsigned वर्ण trans_table[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

स्थिर वर्ण ध्रुव ascii_to_ebcdic[] =
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

स्थिर वर्ण ध्रुव ascii_to_ibm[] =
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

स्थिर वर्ण ध्रुव ebcdic_to_ascii[] =
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
  जइ (close_stdout_required)
    close_stdout ();
  अण्णहा जइ (close_stream (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
स्थिर शून्य
diagnose (पूर्णाङ्क errnum, वर्ण ध्रुव *fmt, ...)
{
  जइ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  va_list ap;
  va_start (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_end (ap);
}

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
  bs=BYTES        पठ and लिख up to BYTES bytes at a time (स्वभाव: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=BYTES       convert BYTES bytes at a time\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as per the comma separated चिह्नम् list\n\
"));
      oputs (_("\
  गणनम्=N         copy only N निवेशः blocks\n\
"));
      oputs (_("\
  ibs=BYTES       पठ up to BYTES bytes at a time (स्वभाव: 512)\n\
"));
      oputs (_("\
  जइ=FILE         पठ from FILE instead of standard निवेशः\n\
"));
      oputs (_("\
  iflag=FLAGS     पठ as per the comma separated चिह्नम् list\n\
"));
      oputs (_("\
  obs=BYTES       लिख BYTES bytes at a time (स्वभाव: 512)\n\
"));
      oputs (_("\
  of=FILE         लिख to FILE instead of standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     लिख as per the comma separated चिह्नम् list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output blocks\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized निवेशः blocks\n\
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
GB=1000*1000*1000, G=1024*1024*1024, and so on प्रति T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
If N ends in 'B', it counts bytes not blocks.\n\
\n\
Each CONV चिह्नम् may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC to ASCII\n\
  ebcdic    from ASCII to EBCDIC\n\
  ibm       from ASCII to alternate EBCDIC\n\
  block     pad newline-terminated records with spaces to cbs-size\n\
  unblock   replace trailing spaces in cbs-size records with newline\n\
  lcase     change upper पक्ष to lower पक्ष\n\
  ucase     change lower पक्ष to upper पक्ष\n\
  sparse    try to seek rather than लिख all-NUL output blocks\n\
  swab      swap every pair of निवेशः bytes\n\
  sync      pad every निवेशः block with NULs to ibs-size; when used\n\
            with block or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail जइ the output file already exists\n\
  nocreat   करेहि not create the output file\n\
  notrunc   करेहि not truncate the output file\n\
  noerror   अनुवर्त after पठ errors\n\
  fdatasync  physically लिख output file data before finishing\n\
  fsync     likewise, but also लिख metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG चिह्नम् may be:\n\
\n\
  append    append mode (makes sense only प्रति output; conv=notrunc suggested)\n\
"), stdout);
      जइ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      जइ (O_DIRECT)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      जइ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      जइ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      जइ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      जइ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#जइ HAVE_POSIX_FADVISE
      जइ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#endif
      जइ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      जइ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      जइ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      जइ (O_द्विमान)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      जइ (O_TEXT)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        printf (_("\
\n\
Sending a %s सङ्केत to a running 'dd' process makes it\n\
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

गणना { human_opts = (human_autoscale | human_round_to_nearest
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

स्थिर शून्य
alloc_ibuf (शून्य)
{
  जइ (ibuf)
    प्रत्यागम;

  bool extra_byte_for_swab = !!(conversions_mask & C_SWAB);
  ibuf = alignalloc (page_size, निवेशः_blocksize + extra_byte_for_swab);
  जइ (!ibuf)
    {
      वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             निवेशः_blocksize,
             human_readable (निवेशः_blocksize, hbuf,
                             human_opts | human_base_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

स्थिर शून्य
alloc_obuf (शून्य)
{
  जइ (obuf)
    प्रत्यागम;

  जइ (conversions_mask & C_TWOBUFS)
    {
      obuf = alignalloc (page_size, output_blocksize);
      जइ (!obuf)
        {
          वर्ण hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_base_1024, 1, 1));
        }
    }
  अण्णहा
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

स्थिर शून्य
translate_charset (वर्ण ध्रुव *new_trans)
{
  प्रति (पूर्णाङ्क i = 0; i < 256; i++)
    trans_table[i] = new_trans[trans_table[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

स्थिर अन्तःस्थ bool
multiple_bits_set (पूर्णाङ्क i)
{
  प्रत्यागम MULTIPLE_BITS_SET (i);
}

स्थिर bool
abbreviation_lacks_prefix (वर्ण ध्रुव *message)
{
  प्रत्यागम message[strlen (message) - 2] == ' ';
}

/* Print transfer statistics.  */

स्थिर शून्य
print_xfer_stats (xtime_t progress_time)
{
  xtime_t now = progress_time ? progress_time : gethrxtime ();
  स्थिर वर्ण ध्रुव slash_s[] = "/s";
  वर्ण hbuf[3][LONGEST_HUMAN_READABLE + परिमाण slash_s];
  द्विगुण delta_s;
  वर्ण ध्रुव *bytes_per_second;
  वर्ण ध्रुव *si = human_readable (w_bytes, hbuf[0], human_opts, 1, 1);
  वर्ण ध्रुव *iec = human_readable (w_bytes, hbuf[1],
                                    human_opts | human_base_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  वर्ण *bpsbuf = hbuf[2];
  पूर्णाङ्क bpsbufsize = परिमाण hbuf[2];
  जइ (start_time < now)
    {
      द्विगुण XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - start_time;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      bytes_per_second = human_readable (w_bytes, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (bytes_per_second - bpsbuf + bpsbuf, slash_s);
    }
  अण्णहा
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      bytes_per_second = bpsbuf;
    }

  जइ (progress_time)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  वर्ण delta_s_buf[24];
  snprintf (delta_s_buf, परिमाण delta_s_buf,
            progress_time ? "%.0f s" : "%g s", delta_s);

  पूर्णाङ्क stats_len
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

  जइ (progress_time)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      जइ (0 <= stats_len && stats_len < progress_len)
        fprintf (stderr, "%*s", progress_len - stats_len, "");
      progress_len = stats_len;
    }
  अण्णहा
    fputc ('\n', stderr);

  reported_w_bytes = w_bytes;
}

स्थिर शून्य
print_stats (शून्य)
{
  जइ (status_level == STATUS_NONE)
    प्रत्यागम;

  जइ (0 < progress_len)
    {
      fputc ('\n', stderr);
      progress_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_full, r_partial, w_full, w_partial);

  जइ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  जइ (status_level == STATUS_NOXFER)
    प्रत्यागम;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

स्थिर शून्य
interrupt_handler (पूर्णाङ्क sig)
{
  जइ (! SA_RESETHAND)
    सङ्केत (sig, SIG_DFL);
  interrupt_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

स्थिर शून्य
siginfo_handler (MAYBE_UNUSED पूर्णाङ्क sig)
{
  info_signal_गणनम्++;
}

/* Install the signal handlers.  */

स्थिर शून्य
install_signal_handlers (शून्य)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  संरचना sigaction act;
  sigemptyset (&caught_signals);
  जइ (catch_siginfo)
    sigaddset (&caught_signals, SIGINFO);
  sigaction (SIGINT, रिक्त, &act);
  जइ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_signals, SIGINT);
  act.sa_mask = caught_signals;

  जइ (sigismember (&caught_signals, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, रिक्त);
    }

  जइ (sigismember (&caught_signals, SIGINT))
    {
      act.sa_handler = interrupt_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, रिक्त);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
स्थिर पूर्णाङ्क
iclose (पूर्णाङ्क fd)
{
  जइ (close (fd) != 0)
    करेहि
      जइ (errno != EINTR)
        प्रत्यागम -1;
    जाव (close (fd) != 0 && errno != EBADF);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क synchronize_output (शून्य);

स्थिर शून्य
cleanup (शून्य)
{
  जइ (!interrupt_signal)
    {
      पूर्णाङ्क sync_status = synchronize_output ();
      जइ (sync_status)
        exit (sync_status);
    }

  जइ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (निवेशः_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  जइ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

स्थिर शून्य
process_signals (शून्य)
{
  जाव (interrupt_signal || info_signal_गणनम्)
    {
      पूर्णाङ्क interrupt;
      पूर्णाङ्क infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      interrupt = interrupt_signal;
      infos = info_signal_गणनम्;

      जइ (infos)
        info_signal_गणनम् = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, रिक्त);

      जइ (interrupt)
        cleanup ();
      print_stats ();
      जइ (interrupt)
        उत्थापय (interrupt);
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
quit (पूर्णाङ्क code)
{
  finish_up ();
  exit (code);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

स्थिर off_t
cache_round (पूर्णाङ्क fd, off_t len)
{
  स्थिर off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  जइ (len)
    {
      intmax_t c_pending;
      जइ (ckd_add (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      जइ (c_pending > *pending)
        len = c_pending - *pending;
      अण्णहा
        len = 0;
    }
  अण्णहा
    len = *pending;

  प्रत्यागम len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

स्थिर bool
invalidate_cache (पूर्णाङ्क fd, off_t len)
{
  पूर्णाङ्क adv_ret = -1;
  off_t offset;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_round (fd, len);
  जइ (len && !clen)
    प्रत्यागम true; /* Don't advise this time.  */
  अण्णहा जइ (! len && ! clen && ! nocache_eof)
    प्रत्यागम true;
  off_t pending = len ? cache_round (fd, 0) : 0;

  जइ (fd == STDIN_FILENO)
    {
      जइ (निवेशः_seekable)
        offset = निवेशः_offset;
      अण्णहा
        {
          offset = -1;
          errno = ESPIPE;
        }
    }
  अण्णहा
    {
      स्थिर off_t output_offset = -2;

      जइ (output_offset != -1)
        {
          जइ (output_offset < 0)
            output_offset = lseek (fd, 0, SEEK_CUR);
          अण्णहा जइ (len)
            output_offset += clen + pending;
        }

      offset = output_offset;
    }

  जइ (0 <= offset)
   {
     जइ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#जइ HAVE_POSIX_FADVISE
     offset = offset - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     जइ (clen == 0)
       offset -= offset % page_size;
     adv_ret = posix_fadvise (fd, offset, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#अण्णहा
     errno = ENOTSUP;
#endif
   }

  प्रत्यागम adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

स्थिर ssize_t
iread (पूर्णाङ्क fd, वर्ण *buf, idx_t size)
{
  ssize_t nread;
  स्थिर ssize_t prev_nread;

  करेहि
    {
      process_signals ();
      nread = पठ (fd, buf, size);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      जइ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < size
          && (निवेशः_flags & O_DIRECT))
        {
          errno = 0;
          nread = 0;
        }
    }
  जाव (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  जइ (0 < nread && nread < size)
    process_signals ();

  जइ (0 < nread && warn_partial_read)
    {
      जइ (0 < prev_nread && prev_nread < size)
        {
          idx_t prev = prev_nread;
          जइ (status_level != STATUS_NONE)
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
  प्रत्यागम nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
स्थिर ssize_t
iread_fullblock (पूर्णाङ्क fd, वर्ण *buf, idx_t size)
{
  ssize_t nread = 0;

  जाव (0 < size)
    {
      ssize_t ncurr = iread (fd, buf, size);
      जइ (ncurr < 0)
        प्रत्यागम ncurr;
      जइ (ncurr == 0)
        भङ्ग;
      nread += ncurr;
      buf   += ncurr;
      size  -= ncurr;
    }

  प्रत्यागम nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

स्थिर idx_t
iwrite (पूर्णाङ्क fd, वर्ण ध्रुव *buf, idx_t size)
{
  idx_t total_written = 0;

  जइ ((output_flags & O_DIRECT) && size < output_blocksize)
    {
      पूर्णाङ्क old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      जइ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_DIRECT) != 0
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

  जाव (total_written < size)
    {
      ssize_t nwritten = 0;
      process_signals ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      जइ ((conversions_mask & C_SPARSE) && is_nul (buf, size))
        {
          जइ (lseek (fd, size, SEEK_CUR) < 0)
            {
              conversions_mask &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          अण्णहा
            {
              final_op_was_seek = true;
              nwritten = size;
            }
        }

      जइ (!nwritten)
        nwritten = लिख (fd, buf + total_written, size - total_written);

      जइ (nwritten < 0)
        {
          जइ (errno != EINTR)
            भङ्ग;
        }
      अण्णहा जइ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          भङ्ग;
        }
      अण्णहा
        total_written += nwritten;
    }

  जइ (o_nocache && total_written)
    invalidate_cache (fd, total_written);

  प्रत्यागम total_written;
}

/* Write, then empty, the output buffer 'obuf'. */

स्थिर शून्य
write_output (शून्य)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_bytes += nwritten;
  जइ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      जइ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  अण्णहा
    w_full++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

स्थिर पूर्णाङ्क
ifdatasync (पूर्णाङ्क fd)
{
  पूर्णाङ्क ret;

  करेहि
    {
      process_signals ();
      ret = fdatasync (fd);
    }
  जाव (ret < 0 && errno == EINTR);

  प्रत्यागम ret;
}

/* Restart on EINTR from fd_reopen.  */

स्थिर पूर्णाङ्क
ifd_reopen (पूर्णाङ्क desired_fd, वर्ण ध्रुव *file, पूर्णाङ्क flag, mode_t mode)
{
  पूर्णाङ्क ret;

  करेहि
    {
      process_signals ();
      ret = fd_reopen (desired_fd, file, flag, mode);
    }
  जाव (ret < 0 && errno == EINTR);

  प्रत्यागम ret;
}

/* Restart on EINTR from fstat.  */

स्थिर पूर्णाङ्क
ifstat (पूर्णाङ्क fd, संरचना stat *st)
{
  पूर्णाङ्क ret;

  करेहि
    {
      process_signals ();
      ret = fstat (fd, st);
    }
  जाव (ret < 0 && errno == EINTR);

  प्रत्यागम ret;
}

/* Restart on EINTR from fsync.  */

स्थिर पूर्णाङ्क
ifsync (पूर्णाङ्क fd)
{
  पूर्णाङ्क ret;

  करेहि
    {
      process_signals ();
      ret = fsync (fd);
    }
  जाव (ret < 0 && errno == EINTR);

  प्रत्यागम ret;
}

/* Restart on EINTR from ftruncate.  */

स्थिर पूर्णाङ्क
iftruncate (पूर्णाङ्क fd, off_t length)
{
  पूर्णाङ्क ret;

  करेहि
    {
      process_signals ();
      ret = ftruncate (fd, length);
    }
  जाव (ret < 0 && errno == EINTR);

  प्रत्यागम ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_PURE
स्थिर bool
operand_matches (वर्ण ध्रुव *str, वर्ण ध्रुव *pattern, वर्ण delim)
{
  जाव (*pattern)
    जइ (*str++ != *pattern++)
      प्रत्यागम false;
  प्रत्यागम !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

स्थिर पूर्णाङ्क
parse_symbols (वर्ण ध्रुव *str, संरचना चिह्नम्_मानम् ध्रुव *table,
               bool exclusive, वर्ण ध्रुव *error_msgid)
{
  पूर्णाङ्क मानम् = 0;

  जाव (true)
    {
      वर्ण ध्रुव *strcomma = strchr (str, ',');
      संरचना चिह्नम्_मानम् ध्रुव *entry;

      प्रति (entry = table;
           ! (operand_matches (str, entry->चिह्नम्, ',') && entry->मानम्);
           entry++)
        {
          जइ (! entry->चिह्नम्[0])
            {
              idx_t slen = strcomma ? strcomma - str : strlen (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      जइ (exclusive)
        मानम् = entry->मानम्;
      अण्णहा
        मानम् |= entry->मानम्;
      जइ (!strcomma)
        भङ्ग;
      str = strcomma + 1;
    }

  प्रत्यागम मानम्;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

स्थिर intmax_t
parse_integer (वर्ण ध्रुव *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  पूर्णाङ्क indeterminate = 0;
  स्थिर वर्ण ध्रुव suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t result = 1;
  bool overflow = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  जाव (true)
    {
      uintmax_t n = indeterminate;
      वर्ण *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      जइ ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          जइ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_OVERFLOW) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      जइ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      जइ ((e & ~LONGINT_OVERFLOW) != LONGINT_OK)
        {
          जइ (! (e & LONGINT_OVERFLOW) && n <= INTMAX_MAX)
            {
              *invalid = e;
              प्रत्यागम indeterminate;
            }
          e = LONGINT_OVERFLOW;
        }

      जइ (n == 0)
        result = 0;
      अण्णहा जइ (result != 0)
        {
          intmax_t product;
          जइ ((e & LONGINT_OVERFLOW)
              || overflow
              || ckd_mul (&product, result, n))
            overflow = true;
          अण्णहा
            result = product;
        }

      जइ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      जइ (! multiply)
        भङ्ग;
      str = suffix + 1;
    }

  जइ (result == 0)
    {
      जइ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  अण्णहा जइ (overflow)
    {
      e = LONGINT_OVERFLOW;
      result = INTMAX_MAX;
    }

  *invalid = e;
  प्रत्यागम result;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_PURE
स्थिर bool
operand_is (वर्ण ध्रुव *operand, वर्ण ध्रुव *name)
{
  प्रत्यागम operand_matches (operand, name, '=');
}

स्थिर शून्य
scanargs (पूर्णाङ्क argc, वर्ण *ध्रुव *argv)
{
  idx_t blocksize = 0;
  intmax_t गणनम् = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool गणनम्_B = false, skip_B = false, seek_B = false;

  प्रति (पूर्णाङ्क i = optind; i < argc; i++)
    {
      वर्ण ध्रुव *name = argv[i];
      वर्ण ध्रुव *val = strchr (name, '=');

      जइ (val == रिक्त)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      जइ (operand_is (name, "if"))
        निवेशः_file = val;
      अण्णहा जइ (operand_is (name, "of"))
        output_file = val;
      अण्णहा जइ (operand_is (name, "conv"))
        conversions_mask |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      अण्णहा जइ (operand_is (name, "iflag"))
        निवेशः_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      अण्णहा जइ (operand_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      अण्णहा जइ (operand_is (name, "status"))
        status_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      अण्णहा
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = रिक्त;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          जइ (operand_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &निवेशः_blocksize;
            }
          अण्णहा जइ (operand_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          अण्णहा जइ (operand_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          अण्णहा जइ (operand_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          अण्णहा जइ (operand_is (name, "skip") || operand_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          अण्णहा जइ (operand_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          अण्णहा जइ (operand_is (name, "count"))
            {
              गणनम् = n;
              गणनम्_B = has_B;
            }
          अण्णहा
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          जइ (n < n_min)
            invalid = LONGINT_INVALID;
          अण्णहा जइ (n_max < n)
            invalid = LONGINT_OVERFLOW;

          जइ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          अण्णहा जइ (converted_idx)
            *converted_idx = n;
        }
    }

  जइ (blocksize)
    निवेशः_blocksize = output_blocksize = blocksize;
  अण्णहा
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_mask |= C_TWOBUFS;
    }

  जइ (निवेशः_blocksize == 0)
    निवेशः_blocksize = DEFAULT_BLOCKSIZE;
  जइ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  जइ (conversion_blocksize == 0)
    conversions_mask &= ~(C_BLOCK | C_UNBLOCK);

  जइ (निवेशः_flags & (O_DSYNC | O_SYNC))
    निवेशः_flags |= O_RSYNC;

  जइ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  जइ (skip_B)
    निवेशः_flags |= O_SKIP_BYTES;
  जइ (निवेशः_flags & O_SKIP_BYTES && skip != 0)
    {
      skip_records = skip / निवेशः_blocksize;
      skip_bytes = skip % निवेशः_blocksize;
    }
  अण्णहा जइ (skip != 0)
    skip_records = skip;

  जइ (गणनम्_B)
    निवेशः_flags |= O_गणनम्_BYTES;
  जइ (निवेशः_flags & O_गणनम्_BYTES && गणनम् != INTMAX_MAX)
    {
      max_records = गणनम् / निवेशः_blocksize;
      max_bytes = गणनम् % निवेशः_blocksize;
    }
  अण्णहा जइ (गणनम् != INTMAX_MAX)
    max_records = गणनम्;

  जइ (seek_B)
    output_flags |= O_SEEK_BYTES;
  जइ (output_flags & O_SEEK_BYTES && seek != 0)
    {
      seek_records = seek / output_blocksize;
      seek_bytes = seek % output_blocksize;
    }
  अण्णहा जइ (seek != 0)
    seek_records = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_mask & C_TWOBUFS) && ! (निवेशः_flags & O_FULLBLOCK)
     && (skip_records
         || (0 < max_records && max_records < INTMAX_MAX)
         || (निवेशः_flags | output_flags) & O_DIRECT));

  iread_fnc = ((निवेशः_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  निवेशः_flags &= ~O_FULLBLOCK;

  जइ (multiple_bits_set (conversions_mask & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  जइ (multiple_bits_set (conversions_mask & (C_BLOCK | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  जइ (multiple_bits_set (conversions_mask & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  जइ (multiple_bits_set (conversions_mask & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  जइ (multiple_bits_set (निवेशः_flags & (O_DIRECT | O_NOCACHE))
      || multiple_bits_set (output_flags & (O_DIRECT | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  जइ (निवेशः_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_records == 0 && max_bytes == 0);
      निवेशः_flags &= ~O_NOCACHE;
    }
  जइ (output_flags & O_NOCACHE)
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
  जइ (conversions_mask & C_ASCII)
    translate_charset (ebcdic_to_ascii);

  जइ (conversions_mask & C_UCASE)
    {
      प्रति (पूर्णाङ्क i = 0; i < 256; i++)
        trans_table[i] = toupper (trans_table[i]);
      translation_needed = true;
    }
  अण्णहा जइ (conversions_mask & C_LCASE)
    {
      प्रति (पूर्णाङ्क i = 0; i < 256; i++)
        trans_table[i] = tolower (trans_table[i]);
      translation_needed = true;
    }

  जइ (conversions_mask & C_EBCDIC)
    {
      translate_charset (ascii_to_ebcdic);
      newline_character = ascii_to_ebcdic['\n'];
      space_character = ascii_to_ebcdic[' '];
    }
  अण्णहा जइ (conversions_mask & C_IBM)
    {
      translate_charset (ascii_to_ibm);
      newline_character = ascii_to_ibm['\n'];
      space_character = ascii_to_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

स्थिर शून्य
translate_buffer (वर्ण *buf, idx_t nread)
{
  वर्ण *cp = buf;
  प्रति (idx_t i = nread; i; i--, cp++)
    *cp = trans_table[to_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

स्थिर वर्ण *
swab_buffer (वर्ण *buf, idx_t *nread, पूर्णाङ्क *saved_byte)
{
  जइ (*nread == 0)
    प्रत्यागम buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  पूर्णाङ्क prev_saved = *saved_byte;
  जइ ((prev_saved < 0) == (*nread & 1))
    {
      unsigned वर्ण c = buf[--*nread];
      *saved_byte = c;
    }
  अण्णहा
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  प्रति (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  जइ (prev_saved < 0)
    प्रत्यागम buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  प्रत्यागम buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

स्थिर शून्य
advance_निवेशः_offset (intmax_t offset)
{
  जइ (0 <= निवेशः_offset && ckd_add (&निवेशः_offset, निवेशः_offset, offset))
    निवेशः_offset = -1;
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
skip (पूर्णाङ्क fdesc, वर्ण ध्रुव *file, intmax_t records, idx_t blocksize,
      idx_t *bytes)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t offset;
  जइ (! ckd_mul (&offset, records, blocksize)
      && ! ckd_add (&offset, offset, *bytes)
      && 0 <= lseek (fdesc, offset, SEEK_CUR))
    {
      जइ (fdesc == STDIN_FILENO)
        {
           संरचना stat st;
           जइ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           जइ (usable_st_size (&st) && 0 < st.st_size && 0 <= निवेशः_offset
               && st.st_size - निवेशः_offset < offset)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               records = ( offset - st.st_size ) / blocksize;
               offset = st.st_size - निवेशः_offset;
             }
           अण्णहा
             records = 0;
           advance_निवेशः_offset (offset);
        }
      अण्णहा
        {
          records = 0;
          *bytes = 0;
        }
      प्रत्यागम records;
    }
  अण्णहा
    {
      पूर्णाङ्क lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      जइ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          जइ (!lseek_errno)
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

      वर्ण *buf;
      जइ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      अण्णहा
        {
          alloc_obuf ();
          buf = obuf;
        }

      करेहि
        {
          ssize_t nread = iread_fnc (fdesc, buf, records ? blocksize : *bytes);
          जइ (nread < 0)
            {
              जइ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  जइ (conversions_mask & C_NOERROR)
                    print_stats ();
                }
              अण्णहा
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          अण्णहा जइ (nread == 0)
            भङ्ग;
          अण्णहा जइ (fdesc == STDIN_FILENO)
            advance_निवेशः_offset (nread);

          जइ (records != 0)
            records--;
          अण्णहा
            *bytes = 0;
        }
      जाव (records || *bytes);

      प्रत्यागम records;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

स्थिर bool
advance_निवेशः_after_read_error (idx_t nbytes)
{
  जइ (! निवेशः_seekable)
    {
      जइ (निवेशः_seek_errno == ESPIPE)
        प्रत्यागम true;
      errno = निवेशः_seek_errno;
    }
  अण्णहा
    {
      off_t offset;
      advance_निवेशः_offset (nbytes);
      जइ (निवेशः_offset < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (निवेशः_file));
          प्रत्यागम false;
        }
      offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
      जइ (0 <= offset)
        {
          off_t diff;
          जइ (offset == निवेशः_offset)
            प्रत्यागम true;
          diff = निवेशः_offset - offset;
          जइ (! (0 <= diff && diff <= nbytes) && status_level != STATUS_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          जइ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            प्रत्यागम true;
          जइ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (निवेशः_file));
  प्रत्यागम false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

स्थिर शून्य
copy_simple (वर्ण ध्रुव *buf, idx_t nread)
{
  वर्ण ध्रुव *start = buf;	/* First uncopied char in BUF.  */

  करेहि
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      memcpy (obuf + oc, start, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      start += nfree;
      oc += nfree;
      जइ (oc >= output_blocksize)
        write_output ();
    }
  जाव (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

स्थिर शून्य
copy_with_block (वर्ण ध्रुव *buf, idx_t nread)
{
  प्रति (idx_t i = nread; i; i--, buf++)
    {
      जइ (*buf == newline_character)
        {
          जइ (col < conversion_blocksize)
            {
              प्रति (idx_t j = col; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          col = 0;
        }
      अण्णहा
        {
          जइ (col == conversion_blocksize)
            r_truncate++;
          अण्णहा जइ (col < conversion_blocksize)
            output_char (*buf);
          col++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

स्थिर शून्य
copy_with_unblock (वर्ण ध्रुव *buf, idx_t nread)
{
  स्थिर idx_t pending_spaces = 0;

  प्रति (idx_t i = 0; i < nread; i++)
    {
      वर्ण c = buf[i];

      जइ (col++ >= conversion_blocksize)
        {
          col = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      अण्णहा जइ (c == space_character)
        pending_spaces++;
      अण्णहा
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          जाव (pending_spaces)
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
set_fd_flags (पूर्णाङ्क fd, पूर्णाङ्क add_flags, वर्ण ध्रुव *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  add_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  जइ (add_flags)
    {
      पूर्णाङ्क old_flags = fcntl (fd, F_GETFL);
      पूर्णाङ्क new_flags = old_flags | add_flags;
      bool ok = true;
      जइ (old_flags < 0)
        ok = false;
      अण्णहा जइ (old_flags != new_flags)
        {
          जइ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              संरचना stat st;
              जइ (ifstat (fd, &st) != 0)
                ok = false;
              अण्णहा जइ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_mode))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              अण्णहा जइ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          जइ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      जइ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

स्थिर पूर्णाङ्क
dd_copy (शून्य)
{
  वर्ण *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  पूर्णाङ्क exit_status = EXIT_SUCCESS;
  idx_t n_bytes_read;

  जइ (skip_records != 0 || skip_bytes != 0)
    {
      intmax_t us_bytes;
      bool us_bytes_overflow =
        (ckd_mul (&us_bytes, skip_records, निवेशः_blocksize)
         || ckd_add (&us_bytes, skip_bytes, us_bytes));
      off_t निवेशः_offset0 = निवेशः_offset;
      intmax_t us_blocks = skip (STDIN_FILENO, निवेशः_file,
                                 skip_records, निवेशः_blocksize, &skip_bytes);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      जइ ((us_blocks
           || (0 <= निवेशः_offset
               && (us_bytes_overflow
                   || us_bytes != निवेशः_offset - निवेशः_offset0)))
          && status_level != STATUS_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (निवेशः_file));
        }
    }

  जइ (seek_records != 0 || seek_bytes != 0)
    {
      idx_t bytes = seek_bytes;
      intmax_t write_records = skip (STDOUT_FILENO, output_file,
                                      seek_records, output_blocksize, &bytes);

      जइ (write_records != 0 || bytes != 0)
        {
          memset (obuf, 0, write_records ? output_blocksize : bytes);

          करेहि
            {
              idx_t size = write_records ? output_blocksize : bytes;
              जइ (iwrite (STDOUT_FILENO, obuf, size) != size)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              जइ (write_records != 0)
                write_records--;
              अण्णहा
                bytes = 0;
            }
          जाव (write_records || bytes);
        }
    }

  जइ (max_records == 0 && max_bytes == 0)
    प्रत्यागम exit_status;

  alloc_ibuf ();
  alloc_obuf ();
  पूर्णाङ्क saved_byte = -1;

  जाव (true)
    {
      जइ (status_level == STATUS_PROGRESS)
        {
          xtime_t progress_time = gethrxtime ();
          जइ (next_time <= progress_time)
            {
              print_xfer_stats (progress_time);
              next_time += XTIME_PRECISION;
            }
        }

      जइ (r_partial + r_full >= max_records + !!max_bytes)
        भङ्ग;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      जइ ((conversions_mask & C_SYNC) && (conversions_mask & C_NOERROR))
        memset (ibuf,
                (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                निवेशः_blocksize);

      जइ (r_partial + r_full >= max_records)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_bytes);
      अण्णहा
        nread = iread_fnc (STDIN_FILENO, ibuf, निवेशः_blocksize);

      जइ (nread > 0)
        {
          advance_निवेशः_offset (nread);
          जइ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      अण्णहा जइ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_mask & C_NOTRUNC);
          भङ्ग;			/* EOF.  */
        }
      अण्णहा
        {
          जइ (!(conversions_mask & C_NOERROR) || status_level != STATUS_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (निवेशः_file));

          जइ (conversions_mask & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = निवेशः_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              जइ (!advance_निवेशः_after_read_error (bad_portion))
                {
                  exit_status = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  निवेशः_seekable = false;
                  निवेशः_seek_errno = ESPIPE;
                }
              जइ ((conversions_mask & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              अण्णहा
                अनुवर्त;
            }
          अण्णहा
            {
              /* Write any partial block. */
              exit_status = EXIT_FAILURE;
              भङ्ग;
            }
        }

      n_bytes_read = nread;

      जइ (n_bytes_read < निवेशः_blocksize)
        {
          r_partial++;
          partread = n_bytes_read;
          जइ (conversions_mask & C_SYNC)
            {
              जइ (!(conversions_mask & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_bytes_read,
                        (conversions_mask & (C_BLOCK | C_UNBLOCK)) ? ' ' : '\0',
                        निवेशः_blocksize - n_bytes_read);
              n_bytes_read = निवेशः_blocksize;
            }
        }
      अण्णहा
        {
          r_full++;
          partread = 0;
        }

      जइ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_bytes_read);
          w_bytes += nwritten;
          जइ (nwritten != n_bytes_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              जइ (nwritten != 0)
                w_partial++;
              प्रत्यागम EXIT_FAILURE;
            }
          अण्णहा जइ (n_bytes_read == निवेशः_blocksize)
            w_full++;
          अण्णहा
            w_partial++;
          अनुवर्त;
        }

      /* Do any translations on the whole buffer at once.  */

      जइ (translation_needed)
        translate_buffer (ibuf, n_bytes_read);

      जइ (conversions_mask & C_SWAB)
        bufstart = swab_buffer (ibuf, &n_bytes_read, &saved_byte);
      अण्णहा
        bufstart = ibuf;

      जइ (conversions_mask & C_BLOCK)
        copy_with_block (bufstart, n_bytes_read);
      अण्णहा जइ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (bufstart, n_bytes_read);
      अण्णहा
        copy_simple (bufstart, n_bytes_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  जइ (0 <= saved_byte)
    {
      वर्ण saved_char = saved_byte;
      जइ (conversions_mask & C_BLOCK)
        copy_with_block (&saved_char, 1);
      अण्णहा जइ (conversions_mask & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      अण्णहा
        output_char (saved_char);
    }

  जइ ((conversions_mask & C_BLOCK) && col > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      प्रति (idx_t i = col; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  जइ (col && (conversions_mask & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  जइ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_bytes += nwritten;
      जइ (nwritten != 0)
        w_partial++;
      जइ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          प्रत्यागम EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  जइ (final_op_was_seek)
    {
      संरचना stat stdout_stat;
      जइ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          प्रत्यागम EXIT_FAILURE;
        }
      जइ (S_ISREG (stdout_stat.st_mode) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_offset = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          जइ (0 <= output_offset && stdout_stat.st_size < output_offset)
            {
              जइ (iftruncate (STDOUT_FILENO, output_offset) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_offset}, quoteaf (output_file));
                  प्रत्यागम EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  जइ (conversions_mask & (C_FDATASYNC | C_FSYNC)
      && status_level == STATUS_PROGRESS
      && 0 <= reported_w_bytes && reported_w_bytes < w_bytes)
    print_xfer_stats (0);

  प्रत्यागम exit_status;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

स्थिर पूर्णाङ्क
synchronize_output (शून्य)
{
  पूर्णाङ्क exit_status = 0;
  पूर्णाङ्क mask = conversions_mask;
  conversions_mask &= ~ (C_FDATASYNC | C_FSYNC);

  जइ ((mask & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      जइ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_status = EXIT_FAILURE;
        }
      mask |= C_FSYNC;
    }

  जइ ((mask & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      प्रत्यागम EXIT_FAILURE;
    }

  प्रत्यागम exit_status;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
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
                                   (वर्ण ध्रुव *) रिक्त);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  प्रति (पूर्णाङ्क i = 0; i < 256; i++)
    trans_table[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  जइ (निवेशः_file == रिक्त)
    {
      निवेशः_file = _("standard input");
      set_fd_flags (STDIN_FILENO, निवेशः_flags, निवेशः_file);
    }
  अण्णहा
    {
      जइ (ifd_reopen (STDIN_FILENO, निवेशः_file, O_RDONLY | निवेशः_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (निवेशः_file));
    }

  offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  निवेशः_seekable = (0 <= offset);
  निवेशः_offset = MAX (0, offset);
  निवेशः_seek_errno = errno;

  जइ (output_file == रिक्त)
    {
      output_file = _("standard output");
      set_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  अण्णहा
    {
      mode_t perms = MODE_RW_UGO;
      पूर्णाङ्क opts
        = (output_flags
           | (conversions_mask & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_mask & C_EXCL ? O_EXCL : 0)
           | (seek_records || (conversions_mask & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t size;
      जइ ((ckd_mul (&size, seek_records, output_blocksize)
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
      जइ ((! seek_records
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      जइ (seek_records != 0 && !(conversions_mask & C_NOTRUNC))
        {
          जइ (iftruncate (STDOUT_FILENO, size) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              पूर्णाङ्क ftruncate_errno = errno;
              संरचना stat stdout_stat;
              जइ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              अण्णहा जइ (S_ISREG (stdout_stat.st_mode)
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

  पूर्णाङ्क copy_status = dd_copy ();
  पूर्णाङ्क sync_status = synchronize_output ();
  पूर्णाङ्क exit_status = copy_status | sync_status;

  जइ (max_records == 0 && max_bytes == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      जइ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (निवेशः_file));
          exit_status = EXIT_FAILURE;
        }
      जइ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_status = EXIT_FAILURE;
        }
    }
  अण्णहा
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      जइ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      जइ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  finish_up ();
  main_exit (exit_status);
}
