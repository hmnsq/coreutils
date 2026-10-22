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

#అన్తర్భావ <config.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "alignalloc.h"
#అన్తర్భావ "close-stream.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "gethrxtime.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "dd"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#నిర్వచితం_కాకపోతే SA_RESETHAND
# define SA_RESETHAND 0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SIGINFO
# define SIGINFO SIGUSR1
#అయితే_ముగింపు

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#నిర్వచితం_కాకపోతే O_CIO
# define O_CIO 0
#అయితే_ముగింపు

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#నిర్వచనం_రద్దు O_NOCACHE

#నిర్వచించు output_char(c)				\
  చేయి						\
    {						\
      obuf[oc++] = (c);				\
      ఒకవేళ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  అయితే (0)

/* Default input and output blocksize. */
#నిర్వచించు DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
గణన
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_ఖండం = 010,
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
గణన
  {
    స్థితి_NONE = 1,
    స్థితి_NOXFER = 2,
    స్థితి_DEFAULT = 3,
    స్థితి_పురోగతి = 4
  };

/* The name of the input file, or NULL for the standard input. */
స్థిర అక్షరం స్థిరరాశి *ఆదానం_file = ఖాళీ;

/* The name of the output file, or NULL for the standard output. */
స్థిర అక్షరం స్థిరరాశి *output_file = ఖాళీ;

/* The page size on this host.  */
స్థిర idx_t పుట_కొలత;

/* The number of bytes in which atomic reads are done. */
స్థిర idx_t ఆదానం_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
స్థిర idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
స్థిర idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
స్థిర intmax_t skip_రికార్డులు = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
స్థిర idx_t skip_బైట్లు = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
స్థిర intmax_t seek_రికార్డులు = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
స్థిర intmax_t seek_బైట్లు = 0;

/* Whether the final output was done with a seek (rather than a write).  */
స్థిర bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
స్థిర intmax_t max_రికార్డులు = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
స్థిర idx_t max_బైట్లు = 0;

/* Bit vector of conversions to apply. */
స్థిర పూర్ణాంకం conversions_ముసుగు = 0;

/* Open flags for the input and output files.  */
స్థిర పూర్ణాంకం ఆదానం_flags = 0;
స్థిర పూర్ణాంకం output_flags = 0;

/* Status flags for what is printed to stderr.  */
స్థిర పూర్ణాంకం స్థితి_level = స్థితి_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
స్థిర bool translation_needed = false;

/* Number of partial blocks written. */
స్థిర intmax_t w_partial = 0;

/* Number of full blocks written. */
స్థిర intmax_t w_నిండిన = 0;

/* Number of partial blocks read. */
స్థిర intmax_t r_partial = 0;

/* Number of full blocks read. */
స్థిర intmax_t r_నిండిన = 0;

/* Number of bytes written.  */
స్థిర intmax_t w_బైట్లు = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
స్థిర intmax_t reported_w_బైట్లు = -1;

/* Time that dd started.  */
స్థిర xtime_t ప్రారంభం_సమయం;

/* Next time to report periodic progress.  */
స్థిర xtime_t తదుపరి_సమయం;

/* If positive, the number of bytes output in the current progress line.  */
స్థిర పూర్ణాంకం పురోగతి_len;

/* True if input is seekable.  */
స్థిర bool ఆదానం_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
స్థిర పూర్ణాంకం ఆదానం_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
స్థిర off_t ఆదానం_స్థానభ్రంశం;

/* True if a partial read should be diagnosed.  */
స్థిర bool warn_partial_read;

/* Records truncated by conv=block. */
స్థిర intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
స్థిర అక్షరం newline_character = '\n';
స్థిర అక్షరం space_character = ' ';

/* I/O buffers.  */
స్థిర అక్షరం *ibuf;
స్థిర అక్షరం *obuf;

/* Current index into 'obuf'. */
స్థిర idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
స్థిర idx_t నిలువు = 0;

/* The set of signals that are caught.  */
స్థిర sigset_t caught_సంకేతాలు;

/* If nonzero, the value of the pending fatal signal.  */
స్థిర sig_atomic_t అస్థిర అంతరాయం_signal;

/* A count of the number of pending info signals that have been received.  */
స్థిర sig_atomic_t అస్థిర info_signal_లెక్క;

/* Whether to discard cache for input or output.  */
స్థిర bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
స్థిర bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
స్థిర ssize_t (*iread_fnc) (పూర్ణాంకం fd, అక్షరం *buf, idx_t కొలత);

/* A longest symbol in the struct symbol_values tables below.  */
#నిర్వచించు LONGEST_చిహ్నం "count_bytes"

/* A symbol and the corresponding integer value.  */
నిర్మాణం చిహ్నం_విలువ
{
  అక్షరం చిహ్నం[పరిమాణం LONGEST_చిహ్నం];
  పూర్ణాంకం విలువ;
};

/* Conversion symbols, for conv="...".  */
స్థిర నిర్మాణం చిహ్నం_విలువ స్థిరరాశి conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_ఖండం | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_ఖండం | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_ఖండం | C_TWOBUFS},	/* Variable to fixed length records. */
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

#నిర్వచించు FFS_ముసుగు(x) ((x) ^ ((x) & ((x) - 1)))
గణన
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ద్విమాన
          | O_CIO
          | O_ప్రత్యక్ష
          | O_DIRECTORY
          | O_DSYNC
          | O_EXCL
          | O_NOATIME
          | O_NOCTTY
          | O_NOFOLLOW
          | O_NOLINKS
          | O_NONBLOCK
          | O_SYNC
          | O_పాఠ్యం
          ),

    /* Use its lowest bits for private flags.  */
    O_FULLBLOCK = FFS_ముసుగు (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_ముసుగు (v2),
    v3 = v2 ^ O_NOCACHE,

    O_లెక్క_బైట్లు = FFS_ముసుగు (v3),
    v4 = v3 ^ O_లెక్క_బైట్లు,

    O_SKIP_బైట్లు = FFS_ముసుగు (v4),
    v5 = v4 ^ O_SKIP_బైట్లు,

    O_SEEK_బైట్లు = FFS_ముసుగు (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_లెక్క_బైట్లు != 0);
static_assert (O_SKIP_బైట్లు != 0);
static_assert (O_SEEK_బైట్లు != 0);

#నిర్వచించు గుణిజం_బిట్లు_సమితి(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! గుణిజం_బిట్లు_సమితి (O_FULLBLOCK));
static_assert ( ! గుణిజం_బిట్లు_సమితి (O_NOCACHE));
static_assert ( ! గుణిజం_బిట్లు_సమితి (O_లెక్క_బైట్లు));
static_assert ( ! గుణిజం_బిట్లు_సమితి (O_SKIP_బైట్లు));
static_assert ( ! గుణిజం_బిట్లు_సమితి (O_SEEK_బైట్లు));

/* Flags, for iflag="..." and oflag="...".  */
స్థిర నిర్మాణం చిహ్నం_విలువ స్థిరరాశి flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ద్విమాన},
  {"cio",	  O_CIO},
  {"direct",	  O_ప్రత్యక్ష},
  {"directory",   O_DIRECTORY},
  {"dsync",	  O_DSYNC},
  {"noatime",	  O_NOATIME},
  {"nocache",	  O_NOCACHE},   /* Discard cache.  */
  {"noctty",	  O_NOCTTY},
  {"nofollow",	  HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0},
  {"nolinks",	  O_NOLINKS},
  {"nonblock",	  O_NONBLOCK},
  {"sync",	  O_SYNC},
  {"text",	  O_పాఠ్యం},
  {"fullblock",   O_FULLBLOCK}, /* Accumulate full blocks from input.  */
  {"count_bytes", O_లెక్క_బైట్లు},
  {"skip_bytes",  O_SKIP_బైట్లు},
  {"seek_bytes",  O_SEEK_బైట్లు},
  {"",		0}
};

/* Status, for status="...".  */
స్థిర నిర్మాణం చిహ్నం_విలువ స్థిరరాశి statuses[] =
{
  {"none",	స్థితి_NONE},
  {"noxfer",	స్థితి_NOXFER},
  {"progress",	స్థితి_పురోగతి},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
స్థిర గుర్తులేని అక్షరం trans_పట్టిక[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

స్థిర అక్షరం స్థిరరాశి ascii_గమ్యం_ebcdic[] =
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

స్థిర అక్షరం స్థిరరాశి ascii_గమ్యం_ibm[] =
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

స్థిర అక్షరం స్థిరరాశి ebcdic_గమ్యం_ascii[] =
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
స్థిర bool close_stdout_required = true;

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
స్థిర శూన్య
maybe_close_stdout (శూన్య)
{
  ఒకవేళ (close_stdout_required)
    close_stdout ();
  లేకపోతే ఒకవేళ (close_ప్రవాహం (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
స్థిర శూన్య
diagnose (పూర్ణాంకం errnum, అక్షరం స్థిరరాశి *fmt, ...)
{
  ఒకవేళ (0 < పురోగతి_len)
    {
      fputc ('\n', stderr);
      పురోగతి_len = 0;
    }

  va_list ap;
  va_ప్రారంభం (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_చివర (ap);
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [నిర్వాహ్యం]...\n\
  or:  %s OPTION\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Copy a file, converting and formatting according గమ్యం the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=బైట్లు_2        పఠ and లిఖ పైకి గమ్యం బైట్లు_2 బైట్లు at a సమయం (అప్రమేయ: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=బైట్లు_2       convert బైట్లు_2 బైట్లు at a సమయం\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as చొప్పున the comma separated చిహ్నం list\n\
"));
      oputs (_("\
  లెక్క=N         copy only N ఆదానం ఖండాలు\n\
"));
      oputs (_("\
  ibs=బైట్లు_2       పఠ పైకి గమ్యం బైట్లు_2 బైట్లు at a సమయం (అప్రమేయ: 512)\n\
"));
      oputs (_("\
  ఒకవేళ=FILE         పఠ from FILE instead యొక్క standard ఆదానం\n\
"));
      oputs (_("\
  iflag=FLAGS     పఠ as చొప్పున the comma separated చిహ్నం list\n\
"));
      oputs (_("\
  obs=బైట్లు_2       లిఖ బైట్లు_2 బైట్లు at a సమయం (అప్రమేయ: 512)\n\
"));
      oputs (_("\
  యొక్క=FILE         లిఖ గమ్యం FILE instead యొక్క standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     లిఖ as చొప్పున the comma separated చిహ్నం list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output ఖండాలు\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized ఆదానం ఖండాలు\n\
"));
      oputs (_("\
  స్థితి_2=LEVEL    The LEVEL యొక్క information గమ్యం print గమ్యం standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and బైట్లు_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on కొరకు T, P, E, Z, Y, R, Q.\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
If N చివర్లు లోపల 'B', it counts బైట్లు not ఖండాలు.\n\
\n\
Each CONV చిహ్నం may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC గమ్యం ASCII\n\
  ebcdic    from ASCII గమ్యం EBCDIC\n\
  ibm       from ASCII గమ్యం alternate EBCDIC\n\
  ఖండం     pad newline-terminated రికార్డులు with spaces గమ్యం cbs-కొలత\n\
  unblock   replace trailing spaces లోపల cbs-కొలత రికార్డులు with newline\n\
  lcase     change పై స్థితి గమ్యం క్రింది స్థితి\n\
  ucase     change క్రింది స్థితి గమ్యం పై స్థితి\n\
  sparse    try గమ్యం seek rather than లిఖ all-NUL output ఖండాలు\n\
  swab      మార్పిడి every pair యొక్క ఆదానం బైట్లు\n\
  sync      pad every ఆదానం ఖండం with NULs గమ్యం ibs-కొలత; when వాడిన\n\
            with ఖండం or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail ఒకవేళ the output file already exists\n\
  nocreat   చేయి not create the output file\n\
  notrunc   చేయి not truncate the output file\n\
  noerror   కొనసాగు after పఠ errors\n\
  fdatasync  physically లిఖ output file దత్తాంశం before finishing\n\
  fsync     likewise, but also లిఖ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG చిహ్నం may be:\n\
\n\
  append    append బహుళకం_2 (makes sense only కొరకు output; conv=notrunc suggested)\n\
"), stdout);
      ఒకవేళ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      ఒకవేళ (O_ప్రత్యక్ష)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      ఒకవేళ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      ఒకవేళ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      ఒకవేళ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      ఒకవేళ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#ఒకవేళ HAVE_POSIX_FADVISE
      ఒకవేళ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#అయితే_ముగింపు
      ఒకవేళ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      ఒకవేళ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      ఒకవేళ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      ఒకవేళ (O_ద్విమాన)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      ఒకవేళ (O_పాఠ్యం)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        రాయి (_("\
\n\
Sending a %s సంకేతం గమ్యం a running 'dd' process makes it\n\
print I/O statistics గమ్యం standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Common options to use when displaying sizes and rates.  */

గణన { human_opts = (human_autoscale | human_విడత_గమ్యం_సమీప
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

స్థిర శూన్య
alloc_ibuf (శూన్య)
{
  ఒకవేళ (ibuf)
    తిరిగి;

  bool extra_byte_for_swab = !!(conversions_ముసుగు & C_SWAB);
  ibuf = alignalloc (పుట_కొలత, ఆదానం_blocksize + extra_byte_for_swab);
  ఒకవేళ (!ibuf)
    {
      అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             ఆదానం_blocksize,
             human_readable (ఆదానం_blocksize, hbuf,
                             human_opts | human_పునాది_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

స్థిర శూన్య
alloc_obuf (శూన్య)
{
  ఒకవేళ (obuf)
    తిరిగి;

  ఒకవేళ (conversions_ముసుగు & C_TWOBUFS)
    {
      obuf = alignalloc (పుట_కొలత, output_blocksize);
      ఒకవేళ (!obuf)
        {
          అక్షరం hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_పునాది_1024, 1, 1));
        }
    }
  లేకపోతే
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

స్థిర శూన్య
translate_charset (అక్షరం స్థిరరాశి *new_trans)
{
  కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
    trans_పట్టిక[i] = new_trans[trans_పట్టిక[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

స్థిర అంతఃవరుస bool
గుణిజం_బిట్లు_సమితి_2 (పూర్ణాంకం i)
{
  తిరిగి గుణిజం_బిట్లు_సమితి (i);
}

స్థిర bool
abbreviation_lacks_ఉపసర్గ (అక్షరం స్థిరరాశి *సందేశం_2)
{
  తిరిగి సందేశం_2[అక్షరశ్రేణి_పొడవు (సందేశం_2) - 2] == ' ';
}

/* Print transfer statistics.  */

స్థిర శూన్య
print_xfer_stats (xtime_t పురోగతి_సమయం)
{
  xtime_t now = పురోగతి_సమయం ? పురోగతి_సమయం : gethrxtime ();
  స్థిర అక్షరం స్థిరరాశి slash_s[] = "/s";
  అక్షరం hbuf[3][LONGEST_HUMAN_READABLE + పరిమాణం slash_s];
  ద్విగుణ delta_s;
  అక్షరం స్థిరరాశి *బైట్లు_చొప్పున_రెండవ;
  అక్షరం స్థిరరాశి *si = human_readable (w_బైట్లు, hbuf[0], human_opts, 1, 1);
  అక్షరం స్థిరరాశి *iec = human_readable (w_బైట్లు, hbuf[1],
                                    human_opts | human_పునాది_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  అక్షరం *bpsbuf = hbuf[2];
  పూర్ణాంకం bpsbufsize = పరిమాణం hbuf[2];
  ఒకవేళ (ప్రారంభం_సమయం < now)
    {
      ద్విగుణ XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - ప్రారంభం_సమయం;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      బైట్లు_చొప్పున_రెండవ = human_readable (w_బైట్లు, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (బైట్లు_చొప్పున_రెండవ - bpsbuf + bpsbuf, slash_s);
    }
  లేకపోతే
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      బైట్లు_చొప్పున_రెండవ = bpsbuf;
    }

  ఒకవేళ (పురోగతి_సమయం)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  అక్షరం delta_s_buf[24];
  snprintf (delta_s_buf, పరిమాణం delta_s_buf,
            పురోగతి_సమయం ? "%.0f s" : "%g s", delta_s);

  పూర్ణాంకం stats_len
    = (abbreviation_lacks_ఉపసర్గ (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_బైట్లు)),
                  w_బైట్లు, delta_s_buf, బైట్లు_చొప్పున_రెండవ)
       : abbreviation_lacks_ఉపసర్గ (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_బైట్లు, si, delta_s_buf, బైట్లు_చొప్పున_రెండవ)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_బైట్లు, si, iec, delta_s_buf, బైట్లు_చొప్పున_రెండవ));

  ఒకవేళ (పురోగతి_సమయం)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      ఒకవేళ (0 <= stats_len && stats_len < పురోగతి_len)
        fprintf (stderr, "%*s", పురోగతి_len - stats_len, "");
      పురోగతి_len = stats_len;
    }
  లేకపోతే
    fputc ('\n', stderr);

  reported_w_బైట్లు = w_బైట్లు;
}

స్థిర శూన్య
print_stats (శూన్య)
{
  ఒకవేళ (స్థితి_level == స్థితి_NONE)
    తిరిగి;

  ఒకవేళ (0 < పురోగతి_len)
    {
      fputc ('\n', stderr);
      పురోగతి_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_నిండిన, r_partial, w_నిండిన, w_partial);

  ఒకవేళ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  ఒకవేళ (స్థితి_level == స్థితి_NOXFER)
    తిరిగి;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

స్థిర శూన్య
అంతరాయం_handler (పూర్ణాంకం sig)
{
  ఒకవేళ (! SA_RESETHAND)
    సంకేతం (sig, SIG_DFL);
  అంతరాయం_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

స్థిర శూన్య
siginfo_handler (MAYBE_వాడని పూర్ణాంకం sig)
{
  info_signal_లెక్క++;
}

/* Install the signal handlers.  */

స్థిర శూన్య
install_signal_handlers (శూన్య)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  నిర్మాణం sigaction act;
  sigemptyset (&caught_సంకేతాలు);
  ఒకవేళ (catch_siginfo)
    sigaddset (&caught_సంకేతాలు, SIGINFO);
  sigaction (SIGINT, ఖాళీ, &act);
  ఒకవేళ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_సంకేతాలు, SIGINT);
  act.sa_mask = caught_సంకేతాలు;

  ఒకవేళ (sigismember (&caught_సంకేతాలు, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ఖాళీ);
    }

  ఒకవేళ (sigismember (&caught_సంకేతాలు, SIGINT))
    {
      act.sa_handler = అంతరాయం_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ఖాళీ);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
స్థిర పూర్ణాంకం
iclose (పూర్ణాంకం fd)
{
  ఒకవేళ (close (fd) != 0)
    చేయి
      ఒకవేళ (errno != EINTR)
        తిరిగి -1;
    అయితే (close (fd) != 0 && errno != EBADF);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం synchronize_output (శూన్య);

స్థిర శూన్య
cleanup (శూన్య)
{
  ఒకవేళ (!అంతరాయం_signal)
    {
      పూర్ణాంకం sync_స్థితి = synchronize_output ();
      ఒకవేళ (sync_స్థితి)
        నిష్క్రమించు (sync_స్థితి);
    }

  ఒకవేళ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (ఆదానం_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  ఒకవేళ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

స్థిర శూన్య
process_సంకేతాలు (శూన్య)
{
  అయితే (అంతరాయం_signal || info_signal_లెక్క)
    {
      పూర్ణాంకం అంతరాయం;
      పూర్ణాంకం infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_సంకేతాలు, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      అంతరాయం = అంతరాయం_signal;
      infos = info_signal_లెక్క;

      ఒకవేళ (infos)
        info_signal_లెక్క = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ఖాళీ);

      ఒకవేళ (అంతరాయం)
        cleanup ();
      print_stats ();
      ఒకవేళ (అంతరాయం)
        లేపు (అంతరాయం);
    }
}

స్థిర శూన్య
ముగింపు_పైకి (శూన్య)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_సంకేతాలు ();
  cleanup ();
  print_stats ();
}

స్థిర శూన్య
quit (పూర్ణాంకం సంకేతం_2)
{
  ముగింపు_పైకి ();
  నిష్క్రమించు (సంకేతం_2);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

స్థిర off_t
cache_విడత (పూర్ణాంకం fd, off_t len)
{
  స్థిర off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  ఒకవేళ (len)
    {
      intmax_t c_pending;
      ఒకవేళ (ckd_కూడు (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      ఒకవేళ (c_pending > *pending)
        len = c_pending - *pending;
      లేకపోతే
        len = 0;
    }
  లేకపోతే
    len = *pending;

  తిరిగి len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

స్థిర bool
invalidate_cache (పూర్ణాంకం fd, off_t len)
{
  పూర్ణాంకం adv_ret = -1;
  off_t స్థానభ్రంశం;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_విడత (fd, len);
  ఒకవేళ (len && !clen)
    తిరిగి true; /* Don't advise this time.  */
  లేకపోతే ఒకవేళ (! len && ! clen && ! nocache_eof)
    తిరిగి true;
  off_t pending = len ? cache_విడత (fd, 0) : 0;

  ఒకవేళ (fd == STDIN_FILENO)
    {
      ఒకవేళ (ఆదానం_seekable)
        స్థానభ్రంశం = ఆదానం_స్థానభ్రంశం;
      లేకపోతే
        {
          స్థానభ్రంశం = -1;
          errno = ESPIPE;
        }
    }
  లేకపోతే
    {
      స్థిర off_t output_స్థానభ్రంశం = -2;

      ఒకవేళ (output_స్థానభ్రంశం != -1)
        {
          ఒకవేళ (output_స్థానభ్రంశం < 0)
            output_స్థానభ్రంశం = lseek (fd, 0, SEEK_CUR);
          లేకపోతే ఒకవేళ (len)
            output_స్థానభ్రంశం += clen + pending;
        }

      స్థానభ్రంశం = output_స్థానభ్రంశం;
    }

  ఒకవేళ (0 <= స్థానభ్రంశం)
   {
     ఒకవేళ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#ఒకవేళ HAVE_POSIX_FADVISE
     స్థానభ్రంశం = స్థానభ్రంశం - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     ఒకవేళ (clen == 0)
       స్థానభ్రంశం -= స్థానభ్రంశం % పుట_కొలత;
     adv_ret = posix_fadvise (fd, స్థానభ్రంశం, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#లేకపోతే
     errno = ENOTSUP;
#అయితే_ముగింపు
   }

  తిరిగి adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

స్థిర ssize_t
iread (పూర్ణాంకం fd, అక్షరం *buf, idx_t కొలత)
{
  ssize_t nread;
  స్థిర ssize_t prev_nread;

  చేయి
    {
      process_సంకేతాలు ();
      nread = పఠ (fd, buf, కొలత);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      ఒకవేళ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < కొలత
          && (ఆదానం_flags & O_ప్రత్యక్ష))
        {
          errno = 0;
          nread = 0;
        }
    }
  అయితే (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  ఒకవేళ (0 < nread && nread < కొలత)
    process_సంకేతాలు ();

  ఒకవేళ (0 < nread && warn_partial_read)
    {
      ఒకవేళ (0 < prev_nread && prev_nread < కొలత)
        {
          idx_t prev = prev_nread;
          ఒకవేళ (స్థితి_level != స్థితి_NONE)
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
  తిరిగి nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
స్థిర ssize_t
iread_fullblock (పూర్ణాంకం fd, అక్షరం *buf, idx_t కొలత)
{
  ssize_t nread = 0;

  అయితే (0 < కొలత)
    {
      ssize_t ncurr = iread (fd, buf, కొలత);
      ఒకవేళ (ncurr < 0)
        తిరిగి ncurr;
      ఒకవేళ (ncurr == 0)
        ఆపు;
      nread += ncurr;
      buf   += ncurr;
      కొలత  -= ncurr;
    }

  తిరిగి nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

స్థిర idx_t
iwrite (పూర్ణాంకం fd, అక్షరం స్థిరరాశి *buf, idx_t కొలత)
{
  idx_t మొత్తం_written = 0;

  ఒకవేళ ((output_flags & O_ప్రత్యక్ష) && కొలత < output_blocksize)
    {
      పూర్ణాంకం old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      ఒకవేళ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_ప్రత్యక్ష) != 0
          && స్థితి_level != స్థితి_NONE)
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
      conversions_ముసుగు |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  అయితే (మొత్తం_written < కొలత)
    {
      ssize_t nwritten = 0;
      process_సంకేతాలు ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      ఒకవేళ ((conversions_ముసుగు & C_SPARSE) && is_nul (buf, కొలత))
        {
          ఒకవేళ (lseek (fd, కొలత, SEEK_CUR) < 0)
            {
              conversions_ముసుగు &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          లేకపోతే
            {
              final_op_was_seek = true;
              nwritten = కొలత;
            }
        }

      ఒకవేళ (!nwritten)
        nwritten = లిఖ (fd, buf + మొత్తం_written, కొలత - మొత్తం_written);

      ఒకవేళ (nwritten < 0)
        {
          ఒకవేళ (errno != EINTR)
            ఆపు;
        }
      లేకపోతే ఒకవేళ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          ఆపు;
        }
      లేకపోతే
        మొత్తం_written += nwritten;
    }

  ఒకవేళ (o_nocache && మొత్తం_written)
    invalidate_cache (fd, మొత్తం_written);

  తిరిగి మొత్తం_written;
}

/* Write, then empty, the output buffer 'obuf'. */

స్థిర శూన్య
write_output (శూన్య)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_బైట్లు += nwritten;
  ఒకవేళ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      ఒకవేళ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  లేకపోతే
    w_నిండిన++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

స్థిర పూర్ణాంకం
ifdatasync (పూర్ణాంకం fd)
{
  పూర్ణాంకం ret;

  చేయి
    {
      process_సంకేతాలు ();
      ret = fdatasync (fd);
    }
  అయితే (ret < 0 && errno == EINTR);

  తిరిగి ret;
}

/* Restart on EINTR from fd_reopen.  */

స్థిర పూర్ణాంకం
ifd_reopen (పూర్ణాంకం desired_fd, అక్షరం స్థిరరాశి *file, పూర్ణాంకం flag, mode_t బహుళకం_2)
{
  పూర్ణాంకం ret;

  చేయి
    {
      process_సంకేతాలు ();
      ret = fd_reopen (desired_fd, file, flag, బహుళకం_2);
    }
  అయితే (ret < 0 && errno == EINTR);

  తిరిగి ret;
}

/* Restart on EINTR from fstat.  */

స్థిర పూర్ణాంకం
ifstat (పూర్ణాంకం fd, నిర్మాణం stat *st)
{
  పూర్ణాంకం ret;

  చేయి
    {
      process_సంకేతాలు ();
      ret = fstat (fd, st);
    }
  అయితే (ret < 0 && errno == EINTR);

  తిరిగి ret;
}

/* Restart on EINTR from fsync.  */

స్థిర పూర్ణాంకం
ifsync (పూర్ణాంకం fd)
{
  పూర్ణాంకం ret;

  చేయి
    {
      process_సంకేతాలు ();
      ret = fsync (fd);
    }
  అయితే (ret < 0 && errno == EINTR);

  తిరిగి ret;
}

/* Restart on EINTR from ftruncate.  */

స్థిర పూర్ణాంకం
iftruncate (పూర్ణాంకం fd, off_t నిడివి)
{
  పూర్ణాంకం ret;

  చేయి
    {
      process_సంకేతాలు ();
      ret = ftruncate (fd, నిడివి);
    }
  అయితే (ret < 0 && errno == EINTR);

  తిరిగి ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_శుద్ధ
స్థిర bool
నిర్వాహ్యం_matches (అక్షరం స్థిరరాశి *str, అక్షరం స్థిరరాశి *నమూనా_2, అక్షరం delim)
{
  అయితే (*నమూనా_2)
    ఒకవేళ (*str++ != *నమూనా_2++)
      తిరిగి false;
  తిరిగి !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

స్థిర పూర్ణాంకం
parse_symbols (అక్షరం స్థిరరాశి *str, నిర్మాణం చిహ్నం_విలువ స్థిరరాశి *పట్టిక,
               bool exclusive, అక్షరం స్థిరరాశి *error_msgid)
{
  పూర్ణాంకం విలువ = 0;

  అయితే (true)
    {
      అక్షరం స్థిరరాశి *strcomma = strchr (str, ',');
      నిర్మాణం చిహ్నం_విలువ స్థిరరాశి *నమోదు_2;

      కొరకు (నమోదు_2 = పట్టిక;
           ! (నిర్వాహ్యం_matches (str, నమోదు_2->చిహ్నం, ',') && నమోదు_2->విలువ);
           నమోదు_2++)
        {
          ఒకవేళ (! నమోదు_2->చిహ్నం[0])
            {
              idx_t slen = strcomma ? strcomma - str : అక్షరశ్రేణి_పొడవు (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      ఒకవేళ (exclusive)
        విలువ = నమోదు_2->విలువ;
      లేకపోతే
        విలువ |= నమోదు_2->విలువ;
      ఒకవేళ (!strcomma)
        ఆపు;
      str = strcomma + 1;
    }

  తిరిగి విలువ;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

స్థిర intmax_t
parse_integer (అక్షరం స్థిరరాశి *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  పూర్ణాంకం indeterminate = 0;
  స్థిర అక్షరం స్థిరరాశి suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t ఫలితం = 1;
  bool అధికప్రవాహం = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  అయితే (true)
    {
      uintmax_t n = indeterminate;
      అక్షరం *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      ఒకవేళ ((e & ~LONGINT_అధికప్రవాహం) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          ఒకవేళ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_అధికప్రవాహం) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      ఒకవేళ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      ఒకవేళ ((e & ~LONGINT_అధికప్రవాహం) != LONGINT_OK)
        {
          ఒకవేళ (! (e & LONGINT_అధికప్రవాహం) && n <= INTMAX_MAX)
            {
              *invalid = e;
              తిరిగి indeterminate;
            }
          e = LONGINT_అధికప్రవాహం;
        }

      ఒకవేళ (n == 0)
        ఫలితం = 0;
      లేకపోతే ఒకవేళ (ఫలితం != 0)
        {
          intmax_t లబ్ధం;
          ఒకవేళ ((e & LONGINT_అధికప్రవాహం)
              || అధికప్రవాహం
              || ckd_mul (&లబ్ధం, ఫలితం, n))
            అధికప్రవాహం = true;
          లేకపోతే
            ఫలితం = లబ్ధం;
        }

      ఒకవేళ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      ఒకవేళ (! multiply)
        ఆపు;
      str = suffix + 1;
    }

  ఒకవేళ (ఫలితం == 0)
    {
      ఒకవేళ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  లేకపోతే ఒకవేళ (అధికప్రవాహం)
    {
      e = LONGINT_అధికప్రవాహం;
      ఫలితం = INTMAX_MAX;
    }

  *invalid = e;
  తిరిగి ఫలితం;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_శుద్ధ
స్థిర bool
నిర్వాహ్యం_is (అక్షరం స్థిరరాశి *నిర్వాహ్యం_2, అక్షరం స్థిరరాశి *name)
{
  తిరిగి నిర్వాహ్యం_matches (నిర్వాహ్యం_2, name, '=');
}

స్థిర శూన్య
scanargs (పూర్ణాంకం argc, అక్షరం *స్థిరరాశి *argv)
{
  idx_t blocksize = 0;
  intmax_t లెక్క = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool లెక్క_B = false, skip_B = false, seek_B = false;

  కొరకు (పూర్ణాంకం i = optind; i < argc; i++)
    {
      అక్షరం స్థిరరాశి *name = argv[i];
      అక్షరం స్థిరరాశి *val = strchr (name, '=');

      ఒకవేళ (val == ఖాళీ)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      ఒకవేళ (నిర్వాహ్యం_is (name, "if"))
        ఆదానం_file = val;
      లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "of"))
        output_file = val;
      లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "conv"))
        conversions_ముసుగు |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "iflag"))
        ఆదానం_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "status"))
        స్థితి_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      లేకపోతే
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ఖాళీ;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          ఒకవేళ (నిర్వాహ్యం_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &ఆదానం_blocksize;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "skip") || నిర్వాహ్యం_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          లేకపోతే ఒకవేళ (నిర్వాహ్యం_is (name, "count"))
            {
              లెక్క = n;
              లెక్క_B = has_B;
            }
          లేకపోతే
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          ఒకవేళ (n < n_min)
            invalid = LONGINT_INVALID;
          లేకపోతే ఒకవేళ (n_max < n)
            invalid = LONGINT_అధికప్రవాహం;

          ఒకవేళ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_అధికప్రవాహం ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          లేకపోతే ఒకవేళ (converted_idx)
            *converted_idx = n;
        }
    }

  ఒకవేళ (blocksize)
    ఆదానం_blocksize = output_blocksize = blocksize;
  లేకపోతే
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_ముసుగు |= C_TWOBUFS;
    }

  ఒకవేళ (ఆదానం_blocksize == 0)
    ఆదానం_blocksize = DEFAULT_BLOCKSIZE;
  ఒకవేళ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  ఒకవేళ (conversion_blocksize == 0)
    conversions_ముసుగు &= ~(C_ఖండం | C_UNBLOCK);

  ఒకవేళ (ఆదానం_flags & (O_DSYNC | O_SYNC))
    ఆదానం_flags |= O_RSYNC;

  ఒకవేళ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (skip_B)
    ఆదానం_flags |= O_SKIP_బైట్లు;
  ఒకవేళ (ఆదానం_flags & O_SKIP_బైట్లు && skip != 0)
    {
      skip_రికార్డులు = skip / ఆదానం_blocksize;
      skip_బైట్లు = skip % ఆదానం_blocksize;
    }
  లేకపోతే ఒకవేళ (skip != 0)
    skip_రికార్డులు = skip;

  ఒకవేళ (లెక్క_B)
    ఆదానం_flags |= O_లెక్క_బైట్లు;
  ఒకవేళ (ఆదానం_flags & O_లెక్క_బైట్లు && లెక్క != INTMAX_MAX)
    {
      max_రికార్డులు = లెక్క / ఆదానం_blocksize;
      max_బైట్లు = లెక్క % ఆదానం_blocksize;
    }
  లేకపోతే ఒకవేళ (లెక్క != INTMAX_MAX)
    max_రికార్డులు = లెక్క;

  ఒకవేళ (seek_B)
    output_flags |= O_SEEK_బైట్లు;
  ఒకవేళ (output_flags & O_SEEK_బైట్లు && seek != 0)
    {
      seek_రికార్డులు = seek / output_blocksize;
      seek_బైట్లు = seek % output_blocksize;
    }
  లేకపోతే ఒకవేళ (seek != 0)
    seek_రికార్డులు = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_ముసుగు & C_TWOBUFS) && ! (ఆదానం_flags & O_FULLBLOCK)
     && (skip_రికార్డులు
         || (0 < max_రికార్డులు && max_రికార్డులు < INTMAX_MAX)
         || (ఆదానం_flags | output_flags) & O_ప్రత్యక్ష));

  iread_fnc = ((ఆదానం_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  ఆదానం_flags &= ~O_FULLBLOCK;

  ఒకవేళ (గుణిజం_బిట్లు_సమితి_2 (conversions_ముసుగు & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  ఒకవేళ (గుణిజం_బిట్లు_సమితి_2 (conversions_ముసుగు & (C_ఖండం | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  ఒకవేళ (గుణిజం_బిట్లు_సమితి_2 (conversions_ముసుగు & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  ఒకవేళ (గుణిజం_బిట్లు_సమితి_2 (conversions_ముసుగు & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  ఒకవేళ (గుణిజం_బిట్లు_సమితి_2 (ఆదానం_flags & (O_ప్రత్యక్ష | O_NOCACHE))
      || గుణిజం_బిట్లు_సమితి_2 (output_flags & (O_ప్రత్యక్ష | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  ఒకవేళ (ఆదానం_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_రికార్డులు == 0 && max_బైట్లు == 0);
      ఆదానం_flags &= ~O_NOCACHE;
    }
  ఒకవేళ (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_రికార్డులు == 0 && max_బైట్లు == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

స్థిర శూన్య
apply_translations (శూన్య)
{
  ఒకవేళ (conversions_ముసుగు & C_ASCII)
    translate_charset (ebcdic_గమ్యం_ascii);

  ఒకవేళ (conversions_ముసుగు & C_UCASE)
    {
      కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
        trans_పట్టిక[i] = పెద్ద_అక్షరం (trans_పట్టిక[i]);
      translation_needed = true;
    }
  లేకపోతే ఒకవేళ (conversions_ముసుగు & C_LCASE)
    {
      కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
        trans_పట్టిక[i] = tolower (trans_పట్టిక[i]);
      translation_needed = true;
    }

  ఒకవేళ (conversions_ముసుగు & C_EBCDIC)
    {
      translate_charset (ascii_గమ్యం_ebcdic);
      newline_character = ascii_గమ్యం_ebcdic['\n'];
      space_character = ascii_గమ్యం_ebcdic[' '];
    }
  లేకపోతే ఒకవేళ (conversions_ముసుగు & C_IBM)
    {
      translate_charset (ascii_గమ్యం_ibm);
      newline_character = ascii_గమ్యం_ibm['\n'];
      space_character = ascii_గమ్యం_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

స్థిర శూన్య
translate_బఫరు (అక్షరం *buf, idx_t nread)
{
  అక్షరం *cp = buf;
  కొరకు (idx_t i = nread; i; i--, cp++)
    *cp = trans_పట్టిక[గమ్యం_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

స్థిర అక్షరం *
swab_బఫరు (అక్షరం *buf, idx_t *nread, పూర్ణాంకం *saved_byte)
{
  ఒకవేళ (*nread == 0)
    తిరిగి buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  పూర్ణాంకం prev_saved = *saved_byte;
  ఒకవేళ ((prev_saved < 0) == (*nread & 1))
    {
      గుర్తులేని అక్షరం c = buf[--*nread];
      *saved_byte = c;
    }
  లేకపోతే
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  కొరకు (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  ఒకవేళ (prev_saved < 0)
    తిరిగి buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  తిరిగి buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

స్థిర శూన్య
advance_ఆదానం_స్థానభ్రంశం (intmax_t స్థానభ్రంశం)
{
  ఒకవేళ (0 <= ఆదానం_స్థానభ్రంశం && ckd_కూడు (&ఆదానం_స్థానభ్రంశం, ఆదానం_స్థానభ్రంశం, స్థానభ్రంశం))
    ఆదానం_స్థానభ్రంశం = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

స్థిర intmax_t
skip (పూర్ణాంకం fdesc, అక్షరం స్థిరరాశి *file, intmax_t రికార్డులు, idx_t blocksize,
      idx_t *బైట్లు)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t స్థానభ్రంశం;
  ఒకవేళ (! ckd_mul (&స్థానభ్రంశం, రికార్డులు, blocksize)
      && ! ckd_కూడు (&స్థానభ్రంశం, స్థానభ్రంశం, *బైట్లు)
      && 0 <= lseek (fdesc, స్థానభ్రంశం, SEEK_CUR))
    {
      ఒకవేళ (fdesc == STDIN_FILENO)
        {
           నిర్మాణం stat st;
           ఒకవేళ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           ఒకవేళ (usable_st_కొలత (&st) && 0 < st.st_కొలత && 0 <= ఆదానం_స్థానభ్రంశం
               && st.st_కొలత - ఆదానం_స్థానభ్రంశం < స్థానభ్రంశం)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               రికార్డులు = ( స్థానభ్రంశం - st.st_కొలత ) / blocksize;
               స్థానభ్రంశం = st.st_కొలత - ఆదానం_స్థానభ్రంశం;
             }
           లేకపోతే
             రికార్డులు = 0;
           advance_ఆదానం_స్థానభ్రంశం (స్థానభ్రంశం);
        }
      లేకపోతే
        {
          రికార్డులు = 0;
          *బైట్లు = 0;
        }
      తిరిగి రికార్డులు;
    }
  లేకపోతే
    {
      పూర్ణాంకం lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      ఒకవేళ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          ఒకవేళ (!lseek_errno)
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

      అక్షరం *buf;
      ఒకవేళ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      లేకపోతే
        {
          alloc_obuf ();
          buf = obuf;
        }

      చేయి
        {
          ssize_t nread = iread_fnc (fdesc, buf, రికార్డులు ? blocksize : *బైట్లు);
          ఒకవేళ (nread < 0)
            {
              ఒకవేళ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  ఒకవేళ (conversions_ముసుగు & C_NOERROR)
                    print_stats ();
                }
              లేకపోతే
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          లేకపోతే ఒకవేళ (nread == 0)
            ఆపు;
          లేకపోతే ఒకవేళ (fdesc == STDIN_FILENO)
            advance_ఆదానం_స్థానభ్రంశం (nread);

          ఒకవేళ (రికార్డులు != 0)
            రికార్డులు--;
          లేకపోతే
            *బైట్లు = 0;
        }
      అయితే (రికార్డులు || *బైట్లు);

      తిరిగి రికార్డులు;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

స్థిర bool
advance_ఆదానం_after_read_error (idx_t nbytes)
{
  ఒకవేళ (! ఆదానం_seekable)
    {
      ఒకవేళ (ఆదానం_seek_errno == ESPIPE)
        తిరిగి true;
      errno = ఆదానం_seek_errno;
    }
  లేకపోతే
    {
      off_t స్థానభ్రంశం;
      advance_ఆదానం_స్థానభ్రంశం (nbytes);
      ఒకవేళ (ఆదానం_స్థానభ్రంశం < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (ఆదానం_file));
          తిరిగి false;
        }
      స్థానభ్రంశం = lseek (STDIN_FILENO, 0, SEEK_CUR);
      ఒకవేళ (0 <= స్థానభ్రంశం)
        {
          off_t diff;
          ఒకవేళ (స్థానభ్రంశం == ఆదానం_స్థానభ్రంశం)
            తిరిగి true;
          diff = ఆదానం_స్థానభ్రంశం - స్థానభ్రంశం;
          ఒకవేళ (! (0 <= diff && diff <= nbytes) && స్థితి_level != స్థితి_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          ఒకవేళ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            తిరిగి true;
          ఒకవేళ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (ఆదానం_file));
  తిరిగి false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

స్థిర శూన్య
copy_simple (అక్షరం స్థిరరాశి *buf, idx_t nread)
{
  అక్షరం స్థిరరాశి *ప్రారంభం = buf;	/* First uncopied char in BUF.  */

  చేయి
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      జ్ఞాపకం_నకలు (obuf + oc, ప్రారంభం, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      ప్రారంభం += nfree;
      oc += nfree;
      ఒకవేళ (oc >= output_blocksize)
        write_output ();
    }
  అయితే (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

స్థిర శూన్య
copy_with_ఖండం (అక్షరం స్థిరరాశి *buf, idx_t nread)
{
  కొరకు (idx_t i = nread; i; i--, buf++)
    {
      ఒకవేళ (*buf == newline_character)
        {
          ఒకవేళ (నిలువు < conversion_blocksize)
            {
              కొరకు (idx_t j = నిలువు; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          నిలువు = 0;
        }
      లేకపోతే
        {
          ఒకవేళ (నిలువు == conversion_blocksize)
            r_truncate++;
          లేకపోతే ఒకవేళ (నిలువు < conversion_blocksize)
            output_char (*buf);
          నిలువు++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

స్థిర శూన్య
copy_with_unblock (అక్షరం స్థిరరాశి *buf, idx_t nread)
{
  స్థిర idx_t pending_spaces = 0;

  కొరకు (idx_t i = 0; i < nread; i++)
    {
      అక్షరం c = buf[i];

      ఒకవేళ (నిలువు++ >= conversion_blocksize)
        {
          నిలువు = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      లేకపోతే ఒకవేళ (c == space_character)
        pending_spaces++;
      లేకపోతే
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          అయితే (pending_spaces)
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

స్థిర శూన్య
సమితి_fd_flags (పూర్ణాంకం fd, పూర్ణాంకం కూడు_flags, అక్షరం స్థిరరాశి *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  కూడు_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  ఒకవేళ (కూడు_flags)
    {
      పూర్ణాంకం old_flags = fcntl (fd, F_GETFL);
      పూర్ణాంకం new_flags = old_flags | కూడు_flags;
      bool ok = true;
      ఒకవేళ (old_flags < 0)
        ok = false;
      లేకపోతే ఒకవేళ (old_flags != new_flags)
        {
          ఒకవేళ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              నిర్మాణం stat st;
              ఒకవేళ (ifstat (fd, &st) != 0)
                ok = false;
              లేకపోతే ఒకవేళ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_బహుళకం))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              లేకపోతే ఒకవేళ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          ఒకవేళ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      ఒకవేళ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

స్థిర పూర్ణాంకం
dd_copy (శూన్య)
{
  అక్షరం *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  పూర్ణాంకం exit_స్థితి = EXIT_SUCCESS;
  idx_t n_బైట్లు_read;

  ఒకవేళ (skip_రికార్డులు != 0 || skip_బైట్లు != 0)
    {
      intmax_t us_బైట్లు;
      bool us_బైట్లు_అధికప్రవాహం =
        (ckd_mul (&us_బైట్లు, skip_రికార్డులు, ఆదానం_blocksize)
         || ckd_కూడు (&us_బైట్లు, skip_బైట్లు, us_బైట్లు));
      off_t ఆదానం_offset0 = ఆదానం_స్థానభ్రంశం;
      intmax_t us_ఖండాలు = skip (STDIN_FILENO, ఆదానం_file,
                                 skip_రికార్డులు, ఆదానం_blocksize, &skip_బైట్లు);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      ఒకవేళ ((us_ఖండాలు
           || (0 <= ఆదానం_స్థానభ్రంశం
               && (us_బైట్లు_అధికప్రవాహం
                   || us_బైట్లు != ఆదానం_స్థానభ్రంశం - ఆదానం_offset0)))
          && స్థితి_level != స్థితి_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (ఆదానం_file));
        }
    }

  ఒకవేళ (seek_రికార్డులు != 0 || seek_బైట్లు != 0)
    {
      idx_t బైట్లు = seek_బైట్లు;
      intmax_t write_రికార్డులు = skip (STDOUT_FILENO, output_file,
                                      seek_రికార్డులు, output_blocksize, &బైట్లు);

      ఒకవేళ (write_రికార్డులు != 0 || బైట్లు != 0)
        {
          memset (obuf, 0, write_రికార్డులు ? output_blocksize : బైట్లు);

          చేయి
            {
              idx_t కొలత = write_రికార్డులు ? output_blocksize : బైట్లు;
              ఒకవేళ (iwrite (STDOUT_FILENO, obuf, కొలత) != కొలత)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              ఒకవేళ (write_రికార్డులు != 0)
                write_రికార్డులు--;
              లేకపోతే
                బైట్లు = 0;
            }
          అయితే (write_రికార్డులు || బైట్లు);
        }
    }

  ఒకవేళ (max_రికార్డులు == 0 && max_బైట్లు == 0)
    తిరిగి exit_స్థితి;

  alloc_ibuf ();
  alloc_obuf ();
  పూర్ణాంకం saved_byte = -1;

  అయితే (true)
    {
      ఒకవేళ (స్థితి_level == స్థితి_పురోగతి)
        {
          xtime_t పురోగతి_సమయం = gethrxtime ();
          ఒకవేళ (తదుపరి_సమయం <= పురోగతి_సమయం)
            {
              print_xfer_stats (పురోగతి_సమయం);
              తదుపరి_సమయం += XTIME_PRECISION;
            }
        }

      ఒకవేళ (r_partial + r_నిండిన >= max_రికార్డులు + !!max_బైట్లు)
        ఆపు;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      ఒకవేళ ((conversions_ముసుగు & C_SYNC) && (conversions_ముసుగు & C_NOERROR))
        memset (ibuf,
                (conversions_ముసుగు & (C_ఖండం | C_UNBLOCK)) ? ' ' : '\0',
                ఆదానం_blocksize);

      ఒకవేళ (r_partial + r_నిండిన >= max_రికార్డులు)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_బైట్లు);
      లేకపోతే
        nread = iread_fnc (STDIN_FILENO, ibuf, ఆదానం_blocksize);

      ఒకవేళ (nread > 0)
        {
          advance_ఆదానం_స్థానభ్రంశం (nread);
          ఒకవేళ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      లేకపోతే ఒకవేళ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_ముసుగు & C_NOTRUNC);
          ఆపు;			/* EOF.  */
        }
      లేకపోతే
        {
          ఒకవేళ (!(conversions_ముసుగు & C_NOERROR) || స్థితి_level != స్థితి_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (ఆదానం_file));

          ఒకవేళ (conversions_ముసుగు & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = ఆదానం_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              ఒకవేళ (!advance_ఆదానం_after_read_error (bad_portion))
                {
                  exit_స్థితి = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  ఆదానం_seekable = false;
                  ఆదానం_seek_errno = ESPIPE;
                }
              ఒకవేళ ((conversions_ముసుగు & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              లేకపోతే
                కొనసాగు;
            }
          లేకపోతే
            {
              /* Write any partial block. */
              exit_స్థితి = EXIT_FAILURE;
              ఆపు;
            }
        }

      n_బైట్లు_read = nread;

      ఒకవేళ (n_బైట్లు_read < ఆదానం_blocksize)
        {
          r_partial++;
          partread = n_బైట్లు_read;
          ఒకవేళ (conversions_ముసుగు & C_SYNC)
            {
              ఒకవేళ (!(conversions_ముసుగు & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_బైట్లు_read,
                        (conversions_ముసుగు & (C_ఖండం | C_UNBLOCK)) ? ' ' : '\0',
                        ఆదానం_blocksize - n_బైట్లు_read);
              n_బైట్లు_read = ఆదానం_blocksize;
            }
        }
      లేకపోతే
        {
          r_నిండిన++;
          partread = 0;
        }

      ఒకవేళ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_బైట్లు_read);
          w_బైట్లు += nwritten;
          ఒకవేళ (nwritten != n_బైట్లు_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              ఒకవేళ (nwritten != 0)
                w_partial++;
              తిరిగి EXIT_FAILURE;
            }
          లేకపోతే ఒకవేళ (n_బైట్లు_read == ఆదానం_blocksize)
            w_నిండిన++;
          లేకపోతే
            w_partial++;
          కొనసాగు;
        }

      /* Do any translations on the whole buffer at once.  */

      ఒకవేళ (translation_needed)
        translate_బఫరు (ibuf, n_బైట్లు_read);

      ఒకవేళ (conversions_ముసుగు & C_SWAB)
        bufstart = swab_బఫరు (ibuf, &n_బైట్లు_read, &saved_byte);
      లేకపోతే
        bufstart = ibuf;

      ఒకవేళ (conversions_ముసుగు & C_ఖండం)
        copy_with_ఖండం (bufstart, n_బైట్లు_read);
      లేకపోతే ఒకవేళ (conversions_ముసుగు & C_UNBLOCK)
        copy_with_unblock (bufstart, n_బైట్లు_read);
      లేకపోతే
        copy_simple (bufstart, n_బైట్లు_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  ఒకవేళ (0 <= saved_byte)
    {
      అక్షరం saved_char = saved_byte;
      ఒకవేళ (conversions_ముసుగు & C_ఖండం)
        copy_with_ఖండం (&saved_char, 1);
      లేకపోతే ఒకవేళ (conversions_ముసుగు & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      లేకపోతే
        output_char (saved_char);
    }

  ఒకవేళ ((conversions_ముసుగు & C_ఖండం) && నిలువు > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      కొరకు (idx_t i = నిలువు; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  ఒకవేళ (నిలువు && (conversions_ముసుగు & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  ఒకవేళ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_బైట్లు += nwritten;
      ఒకవేళ (nwritten != 0)
        w_partial++;
      ఒకవేళ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          తిరిగి EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  ఒకవేళ (final_op_was_seek)
    {
      నిర్మాణం stat stdout_stat;
      ఒకవేళ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          తిరిగి EXIT_FAILURE;
        }
      ఒకవేళ (S_ISREG (stdout_stat.st_బహుళకం) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_స్థానభ్రంశం = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          ఒకవేళ (0 <= output_స్థానభ్రంశం && stdout_stat.st_కొలత < output_స్థానభ్రంశం)
            {
              ఒకవేళ (iftruncate (STDOUT_FILENO, output_స్థానభ్రంశం) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_స్థానభ్రంశం}, quoteaf (output_file));
                  తిరిగి EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  ఒకవేళ (conversions_ముసుగు & (C_FDATASYNC | C_FSYNC)
      && స్థితి_level == స్థితి_పురోగతి
      && 0 <= reported_w_బైట్లు && reported_w_బైట్లు < w_బైట్లు)
    print_xfer_stats (0);

  తిరిగి exit_స్థితి;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

స్థిర పూర్ణాంకం
synchronize_output (శూన్య)
{
  పూర్ణాంకం exit_స్థితి = 0;
  పూర్ణాంకం ముసుగు = conversions_ముసుగు;
  conversions_ముసుగు &= ~ (C_FDATASYNC | C_FSYNC);

  ఒకవేళ ((ముసుగు & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      ఒకవేళ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_స్థితి = EXIT_FAILURE;
        }
      ముసుగు |= C_FSYNC;
    }

  ఒకవేళ ((ముసుగు & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      తిరిగి EXIT_FAILURE;
    }

  తిరిగి exit_స్థితి;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  off_t స్థానభ్రంశం;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  పుట_కొలత = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE, సంస్కరణ,
                                   true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  కొరకు (పూర్ణాంకం i = 0; i < 256; i++)
    trans_పట్టిక[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  ఒకవేళ (ఆదానం_file == ఖాళీ)
    {
      ఆదానం_file = _("standard input");
      సమితి_fd_flags (STDIN_FILENO, ఆదానం_flags, ఆదానం_file);
    }
  లేకపోతే
    {
      ఒకవేళ (ifd_reopen (STDIN_FILENO, ఆదానం_file, O_RDONLY | ఆదానం_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (ఆదానం_file));
    }

  స్థానభ్రంశం = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ఆదానం_seekable = (0 <= స్థానభ్రంశం);
  ఆదానం_స్థానభ్రంశం = MAX (0, స్థానభ్రంశం);
  ఆదానం_seek_errno = errno;

  ఒకవేళ (output_file == ఖాళీ)
    {
      output_file = _("standard output");
      సమితి_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  లేకపోతే
    {
      mode_t perms = బహుళకం_RW_UGO;
      పూర్ణాంకం opts
        = (output_flags
           | (conversions_ముసుగు & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_ముసుగు & C_EXCL ? O_EXCL : 0)
           | (seek_రికార్డులు || (conversions_ముసుగు & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t కొలత;
      ఒకవేళ ((ckd_mul (&కొలత, seek_రికార్డులు, output_blocksize)
           || ckd_కూడు (&కొలత, seek_బైట్లు, కొలత))
          && !(conversions_ముసుగు & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_రికార్డులు, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      ఒకవేళ ((! seek_రికార్డులు
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      ఒకవేళ (seek_రికార్డులు != 0 && !(conversions_ముసుగు & C_NOTRUNC))
        {
          ఒకవేళ (iftruncate (STDOUT_FILENO, కొలత) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              పూర్ణాంకం ftruncate_errno = errno;
              నిర్మాణం stat stdout_stat;
              ఒకవేళ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              లేకపోతే ఒకవేళ (S_ISREG (stdout_stat.st_బహుళకం)
                       || S_ISDIR (stdout_stat.st_బహుళకం)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = కొలత;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  ప్రారంభం_సమయం = gethrxtime ();
  తదుపరి_సమయం = ప్రారంభం_సమయం + XTIME_PRECISION;

  పూర్ణాంకం copy_స్థితి = dd_copy ();
  పూర్ణాంకం sync_స్థితి = synchronize_output ();
  పూర్ణాంకం exit_స్థితి = copy_స్థితి | sync_స్థితి;

  ఒకవేళ (max_రికార్డులు == 0 && max_బైట్లు == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      ఒకవేళ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (ఆదానం_file));
          exit_స్థితి = EXIT_FAILURE;
        }
      ఒకవేళ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_స్థితి = EXIT_FAILURE;
        }
    }
  లేకపోతే
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      ఒకవేళ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      ఒకవేళ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  ముగింపు_పైకి ();
  main_exit (exit_స్థితి);
}
