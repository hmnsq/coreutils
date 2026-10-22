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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "alignalloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "close-stream.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gethrxtime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "dd"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Stuart Kemp")

/* NonStop circa 2011 lacks SA_RESETHAND; see Bug#9076.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SA_RESETHAND
# define SA_RESETHAND 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGINFO
# define SIGINFO SIGUSR1
#ਜੇ_ਅੰਤ

/* This may belong in GNULIB's fcntl module instead.
   Define O_CIO to 0 if it is not supported by this OS. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ O_CIO
# define O_CIO 0
#ਜੇ_ਅੰਤ

/* On AIX 5.1 and AIX 5.2, O_NOCACHE is defined via <fcntl.h>
   and would interfere with our use of that name, below.  */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ O_NOCACHE

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ output_char(c)				\
  ਕਰੋ						\
    {						\
      obuf[oc++] = (c);				\
      ਜੇ (oc >= output_blocksize)		\
        write_output ();			\
    }						\
  ਜਦੋਂ (0)

/* Default input and output blocksize. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_BLOCKSIZE 512

/* Conversions bit masks. */
ਗਿਣਤੀ
  {
    C_ASCII = 01,

    C_EBCDIC = 02,
    C_IBM = 04,
    C_ਖੰਡ = 010,
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
ਗਿਣਤੀ
  {
    ਸਥਿਤੀ_NONE = 1,
    ਸਥਿਤੀ_NOXFER = 2,
    ਸਥਿਤੀ_DEFAULT = 3,
    ਸਥਿਤੀ_ਤਰੱਕੀ = 4
  };

/* The name of the input file, or NULL for the standard input. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਨਿਵੇਸ਼_file = ਨਿਰਾ;

/* The name of the output file, or NULL for the standard output. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *output_file = ਨਿਰਾ;

/* The page size on this host.  */
ਸਥਿਰ idx_t ਪੰਨਾ_ਮਾਪ;

/* The number of bytes in which atomic reads are done. */
ਸਥਿਰ idx_t ਨਿਵੇਸ਼_blocksize = 0;

/* The number of bytes in which atomic writes are done. */
ਸਥਿਰ idx_t output_blocksize = 0;

/* Conversion buffer size, in bytes.  0 prevents conversions. */
ਸਥਿਰ idx_t conversion_blocksize = 0;

/* Skip this many records of 'input_blocksize' bytes before input. */
ਸਥਿਰ intmax_t skip_ਰਿਕਾਰਡਾਂ = 0;

/* Skip this many bytes before input in addition of 'skip_records'
   records.  */
ਸਥਿਰ idx_t skip_ਬਾਈਟਾਂ = 0;

/* Skip this many records of 'output_blocksize' bytes before output. */
ਸਥਿਰ intmax_t seek_ਰਿਕਾਰਡਾਂ = 0;

/* Skip this many bytes in addition to 'seek_records' records before
   output.  */
ਸਥਿਰ intmax_t seek_ਬਾਈਟਾਂ = 0;

/* Whether the final output was done with a seek (rather than a write).  */
ਸਥਿਰ bool final_op_was_seek;

/* Copy only this many records.  The default is effectively infinity.  */
ਸਥਿਰ intmax_t max_ਰਿਕਾਰਡਾਂ = INTMAX_MAX;

/* Copy this many bytes in addition to 'max_records' records.  */
ਸਥਿਰ idx_t max_ਬਾਈਟਾਂ = 0;

/* Bit vector of conversions to apply. */
ਸਥਿਰ ਪੂਰਨ conversions_ਨਕਾਬ = 0;

/* Open flags for the input and output files.  */
ਸਥਿਰ ਪੂਰਨ ਨਿਵੇਸ਼_flags = 0;
ਸਥਿਰ ਪੂਰਨ output_flags = 0;

/* Status flags for what is printed to stderr.  */
ਸਥਿਰ ਪੂਰਨ ਸਥਿਤੀ_level = ਸਥਿਤੀ_DEFAULT;

/* If nonzero, filter characters through the translation table.  */
ਸਥਿਰ bool translation_needed = false;

/* Number of partial blocks written. */
ਸਥਿਰ intmax_t w_partial = 0;

/* Number of full blocks written. */
ਸਥਿਰ intmax_t w_ਭਰਿਆ = 0;

/* Number of partial blocks read. */
ਸਥਿਰ intmax_t r_partial = 0;

/* Number of full blocks read. */
ਸਥਿਰ intmax_t r_ਭਰਿਆ = 0;

/* Number of bytes written.  */
ਸਥਿਰ intmax_t w_ਬਾਈਟਾਂ = 0;

/* Last-reported number of bytes written, or negative if never reported.  */
ਸਥਿਰ intmax_t reported_w_ਬਾਈਟਾਂ = -1;

/* Time that dd started.  */
ਸਥਿਰ xtime_t ਸ਼ੁਰੂ_ਸਮਾਂ;

/* Next time to report periodic progress.  */
ਸਥਿਰ xtime_t ਅਗਲਾ_ਸਮਾਂ;

/* If positive, the number of bytes output in the current progress line.  */
ਸਥਿਰ ਪੂਰਨ ਤਰੱਕੀ_len;

/* True if input is seekable.  */
ਸਥਿਰ bool ਨਿਵੇਸ਼_seekable;

/* Error number corresponding to initial attempt to lseek input.
   If ESPIPE, do not issue any more diagnostics about it.  */
ਸਥਿਰ ਪੂਰਨ ਨਿਵੇਸ਼_seek_errno;

/* File offset of the input, in bytes, or -1 if it overflowed.  */
ਸਥਿਰ off_t ਨਿਵੇਸ਼_ਵਿੱਥ;

/* True if a partial read should be diagnosed.  */
ਸਥਿਰ bool warn_partial_read;

/* Records truncated by conv=block. */
ਸਥਿਰ intmax_t r_truncate = 0;

/* Output representation of newline and space characters.
   They change if we're converting to EBCDIC.  */
ਸਥਿਰ ਅੱਖਰ newline_character = '\n';
ਸਥਿਰ ਅੱਖਰ space_character = ' ';

/* I/O buffers.  */
ਸਥਿਰ ਅੱਖਰ *ibuf;
ਸਥਿਰ ਅੱਖਰ *obuf;

/* Current index into 'obuf'. */
ਸਥਿਰ idx_t oc = 0;

/* Index into current line, for 'conv=block' and 'conv=unblock'.  */
ਸਥਿਰ idx_t ਖਾਨਾ = 0;

/* The set of signals that are caught.  */
ਸਥਿਰ sigset_t caught_ਸੰਕੇਤ;

/* If nonzero, the value of the pending fatal signal.  */
ਸਥਿਰ sig_atomic_t ਅਸਥਿਰ ਰੁਕਾਵਟ_signal;

/* A count of the number of pending info signals that have been received.  */
ਸਥਿਰ sig_atomic_t ਅਸਥਿਰ info_signal_ਗਿਣਤੀ;

/* Whether to discard cache for input or output.  */
ਸਥਿਰ bool i_nocache, o_nocache;

/* Whether to instruct the kernel to discard the complete file.  */
ਸਥਿਰ bool i_nocache_eof, o_nocache_eof;

/* Function used for read (to handle iflag=fullblock parameter).  */
ਸਥਿਰ ssize_t (*iread_fnc) (ਪੂਰਨ fd, ਅੱਖਰ *buf, idx_t ਮਾਪ);

/* A longest symbol in the struct symbol_values tables below.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LONGEST_ਚਿੰਨ੍ਹ "count_bytes"

/* A symbol and the corresponding integer value.  */
ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ
{
  ਅੱਖਰ ਚਿੰਨ੍ਹ[ਆਕਾਰ LONGEST_ਚਿੰਨ੍ਹ];
  ਪੂਰਨ ਮੁੱਲ;
};

/* Conversion symbols, for conv="...".  */
ਸਥਿਰ ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ ਅਚਲ conversions[] =
{
  {"ascii", C_ASCII | C_UNBLOCK | C_TWOBUFS},	/* EBCDIC to ASCII. */
  {"ebcdic", C_EBCDIC | C_ਖੰਡ | C_TWOBUFS},	/* ASCII to EBCDIC. */
  {"ibm", C_IBM | C_ਖੰਡ | C_TWOBUFS},	/* Different ASCII to EBCDIC. */
  {"block", C_ਖੰਡ | C_TWOBUFS},	/* Variable to fixed length records. */
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FFS_ਨਕਾਬ(x) ((x) ^ ((x) & ((x) - 1)))
ਗਿਣਤੀ
  {
    /* Compute a value that's bitwise disjoint from the union
       of all O_ values.  */
    v = ~(0
          | O_APPEND
          | O_ਦੁਆਧਾਰੀ
          | O_CIO
          | O_ਸਿੱਧਾ
          | O_DIRECTORY
          | O_DSYNC
          | O_EXCL
          | O_NOATIME
          | O_NOCTTY
          | O_NOFOLLOW
          | O_NOLINKS
          | O_NONBLOCK
          | O_SYNC
          | O_ਪਾਠ
          ),

    /* Use its lowest bits for private flags.  */
    O_FULLBLOCK = FFS_ਨਕਾਬ (v),
    v2 = v ^ O_FULLBLOCK,

    O_NOCACHE = FFS_ਨਕਾਬ (v2),
    v3 = v2 ^ O_NOCACHE,

    O_ਗਿਣਤੀ_ਬਾਈਟਾਂ = FFS_ਨਕਾਬ (v3),
    v4 = v3 ^ O_ਗਿਣਤੀ_ਬਾਈਟਾਂ,

    O_SKIP_ਬਾਈਟਾਂ = FFS_ਨਕਾਬ (v4),
    v5 = v4 ^ O_SKIP_ਬਾਈਟਾਂ,

    O_SEEK_ਬਾਈਟਾਂ = FFS_ਨਕਾਬ (v5)
  };

/* Ensure that we got something.  */
static_assert (O_FULLBLOCK != 0);
static_assert (O_NOCACHE != 0);
static_assert (O_ਗਿਣਤੀ_ਬਾਈਟਾਂ != 0);
static_assert (O_SKIP_ਬਾਈਟਾਂ != 0);
static_assert (O_SEEK_ਬਾਈਟਾਂ != 0);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ(i) (((i) & ((i) - 1)) != 0)

/* Ensure that this is a single-bit value.  */
static_assert ( ! ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (O_FULLBLOCK));
static_assert ( ! ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (O_NOCACHE));
static_assert ( ! ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (O_ਗਿਣਤੀ_ਬਾਈਟਾਂ));
static_assert ( ! ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (O_SKIP_ਬਾਈਟਾਂ));
static_assert ( ! ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (O_SEEK_ਬਾਈਟਾਂ));

/* Flags, for iflag="..." and oflag="...".  */
ਸਥਿਰ ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ ਅਚਲ flags[] =
{
  {"append",	  O_APPEND},
  {"binary",	  O_ਦੁਆਧਾਰੀ},
  {"cio",	  O_CIO},
  {"direct",	  O_ਸਿੱਧਾ},
  {"directory",   O_DIRECTORY},
  {"dsync",	  O_DSYNC},
  {"noatime",	  O_NOATIME},
  {"nocache",	  O_NOCACHE},   /* Discard cache.  */
  {"noctty",	  O_NOCTTY},
  {"nofollow",	  HAVE_WORKING_O_NOFOLLOW ? O_NOFOLLOW : 0},
  {"nolinks",	  O_NOLINKS},
  {"nonblock",	  O_NONBLOCK},
  {"sync",	  O_SYNC},
  {"text",	  O_ਪਾਠ},
  {"fullblock",   O_FULLBLOCK}, /* Accumulate full blocks from input.  */
  {"count_bytes", O_ਗਿਣਤੀ_ਬਾਈਟਾਂ},
  {"skip_bytes",  O_SKIP_ਬਾਈਟਾਂ},
  {"seek_bytes",  O_SEEK_ਬਾਈਟਾਂ},
  {"",		0}
};

/* Status, for status="...".  */
ਸਥਿਰ ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ ਅਚਲ statuses[] =
{
  {"none",	ਸਥਿਤੀ_NONE},
  {"noxfer",	ਸਥਿਤੀ_NOXFER},
  {"progress",	ਸਥਿਤੀ_ਤਰੱਕੀ},
  {"",		0}
};

/* Translation table formed by applying successive transformations. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ trans_ਸਾਰਣੀ[256];

/* Standard translation tables, taken from POSIX 1003.1-2013.
   Beware of imitations; there are lots of ASCII<->EBCDIC tables
   floating around the net, perhaps valid for some applications but
   not correct here.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ ascii_ਮੰਜ਼ਿਲ_ebcdic[] =
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

ਸਥਿਰ ਅੱਖਰ ਅਚਲ ascii_ਮੰਜ਼ਿਲ_ibm[] =
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

ਸਥਿਰ ਅੱਖਰ ਅਚਲ ebcdic_ਮੰਜ਼ਿਲ_ascii[] =
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
ਸਥਿਰ bool close_stdout_required = true;

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
ਸਥਿਰ ਖਾਲੀ
maybe_close_stdout (ਖਾਲੀ)
{
  ਜੇ (close_stdout_required)
    close_stdout ();
  ਨਹੀਂਤਾਂ ਜੇ (close_ਧਾਰਾ (stderr) != 0)
    _exit (EXIT_FAILURE);
}

/* Like the 'error' function but handle any pending newline,
   and do not exit.  */

ATTRIBUTE_FORMAT ((__printf__, 2, 3))
ਸਥਿਰ ਖਾਲੀ
diagnose (ਪੂਰਨ errnum, ਅੱਖਰ ਅਚਲ *fmt, ...)
{
  ਜੇ (0 < ਤਰੱਕੀ_len)
    {
      fputc ('\n', stderr);
      ਤਰੱਕੀ_len = 0;
    }

  va_list ap;
  va_ਸ਼ੁਰੂ (ap, fmt);
  verror (0, errnum, fmt, ap);
  va_ਸਿਰਾ (ap);
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [ਸੰਕਾਰਜ]...\n\
  or:  %s OPTION\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Copy a file, converting and formatting according ਮੰਜ਼ਿਲ the operands.\n\
\n\
"), stdout);
      oputs (_("\
  bs=ਬਾਈਟਾਂ_2        ਪਠ and ਲਿਖ ਉੱਪਰ ਮੰਜ਼ਿਲ ਬਾਈਟਾਂ_2 ਬਾਈਟਾਂ at a ਸਮਾਂ (ਮੂਲ: 512);\n\
                  overrides ibs and obs\n\
"));
      oputs (_("\
  cbs=ਬਾਈਟਾਂ_2       convert ਬਾਈਟਾਂ_2 ਬਾਈਟਾਂ at a ਸਮਾਂ\n\
"));
      oputs (_("\
  conv=CONVS      convert the file as ਪ੍ਰਤੀ the comma separated ਚਿੰਨ੍ਹ list\n\
"));
      oputs (_("\
  ਗਿਣਤੀ_2=N         copy only N ਨਿਵੇਸ਼ ਖੰਡਾਂ\n\
"));
      oputs (_("\
  ibs=ਬਾਈਟਾਂ_2       ਪਠ ਉੱਪਰ ਮੰਜ਼ਿਲ ਬਾਈਟਾਂ_2 ਬਾਈਟਾਂ at a ਸਮਾਂ (ਮੂਲ: 512)\n\
"));
      oputs (_("\
  ਜੇ=FILE         ਪਠ from FILE instead ਦਾ standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  iflag=FLAGS     ਪਠ as ਪ੍ਰਤੀ the comma separated ਚਿੰਨ੍ਹ list\n\
"));
      oputs (_("\
  obs=ਬਾਈਟਾਂ_2       ਲਿਖ ਬਾਈਟਾਂ_2 ਬਾਈਟਾਂ at a ਸਮਾਂ (ਮੂਲ: 512)\n\
"));
      oputs (_("\
  ਦਾ=FILE         ਲਿਖ ਮੰਜ਼ਿਲ FILE instead ਦਾ standard output\n\
"));
      oputs (_("\
  oflag=FLAGS     ਲਿਖ as ਪ੍ਰਤੀ the comma separated ਚਿੰਨ੍ਹ list\n\
"));
      oputs (_("\
  seek=N          (or oseek=N) skip N obs sized output ਖੰਡਾਂ\n\
"));
      oputs (_("\
  skip=N          (or iseek=N) skip N ibs sized ਨਿਵੇਸ਼ ਖੰਡਾਂ\n\
"));
      oputs (_("\
  ਸਥਿਤੀ=LEVEL    The LEVEL ਦਾ information ਮੰਜ਼ਿਲ print ਮੰਜ਼ਿਲ standard error;\n\
                  'none' suppresses everything but error messages,\n\
                  'noxfer' suppresses the final transfer statistics,\n\
                  'progress' shows periodic transfer statistics\n\
"));
      fputs (_("\
\n\
N and ਬਾਈਟਾਂ_2 may be followed by the following multiplicative suffixes:\n\
c=1, w=2, b=512, kB=1000, K=1024, MB=1000*1000, M=1024*1024, xM=M,\n\
GB=1000*1000*1000, G=1024*1024*1024, and so on ਲਈ T, P, E, Z, Y, R, Q.\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
If N ਸਿਰੇ ਵਿੱਚ 'B', it counts ਬਾਈਟਾਂ not ਖੰਡਾਂ.\n\
\n\
Each CONV ਚਿੰਨ੍ਹ may be:\n\
\n\
"), stdout);
      fputs (_("\
  ascii     from EBCDIC ਮੰਜ਼ਿਲ ASCII\n\
  ebcdic    from ASCII ਮੰਜ਼ਿਲ EBCDIC\n\
  ibm       from ASCII ਮੰਜ਼ਿਲ alternate EBCDIC\n\
  ਖੰਡ     pad newline-terminated ਰਿਕਾਰਡਾਂ with spaces ਮੰਜ਼ਿਲ cbs-ਮਾਪ\n\
  unblock   replace trailing spaces ਵਿੱਚ cbs-ਮਾਪ ਰਿਕਾਰਡਾਂ with newline\n\
  lcase     change ਉੱਤਲਾ ਹਾਲਤ ਮੰਜ਼ਿਲ ਹੇਠਲਾ ਹਾਲਤ\n\
  ucase     change ਹੇਠਲਾ ਹਾਲਤ ਮੰਜ਼ਿਲ ਉੱਤਲਾ ਹਾਲਤ\n\
  sparse    try ਮੰਜ਼ਿਲ seek rather than ਲਿਖ all-NUL output ਖੰਡਾਂ\n\
  swab      ਅਦਲਾ_ਬਦਲੀ every pair ਦਾ ਨਿਵੇਸ਼ ਬਾਈਟਾਂ\n\
  sync      pad every ਨਿਵੇਸ਼ ਖੰਡ with NULs ਮੰਜ਼ਿਲ ibs-ਮਾਪ; when ਵਰਤਿਆ\n\
            with ਖੰਡ or unblock, pad with spaces rather than NULs\n\
"), stdout);
      fputs (_("\
  excl      fail ਜੇ the output file already exists\n\
  nocreat   ਕਰੋ not create the output file\n\
  notrunc   ਕਰੋ not truncate the output file\n\
  noerror   ਜਾਰੀ after ਪਠ errors\n\
  fdatasync  physically ਲਿਖ output file ਅੰਕੜੇ before finishing\n\
  fsync     likewise, but also ਲਿਖ metadata\n\
"), stdout);
      fputs (_("\
\n\
Each FLAG ਚਿੰਨ੍ਹ may be:\n\
\n\
  append    append ਬਹੁਲਕ_2 (makes sense only ਲਈ output; conv=notrunc suggested)\n\
"), stdout);
      ਜੇ (O_CIO)
        fputs (_("  cio       use concurrent I/O for data\n"), stdout);
      ਜੇ (O_ਸਿੱਧਾ)
        fputs (_("  direct    use direct I/O for data\n"), stdout);
      fputs (_("  directory  fail unless a directory\n"), stdout);
      ਜੇ (O_DSYNC)
        fputs (_("  dsync     use synchronized I/O for data\n"), stdout);
      ਜੇ (O_SYNC)
        fputs (_("  sync      likewise, but also for metadata\n"), stdout);
      fputs (_("  fullblock  accumulate full blocks of input (iflag only)\n"),
             stdout);
      ਜੇ (O_NONBLOCK)
        fputs (_("  nonblock  use non-blocking I/O\n"), stdout);
      ਜੇ (O_NOATIME)
        fputs (_("  noatime   do not update access time\n"), stdout);
#ਜੇ HAVE_POSIX_FADVISE
      ਜੇ (O_NOCACHE)
        fputs (_("  nocache   Request to drop cache.  See also oflag=sync\n"),
               stdout);
#ਜੇ_ਅੰਤ
      ਜੇ (O_NOCTTY)
        fputs (_("  noctty    do not assign controlling terminal from file\n"),
               stdout);
      ਜੇ (HAVE_WORKING_O_NOFOLLOW)
        fputs (_("  nofollow  do not follow symlinks\n"), stdout);
      ਜੇ (O_NOLINKS)
        fputs (_("  nolinks   fail if multiply-linked\n"), stdout);
      ਜੇ (O_ਦੁਆਧਾਰੀ)
        fputs (_("  binary    use binary I/O for data\n"), stdout);
      ਜੇ (O_ਪਾਠ)
        fputs (_("  text      use text I/O for data\n"), stdout);

      {
        ਲਿਖੋ (_("\
\n\
Sending a %s ਸੰਕੇਤ ਮੰਜ਼ਿਲ a running 'dd' process makes it\n\
print I/O statistics ਮੰਜ਼ਿਲ standard error and then resume copying.\n\
\n\
Options are:\n\
\n\
"), SIGINFO == SIGUSR1 ? "USR1" : "INFO");
      }

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Common options to use when displaying sizes and rates.  */

ਗਿਣਤੀ { human_opts = (human_autoscale | human_ਗੇੜਾ_ਮੰਜ਼ਿਲ_ਸਭ_ਤੋਂ_ਨੇੜੇ
                     | human_space_before_unit | human_SI | human_B) };

/* Ensure input buffer IBUF is allocated.  */

ਸਥਿਰ ਖਾਲੀ
alloc_ibuf (ਖਾਲੀ)
{
  ਜੇ (ibuf)
    ਵਾਪਸ;

  bool extra_byte_for_swab = !!(conversions_ਨਕਾਬ & C_SWAB);
  ibuf = alignalloc (ਪੰਨਾ_ਮਾਪ, ਨਿਵੇਸ਼_blocksize + extra_byte_for_swab);
  ਜੇ (!ibuf)
    {
      ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
      error (EXIT_FAILURE, 0,
             _("memory exhausted by input buffer of size %td bytes (%s)"),
             ਨਿਵੇਸ਼_blocksize,
             human_readable (ਨਿਵੇਸ਼_blocksize, hbuf,
                             human_opts | human_ਅਧਾਰ_1024, 1, 1));
    }
}

/* Ensure output buffer OBUF is allocated/initialized.  */

ਸਥਿਰ ਖਾਲੀ
alloc_obuf (ਖਾਲੀ)
{
  ਜੇ (obuf)
    ਵਾਪਸ;

  ਜੇ (conversions_ਨਕਾਬ & C_TWOBUFS)
    {
      obuf = alignalloc (ਪੰਨਾ_ਮਾਪ, output_blocksize);
      ਜੇ (!obuf)
        {
          ਅੱਖਰ hbuf[LONGEST_HUMAN_READABLE + 1];
          error (EXIT_FAILURE, 0,
                 _("memory exhausted by output buffer of size %td"
                   " bytes (%s)"),
                 output_blocksize,
                 human_readable (output_blocksize, hbuf,
                                 human_opts | human_ਅਧਾਰ_1024, 1, 1));
        }
    }
  ਨਹੀਂਤਾਂ
    {
      alloc_ibuf ();
      obuf = ibuf;
    }
}

ਸਥਿਰ ਖਾਲੀ
translate_charset (ਅੱਖਰ ਅਚਲ *new_trans)
{
  ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
    trans_ਸਾਰਣੀ[i] = new_trans[trans_ਸਾਰਣੀ[i]];
  translation_needed = true;
}

/* Return true if I has more than one bit set.  I must be nonnegative.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (ਪੂਰਨ i)
{
  ਵਾਪਸ ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ (i);
}

ਸਥਿਰ bool
abbreviation_lacks_ਅਗੇਤਰ (ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2)
{
  ਵਾਪਸ ਸੁਨੇਹਾ_2[ਲੜੀ_ਲੰਬਾਈ (ਸੁਨੇਹਾ_2) - 2] == ' ';
}

/* Print transfer statistics.  */

ਸਥਿਰ ਖਾਲੀ
print_xfer_stats (xtime_t ਤਰੱਕੀ_ਸਮਾਂ)
{
  xtime_t now = ਤਰੱਕੀ_ਸਮਾਂ ? ਤਰੱਕੀ_ਸਮਾਂ : gethrxtime ();
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ slash_s[] = "/s";
  ਅੱਖਰ hbuf[3][LONGEST_HUMAN_READABLE + ਆਕਾਰ slash_s];
  ਦੁੱਗਣਾ delta_s;
  ਅੱਖਰ ਅਚਲ *ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ;
  ਅੱਖਰ ਅਚਲ *si = human_readable (w_ਬਾਈਟਾਂ, hbuf[0], human_opts, 1, 1);
  ਅੱਖਰ ਅਚਲ *iec = human_readable (w_ਬਾਈਟਾਂ, hbuf[1],
                                    human_opts | human_ਅਧਾਰ_1024, 1, 1);

  /* Use integer arithmetic to compute the transfer rate,
     since that makes it easy to use SI abbreviations.  */
  ਅੱਖਰ *bpsbuf = hbuf[2];
  ਪੂਰਨ bpsbufsize = ਆਕਾਰ hbuf[2];
  ਜੇ (ਸ਼ੁਰੂ_ਸਮਾਂ < now)
    {
      ਦੁੱਗਣਾ XTIME_PRECISIONe0 = XTIME_PRECISION;
      xtime_t delta_xtime = now - ਸ਼ੁਰੂ_ਸਮਾਂ;
      delta_s = delta_xtime / XTIME_PRECISIONe0;
      ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ = human_readable (w_ਬਾਈਟਾਂ, bpsbuf, human_opts,
                                         XTIME_PRECISION, delta_xtime);
      strcat (ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ - bpsbuf + bpsbuf, slash_s);
    }
  ਨਹੀਂਤਾਂ
    {
      delta_s = 0;
      snprintf (bpsbuf, bpsbufsize, "%s B/s", _("Infinity"));
      ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ = bpsbuf;
    }

  ਜੇ (ਤਰੱਕੀ_ਸਮਾਂ)
    fputc ('\r', stderr);

  /* Use full seconds when printing progress, since the progress
     report is output once per second and there is little point
     displaying any subsecond jitter.  Use default precision with %g
     otherwise, as this provides more-useful output then.  With long
     transfers %g can generate a number with an exponent; that is OK.  */
  ਅੱਖਰ delta_s_buf[24];
  snprintf (delta_s_buf, ਆਕਾਰ delta_s_buf,
            ਤਰੱਕੀ_ਸਮਾਂ ? "%.0f s" : "%g s", delta_s);

  ਪੂਰਨ stats_len
    = (abbreviation_lacks_ਅਗੇਤਰ (si)
       ? fprintf (stderr,
                  ngettext ("%jd byte copied, %s, %s",
                            "%jd bytes copied, %s, %s",
                            select_plural (w_ਬਾਈਟਾਂ)),
                  w_ਬਾਈਟਾਂ, delta_s_buf, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ)
       : abbreviation_lacks_ਅਗੇਤਰ (iec)
       ? fprintf (stderr,
                  _("%jd bytes (%s) copied, %s, %s"),
                  w_ਬਾਈਟਾਂ, si, delta_s_buf, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ)
       : fprintf (stderr,
                  _("%jd bytes (%s, %s) copied, %s, %s"),
                  w_ਬਾਈਟਾਂ, si, iec, delta_s_buf, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਦੂਜਾ));

  ਜੇ (ਤਰੱਕੀ_ਸਮਾਂ)
    {
      /* Erase any trailing junk on the output line by outputting
         spaces.  In theory this could glitch the display because the
         formatted translation of a line describing a larger file
         could consume fewer screen columns than the strlen difference
         from the previously formatted translation.  In practice this
         does not seem to be a problem.  */
      ਜੇ (0 <= stats_len && stats_len < ਤਰੱਕੀ_len)
        fprintf (stderr, "%*s", ਤਰੱਕੀ_len - stats_len, "");
      ਤਰੱਕੀ_len = stats_len;
    }
  ਨਹੀਂਤਾਂ
    fputc ('\n', stderr);

  reported_w_ਬਾਈਟਾਂ = w_ਬਾਈਟਾਂ;
}

ਸਥਿਰ ਖਾਲੀ
print_stats (ਖਾਲੀ)
{
  ਜੇ (ਸਥਿਤੀ_level == ਸਥਿਤੀ_NONE)
    ਵਾਪਸ;

  ਜੇ (0 < ਤਰੱਕੀ_len)
    {
      fputc ('\n', stderr);
      ਤਰੱਕੀ_len = 0;
    }

  fprintf (stderr,
           _("%jd+%jd records in\n"
             "%jd+%jd records out\n"),
           r_ਭਰਿਆ, r_partial, w_ਭਰਿਆ, w_partial);

  ਜੇ (r_truncate != 0)
    fprintf (stderr,
             ngettext ("%jd truncated record\n",
                       "%jd truncated records\n",
                       select_plural (r_truncate)),
             r_truncate);

  ਜੇ (ਸਥਿਤੀ_level == ਸਥਿਤੀ_NOXFER)
    ਵਾਪਸ;

  print_xfer_stats (0);
}

/* An ordinary signal was received; arrange for the program to exit.  */

ਸਥਿਰ ਖਾਲੀ
ਰੁਕਾਵਟ_handler (ਪੂਰਨ sig)
{
  ਜੇ (! SA_RESETHAND)
    ਸੰਕੇਤ (sig, SIG_DFL);
  ਰੁਕਾਵਟ_signal = sig;
}

/* An info signal was received; arrange for the program to print status.  */

ਸਥਿਰ ਖਾਲੀ
siginfo_handler (MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ sig)
{
  info_signal_ਗਿਣਤੀ++;
}

/* Install the signal handlers.  */

ਸਥਿਰ ਖਾਲੀ
install_signal_handlers (ਖਾਲੀ)
{
  bool catch_siginfo = ! (SIGINFO == SIGUSR1 && getenv ("POSIXLY_CORRECT"));

  ਬਣਤਰ sigaction act;
  sigemptyset (&caught_ਸੰਕੇਤ);
  ਜੇ (catch_siginfo)
    sigaddset (&caught_ਸੰਕੇਤ, SIGINFO);
  sigaction (SIGINT, ਨਿਰਾ, &act);
  ਜੇ (act.sa_handler != SIG_IGN)
    sigaddset (&caught_ਸੰਕੇਤ, SIGINT);
  act.sa_mask = caught_ਸੰਕੇਤ;

  ਜੇ (sigismember (&caught_ਸੰਕੇਤ, SIGINFO))
    {
      act.sa_handler = siginfo_handler;
      /* Note we don't use SA_RESTART here and instead
         handle EINTR explicitly in iftruncate etc.
         to avoid blocking on uncommitted read/write calls.  */
      act.sa_flags = 0;
      sigaction (SIGINFO, &act, ਨਿਰਾ);
    }

  ਜੇ (sigismember (&caught_ਸੰਕੇਤ, SIGINT))
    {
      act.sa_handler = ਰੁਕਾਵਟ_handler;
      act.sa_flags = SA_NODEFER | SA_RESETHAND;
      sigaction (SIGINT, &act, ਨਿਰਾ);
    }
}

/* Close FD.  Return 0 if successful, -1 (setting errno) otherwise.
   If close fails with errno == EINTR, POSIX says the file descriptor
   is in an unspecified state, so keep trying to close FD but do not
   consider EBADF to be an error.  Do not process signals.  This all
   differs somewhat from functions like ifdatasync and ifsync.  */
ਸਥਿਰ ਪੂਰਨ
iclose (ਪੂਰਨ fd)
{
  ਜੇ (close (fd) != 0)
    ਕਰੋ
      ਜੇ (errno != EINTR)
        ਵਾਪਸ -1;
    ਜਦੋਂ (close (fd) != 0 && errno != EBADF);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ synchronize_output (ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ
cleanup (ਖਾਲੀ)
{
  ਜੇ (!ਰੁਕਾਵਟ_signal)
    {
      ਪੂਰਨ sync_ਸਥਿਤੀ = synchronize_output ();
      ਜੇ (sync_ਸਥਿਤੀ)
        ਬਾਹਰ_ਨਿਕਲੋ (sync_ਸਥਿਤੀ);
    }

  ਜੇ (iclose (STDIN_FILENO) != 0)
    error (EXIT_FAILURE, errno, _("closing input file %s"),
           quoteaf (ਨਿਵੇਸ਼_file));

  /* Don't remove this call to close, even though close_stdout
     closes standard output.  This close is necessary when cleanup
     is called as a consequence of signal handling.  */
  ਜੇ (iclose (STDOUT_FILENO) != 0)
    error (EXIT_FAILURE, errno,
           _("closing output file %s"), quoteaf (output_file));
}

/* Process any pending signals.  If signals are caught, this function
   should be called periodically.  Ideally there should never be an
   unbounded amount of time when signals are not being processed.  */

ਸਥਿਰ ਖਾਲੀ
process_ਸੰਕੇਤ (ਖਾਲੀ)
{
  ਜਦੋਂ (ਰੁਕਾਵਟ_signal || info_signal_ਗਿਣਤੀ)
    {
      ਪੂਰਨ ਰੁਕਾਵਟ;
      ਪੂਰਨ infos;
      sigset_t oldset;

      sigprocmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &oldset);

      /* Reload interrupt_signal and info_signal_count, in case a new
         signal was handled before sigprocmask took effect.  */
      ਰੁਕਾਵਟ = ਰੁਕਾਵਟ_signal;
      infos = info_signal_ਗਿਣਤੀ;

      ਜੇ (infos)
        info_signal_ਗਿਣਤੀ = infos - 1;

      sigprocmask (SIG_SETMASK, &oldset, ਨਿਰਾ);

      ਜੇ (ਰੁਕਾਵਟ)
        cleanup ();
      print_stats ();
      ਜੇ (ਰੁਕਾਵਟ)
        ਉਠਾਓ (ਰੁਕਾਵਟ);
    }
}

ਸਥਿਰ ਖਾਲੀ
ਅੰਤ_ਉੱਪਰ (ਖਾਲੀ)
{
  /* Process signals first, so that cleanup is called at most once.  */
  process_ਸੰਕੇਤ ();
  cleanup ();
  print_stats ();
}

ਸਥਿਰ ਖਾਲੀ
quit (ਪੂਰਨ ਕੋਡ)
{
  ਅੰਤ_ਉੱਪਰ ();
  ਬਾਹਰ_ਨਿਕਲੋ (ਕੋਡ);
}

/* Return LEN rounded down to a multiple of IO_BUFSIZE
   (to minimize calls to the expensive posix_fadvise (,POSIX_FADV_DONTNEED),
   while storing the remainder internally per FD.
   Pass LEN == 0 to get the current remainder.  */

ਸਥਿਰ off_t
cache_ਗੇੜਾ (ਪੂਰਨ fd, off_t len)
{
  ਸਥਿਰ off_t i_pending, o_pending;
  off_t *pending = (fd == STDIN_FILENO ? &i_pending : &o_pending);

  ਜੇ (len)
    {
      intmax_t c_pending;
      ਜੇ (ckd_ਜੋੜੋ (&c_pending, *pending, len))
        c_pending = INTMAX_MAX;
      *pending = c_pending % IO_BUFSIZE;
      ਜੇ (c_pending > *pending)
        len = c_pending - *pending;
      ਨਹੀਂਤਾਂ
        len = 0;
    }
  ਨਹੀਂਤਾਂ
    len = *pending;

  ਵਾਪਸ len;
}

/* Discard the cache from the current offset of either
   STDIN_FILENO or STDOUT_FILENO.
   Return true on success.
   Return false on failure, with errno set.  */

ਸਥਿਰ bool
invalidate_cache (ਪੂਰਨ fd, off_t len)
{
  ਪੂਰਨ adv_ret = -1;
  off_t ਵਿੱਥ;
  bool nocache_eof = (fd == STDIN_FILENO ? i_nocache_eof : o_nocache_eof);

  /* Minimize syscalls.  */
  off_t clen = cache_ਗੇੜਾ (fd, len);
  ਜੇ (len && !clen)
    ਵਾਪਸ true; /* Don't advise this time.  */
  ਨਹੀਂਤਾਂ ਜੇ (! len && ! clen && ! nocache_eof)
    ਵਾਪਸ true;
  off_t pending = len ? cache_ਗੇੜਾ (fd, 0) : 0;

  ਜੇ (fd == STDIN_FILENO)
    {
      ਜੇ (ਨਿਵੇਸ਼_seekable)
        ਵਿੱਥ = ਨਿਵੇਸ਼_ਵਿੱਥ;
      ਨਹੀਂਤਾਂ
        {
          ਵਿੱਥ = -1;
          errno = ESPIPE;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ off_t output_ਵਿੱਥ = -2;

      ਜੇ (output_ਵਿੱਥ != -1)
        {
          ਜੇ (output_ਵਿੱਥ < 0)
            output_ਵਿੱਥ = lseek (fd, 0, SEEK_CUR);
          ਨਹੀਂਤਾਂ ਜੇ (len)
            output_ਵਿੱਥ += clen + pending;
        }

      ਵਿੱਥ = output_ਵਿੱਥ;
    }

  ਜੇ (0 <= ਵਿੱਥ)
   {
     ਜੇ (! len && clen && nocache_eof)
       {
         pending = clen;
         clen = 0;
       }

     /* Note we're being careful here to only invalidate what
        we've read, so as not to dump any read ahead cache.
        Note also the kernel is conservative and only invalidates
        full pages in the specified range.  */
#ਜੇ HAVE_POSIX_FADVISE
     ਵਿੱਥ = ਵਿੱਥ - clen - pending;
     /* ensure full page specified when invalidating to eof.  */
     ਜੇ (clen == 0)
       ਵਿੱਥ -= ਵਿੱਥ % ਪੰਨਾ_ਮਾਪ;
     adv_ret = posix_fadvise (fd, ਵਿੱਥ, clen, POSIX_FADV_DONTNEED);
     errno = adv_ret;
#ਨਹੀਂਤਾਂ
     errno = ENOTSUP;
#ਜੇ_ਅੰਤ
   }

  ਵਾਪਸ adv_ret == 0;
}

/* Read from FD into the buffer BUF of size SIZE, processing any
   signals that arrive before bytes are read.  Return the number of
   bytes read if successful, -1 (setting errno) on failure.  */

ਸਥਿਰ ssize_t
iread (ਪੂਰਨ fd, ਅੱਖਰ *buf, idx_t ਮਾਪ)
{
  ssize_t nread;
  ਸਥਿਰ ssize_t prev_nread;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      nread = ਪਠ (fd, buf, ਮਾਪ);
      /* Ignore final read error with iflag=direct as that
         returns EINVAL due to the non aligned file offset.  */
      ਜੇ (nread == -1 && errno == EINVAL
          && 0 < prev_nread && prev_nread < ਮਾਪ
          && (ਨਿਵੇਸ਼_flags & O_ਸਿੱਧਾ))
        {
          errno = 0;
          nread = 0;
        }
    }
  ਜਦੋਂ (nread < 0 && errno == EINTR);

  /* Short read may be due to received signal.  */
  ਜੇ (0 < nread && nread < ਮਾਪ)
    process_ਸੰਕੇਤ ();

  ਜੇ (0 < nread && warn_partial_read)
    {
      ਜੇ (0 < prev_nread && prev_nread < ਮਾਪ)
        {
          idx_t prev = prev_nread;
          ਜੇ (ਸਥਿਤੀ_level != ਸਥਿਤੀ_NONE)
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
  ਵਾਪਸ nread;
}

/* Wrapper around iread function to accumulate full blocks.  */
ਸਥਿਰ ssize_t
iread_fullblock (ਪੂਰਨ fd, ਅੱਖਰ *buf, idx_t ਮਾਪ)
{
  ssize_t nread = 0;

  ਜਦੋਂ (0 < ਮਾਪ)
    {
      ssize_t ncurr = iread (fd, buf, ਮਾਪ);
      ਜੇ (ncurr < 0)
        ਵਾਪਸ ncurr;
      ਜੇ (ncurr == 0)
        ਰੋਕੋ;
      nread += ncurr;
      buf   += ncurr;
      ਮਾਪ  -= ncurr;
    }

  ਵਾਪਸ nread;
}

/* Write to FD the buffer BUF of size SIZE, processing any signals
   that arrive.  Return the number of bytes written, setting errno if
   this is less than SIZE.  Keep trying if there are partial
   writes.  */

ਸਥਿਰ idx_t
iwrite (ਪੂਰਨ fd, ਅੱਖਰ ਅਚਲ *buf, idx_t ਮਾਪ)
{
  idx_t ਕੁੱਲ_written = 0;

  ਜੇ ((output_flags & O_ਸਿੱਧਾ) && ਮਾਪ < output_blocksize)
    {
      ਪੂਰਨ old_flags = fcntl (STDOUT_FILENO, F_GETFL);
      ਜੇ (fcntl (STDOUT_FILENO, F_SETFL, old_flags & ~O_ਸਿੱਧਾ) != 0
          && ਸਥਿਤੀ_level != ਸਥਿਤੀ_NONE)
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
      conversions_ਨਕਾਬ |= C_FSYNC;

      /* After the subsequent fsync we'll call invalidate_cache
         to attempt to clear all data from the page cache.  */
    }

  ਜਦੋਂ (ਕੁੱਲ_written < ਮਾਪ)
    {
      ssize_t nwritten = 0;
      process_ਸੰਕੇਤ ();

      /* Perform a seek for a NUL block if sparse output is enabled.  */
      final_op_was_seek = false;
      ਜੇ ((conversions_ਨਕਾਬ & C_SPARSE) && is_nul (buf, ਮਾਪ))
        {
          ਜੇ (lseek (fd, ਮਾਪ, SEEK_CUR) < 0)
            {
              conversions_ਨਕਾਬ &= ~C_SPARSE;
              /* Don't warn about the advisory sparse request.  */
            }
          ਨਹੀਂਤਾਂ
            {
              final_op_was_seek = true;
              nwritten = ਮਾਪ;
            }
        }

      ਜੇ (!nwritten)
        nwritten = ਲਿਖ (fd, buf + ਕੁੱਲ_written, ਮਾਪ - ਕੁੱਲ_written);

      ਜੇ (nwritten < 0)
        {
          ਜੇ (errno != EINTR)
            ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (nwritten == 0)
        {
          /* Some buggy drivers return 0 when one tries to write beyond
             a device's end.  (Example: Linux kernel 1.2.13 on /dev/fd0.)
             Set errno to ENOSPC so they get a sensible diagnostic.  */
          errno = ENOSPC;
          ਰੋਕੋ;
        }
      ਨਹੀਂਤਾਂ
        ਕੁੱਲ_written += nwritten;
    }

  ਜੇ (o_nocache && ਕੁੱਲ_written)
    invalidate_cache (fd, ਕੁੱਲ_written);

  ਵਾਪਸ ਕੁੱਲ_written;
}

/* Write, then empty, the output buffer 'obuf'. */

ਸਥਿਰ ਖਾਲੀ
write_output (ਖਾਲੀ)
{
  idx_t nwritten = iwrite (STDOUT_FILENO, obuf, output_blocksize);
  w_ਬਾਈਟਾਂ += nwritten;
  ਜੇ (nwritten != output_blocksize)
    {
      diagnose (errno, _("writing to %s"), quoteaf (output_file));
      ਜੇ (nwritten != 0)
        w_partial++;
      quit (EXIT_FAILURE);
    }
  ਨਹੀਂਤਾਂ
    w_ਭਰਿਆ++;
  oc = 0;
}

/* Restart on EINTR from fdatasync.  */

ਸਥਿਰ ਪੂਰਨ
ifdatasync (ਪੂਰਨ fd)
{
  ਪੂਰਨ ret;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      ret = fdatasync (fd);
    }
  ਜਦੋਂ (ret < 0 && errno == EINTR);

  ਵਾਪਸ ret;
}

/* Restart on EINTR from fd_reopen.  */

ਸਥਿਰ ਪੂਰਨ
ifd_reopen (ਪੂਰਨ desired_fd, ਅੱਖਰ ਅਚਲ *file, ਪੂਰਨ flag, mode_t ਬਹੁਲਕ_2)
{
  ਪੂਰਨ ret;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      ret = fd_reopen (desired_fd, file, flag, ਬਹੁਲਕ_2);
    }
  ਜਦੋਂ (ret < 0 && errno == EINTR);

  ਵਾਪਸ ret;
}

/* Restart on EINTR from fstat.  */

ਸਥਿਰ ਪੂਰਨ
ifstat (ਪੂਰਨ fd, ਬਣਤਰ stat *st)
{
  ਪੂਰਨ ret;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      ret = fstat (fd, st);
    }
  ਜਦੋਂ (ret < 0 && errno == EINTR);

  ਵਾਪਸ ret;
}

/* Restart on EINTR from fsync.  */

ਸਥਿਰ ਪੂਰਨ
ifsync (ਪੂਰਨ fd)
{
  ਪੂਰਨ ret;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      ret = fsync (fd);
    }
  ਜਦੋਂ (ret < 0 && errno == EINTR);

  ਵਾਪਸ ret;
}

/* Restart on EINTR from ftruncate.  */

ਸਥਿਰ ਪੂਰਨ
iftruncate (ਪੂਰਨ fd, off_t ਲੰਮਾਈ)
{
  ਪੂਰਨ ret;

  ਕਰੋ
    {
      process_ਸੰਕੇਤ ();
      ret = ftruncate (fd, ਲੰਮਾਈ);
    }
  ਜਦੋਂ (ret < 0 && errno == EINTR);

  ਵਾਪਸ ret;
}

/* Return true if STR is of the form "PATTERN" or "PATTERNDELIM...".  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
ਸੰਕਾਰਜ_matches (ਅੱਖਰ ਅਚਲ *str, ਅੱਖਰ ਅਚਲ *ਨਮੂਨਾ_2, ਅੱਖਰ delim)
{
  ਜਦੋਂ (*ਨਮੂਨਾ_2)
    ਜੇ (*str++ != *ਨਮੂਨਾ_2++)
      ਵਾਪਸ false;
  ਵਾਪਸ !*str || *str == delim;
}

/* Interpret one "conv=..." or similar operand STR according to the
   symbols in TABLE, returning the flags specified.  If the operand
   cannot be parsed, use ERROR_MSGID to generate a diagnostic.  */

ਸਥਿਰ ਪੂਰਨ
parse_symbols (ਅੱਖਰ ਅਚਲ *str, ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ ਅਚਲ *ਸਾਰਣੀ,
               bool exclusive, ਅੱਖਰ ਅਚਲ *error_msgid)
{
  ਪੂਰਨ ਮੁੱਲ = 0;

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ ਅਚਲ *strcomma = strchr (str, ',');
      ਬਣਤਰ ਚਿੰਨ੍ਹ_ਮੁੱਲ ਅਚਲ *ਇੰਦਰਾਜ;

      ਲਈ (ਇੰਦਰਾਜ = ਸਾਰਣੀ;
           ! (ਸੰਕਾਰਜ_matches (str, ਇੰਦਰਾਜ->ਚਿੰਨ੍ਹ, ',') && ਇੰਦਰਾਜ->ਮੁੱਲ);
           ਇੰਦਰਾਜ++)
        {
          ਜੇ (! ਇੰਦਰਾਜ->ਚਿੰਨ੍ਹ[0])
            {
              idx_t slen = strcomma ? strcomma - str : ਲੜੀ_ਲੰਬਾਈ (str);
              diagnose (0, "%s: %s", _(error_msgid),
                        quotearg_n_style_mem (0, locale_quoting_style,
                                              str, slen));
              usage (EXIT_FAILURE);
            }
        }

      ਜੇ (exclusive)
        ਮੁੱਲ = ਇੰਦਰਾਜ->ਮੁੱਲ;
      ਨਹੀਂਤਾਂ
        ਮੁੱਲ |= ਇੰਦਰਾਜ->ਮੁੱਲ;
      ਜੇ (!strcomma)
        ਰੋਕੋ;
      str = strcomma + 1;
    }

  ਵਾਪਸ ਮੁੱਲ;
}

/* Return the value of STR, interpreted as a non-negative decimal integer,
   optionally multiplied by various values.
   Set *INVALID to an appropriate error value and return INTMAX_MAX if
   it is an overflow, an indeterminate value if some other error occurred.  */

ਸਥਿਰ intmax_t
parse_integer (ਅੱਖਰ ਅਚਲ *str, strtol_error *invalid)
{
  /* Call xstrtoumax, not xstrtoimax, since we don't want to
     allow strings like " -0".  Initialize N to an indeterminate value;
     calling code should not rely on this function returning 0
     when *INVALID represents a non-overflow error.  */
  ਪੂਰਨ indeterminate = 0;
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ suffixes[] = "bcEGkKMPQRTwYZ0";
  intmax_t ਨਤੀਜਾ = 1;
  bool ਉਛਾਲ = false;
  bool warn_zero_multiplier = false;
  strtol_error e;

  ਜਦੋਂ (true)
    {
      uintmax_t n = indeterminate;
      ਅੱਖਰ *suffix;
      e = xstrtoumax (str, &suffix, 10, &n, suffixes);

      ਜੇ ((e & ~LONGINT_ਉਛਾਲ) == LONGINT_INVALID_SUFFIX_CHAR
          && *suffix == 'B' && str < suffix && suffix[-1] != 'B')
        {
          suffix++;
          ਜੇ (!*suffix)
            e &= ~LONGINT_INVALID_SUFFIX_CHAR;
        }

      bool multiply
        = ((e & ~LONGINT_ਉਛਾਲ) == LONGINT_INVALID_SUFFIX_CHAR
           && *suffix == 'x');
      ਜੇ (multiply)
        e &= ~LONGINT_INVALID_SUFFIX_CHAR;

      ਜੇ ((e & ~LONGINT_ਉਛਾਲ) != LONGINT_OK)
        {
          ਜੇ (! (e & LONGINT_ਉਛਾਲ) && n <= INTMAX_MAX)
            {
              *invalid = e;
              ਵਾਪਸ indeterminate;
            }
          e = LONGINT_ਉਛਾਲ;
        }

      ਜੇ (n == 0)
        ਨਤੀਜਾ = 0;
      ਨਹੀਂਤਾਂ ਜੇ (ਨਤੀਜਾ != 0)
        {
          intmax_t ਗੁਣਨਫਲ;
          ਜੇ ((e & LONGINT_ਉਛਾਲ)
              || ਉਛਾਲ
              || ckd_mul (&ਗੁਣਨਫਲ, ਨਤੀਜਾ, n))
            ਉਛਾਲ = true;
          ਨਹੀਂਤਾਂ
            ਨਤੀਜਾ = ਗੁਣਨਫਲ;
        }

      ਜੇ (multiply && STRPREFIX (str, "0x"))
        warn_zero_multiplier = true;

      ਜੇ (! multiply)
        ਰੋਕੋ;
      str = suffix + 1;
    }

  ਜੇ (ਨਤੀਜਾ == 0)
    {
      ਜੇ (warn_zero_multiplier)
        diagnose (0, _("warning: %s is a zero multiplier; "
                       "use %s if that is intended"),
                  quote_n (0, "0x"), quote_n (1, "00x"));
      e = LONGINT_OK;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਉਛਾਲ)
    {
      e = LONGINT_ਉਛਾਲ;
      ਨਤੀਜਾ = INTMAX_MAX;
    }

  *invalid = e;
  ਵਾਪਸ ਨਤੀਜਾ;
}

/* OPERAND is of the form "X=...".  Return true if X is NAME.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
ਸੰਕਾਰਜ_is (ਅੱਖਰ ਅਚਲ *ਸੰਕਾਰਜ_2, ਅੱਖਰ ਅਚਲ *name)
{
  ਵਾਪਸ ਸੰਕਾਰਜ_matches (ਸੰਕਾਰਜ_2, name, '=');
}

ਸਥਿਰ ਖਾਲੀ
scanargs (ਪੂਰਨ argc, ਅੱਖਰ *ਅਚਲ *argv)
{
  idx_t blocksize = 0;
  intmax_t ਗਿਣਤੀ_2 = INTMAX_MAX;
  intmax_t skip = 0;
  intmax_t seek = 0;
  bool ਗਿਣਤੀ_B = false, skip_B = false, seek_B = false;

  ਲਈ (ਪੂਰਨ i = optind; i < argc; i++)
    {
      ਅੱਖਰ ਅਚਲ *name = argv[i];
      ਅੱਖਰ ਅਚਲ *val = strchr (name, '=');

      ਜੇ (val == ਨਿਰਾ)
        {
          diagnose (0, _("unrecognized operand %s"), quoteaf (name));
          usage (EXIT_FAILURE);
        }
      val++;

      ਜੇ (ਸੰਕਾਰਜ_is (name, "if"))
        ਨਿਵੇਸ਼_file = val;
      ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "of"))
        output_file = val;
      ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "conv"))
        conversions_ਨਕਾਬ |= parse_symbols (val, conversions, false,
                                           N_("invalid conversion"));
      ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "iflag"))
        ਨਿਵੇਸ਼_flags |= parse_symbols (val, flags, false,
                                      N_("invalid input flag"));
      ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "oflag"))
        output_flags |= parse_symbols (val, flags, false,
                                       N_("invalid output flag"));
      ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "status"))
        ਸਥਿਤੀ_level = parse_symbols (val, statuses, true,
                                      N_("invalid status level"));
      ਨਹੀਂਤਾਂ
        {
          strtol_error invalid = LONGINT_OK;
          intmax_t n = parse_integer (val, &invalid);
          bool has_B = !!strchr (val, 'B');
          intmax_t n_min = 0;
          intmax_t n_max = INTMAX_MAX;
          idx_t *converted_idx = ਨਿਰਾ;

          /* Maximum blocksize.  Keep it smaller than IDX_MAX, so that
             it fits into blocksize vars even if 1 is added for conv=swab.
             Do not exceed SSIZE_MAX, for the benefit of system calls
             like "read".  And do not exceed OFF_T_MAX, for the
             benefit of the large-offset seek code.  */
          idx_t max_blocksize = MIN (IDX_MAX - 1, MIN (SSIZE_MAX, OFF_T_MAX));

          ਜੇ (ਸੰਕਾਰਜ_is (name, "ibs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &ਨਿਵੇਸ਼_blocksize;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "obs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &output_blocksize;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "bs"))
            {
              n_min = 1;
              n_max = max_blocksize;
              converted_idx = &blocksize;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "cbs"))
            {
              n_min = 1;
              n_max = MIN (SIZE_MAX, IDX_MAX);
              converted_idx = &conversion_blocksize;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "skip") || ਸੰਕਾਰਜ_is (name, "iseek"))
            {
              skip = n;
              skip_B = has_B;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name + (*name == 'o'), "seek"))
            {
              seek = n;
              seek_B = has_B;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਸੰਕਾਰਜ_is (name, "count"))
            {
              ਗਿਣਤੀ_2 = n;
              ਗਿਣਤੀ_B = has_B;
            }
          ਨਹੀਂਤਾਂ
            {
              diagnose (0, _("unrecognized operand %s"), quoteaf (name));
              usage (EXIT_FAILURE);
            }

          ਜੇ (n < n_min)
            invalid = LONGINT_INVALID;
          ਨਹੀਂਤਾਂ ਜੇ (n_max < n)
            invalid = LONGINT_ਉਛਾਲ;

          ਜੇ (invalid != LONGINT_OK)
            error (EXIT_FAILURE, invalid == LONGINT_ਉਛਾਲ ? EOVERFLOW : 0,
                   "%s: %s", _("invalid number"), quoteaf (val));
          ਨਹੀਂਤਾਂ ਜੇ (converted_idx)
            *converted_idx = n;
        }
    }

  ਜੇ (blocksize)
    ਨਿਵੇਸ਼_blocksize = output_blocksize = blocksize;
  ਨਹੀਂਤਾਂ
    {
      /* POSIX says dd aggregates partial reads into
         output_blocksize if bs= is not specified.  */
      conversions_ਨਕਾਬ |= C_TWOBUFS;
    }

  ਜੇ (ਨਿਵੇਸ਼_blocksize == 0)
    ਨਿਵੇਸ਼_blocksize = DEFAULT_BLOCKSIZE;
  ਜੇ (output_blocksize == 0)
    output_blocksize = DEFAULT_BLOCKSIZE;
  ਜੇ (conversion_blocksize == 0)
    conversions_ਨਕਾਬ &= ~(C_ਖੰਡ | C_UNBLOCK);

  ਜੇ (ਨਿਵੇਸ਼_flags & (O_DSYNC | O_SYNC))
    ਨਿਵੇਸ਼_flags |= O_RSYNC;

  ਜੇ (output_flags & O_FULLBLOCK)
    {
      diagnose (0, "%s: %s", _("invalid output flag"), quote ("fullblock"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (skip_B)
    ਨਿਵੇਸ਼_flags |= O_SKIP_ਬਾਈਟਾਂ;
  ਜੇ (ਨਿਵੇਸ਼_flags & O_SKIP_ਬਾਈਟਾਂ && skip != 0)
    {
      skip_ਰਿਕਾਰਡਾਂ = skip / ਨਿਵੇਸ਼_blocksize;
      skip_ਬਾਈਟਾਂ = skip % ਨਿਵੇਸ਼_blocksize;
    }
  ਨਹੀਂਤਾਂ ਜੇ (skip != 0)
    skip_ਰਿਕਾਰਡਾਂ = skip;

  ਜੇ (ਗਿਣਤੀ_B)
    ਨਿਵੇਸ਼_flags |= O_ਗਿਣਤੀ_ਬਾਈਟਾਂ;
  ਜੇ (ਨਿਵੇਸ਼_flags & O_ਗਿਣਤੀ_ਬਾਈਟਾਂ && ਗਿਣਤੀ_2 != INTMAX_MAX)
    {
      max_ਰਿਕਾਰਡਾਂ = ਗਿਣਤੀ_2 / ਨਿਵੇਸ਼_blocksize;
      max_ਬਾਈਟਾਂ = ਗਿਣਤੀ_2 % ਨਿਵੇਸ਼_blocksize;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 != INTMAX_MAX)
    max_ਰਿਕਾਰਡਾਂ = ਗਿਣਤੀ_2;

  ਜੇ (seek_B)
    output_flags |= O_SEEK_ਬਾਈਟਾਂ;
  ਜੇ (output_flags & O_SEEK_ਬਾਈਟਾਂ && seek != 0)
    {
      seek_ਰਿਕਾਰਡਾਂ = seek / output_blocksize;
      seek_ਬਾਈਟਾਂ = seek % output_blocksize;
    }
  ਨਹੀਂਤਾਂ ਜੇ (seek != 0)
    seek_ਰਿਕਾਰਡਾਂ = seek;

  /* Warn about partial reads if bs=SIZE is given and iflag=fullblock
     is not, and if counting or skipping bytes or using direct I/O.
     This helps to avoid confusion with miscounts, and to avoid issues
     with direct I/O on GNU/Linux.  */
  warn_partial_read =
    (! (conversions_ਨਕਾਬ & C_TWOBUFS) && ! (ਨਿਵੇਸ਼_flags & O_FULLBLOCK)
     && (skip_ਰਿਕਾਰਡਾਂ
         || (0 < max_ਰਿਕਾਰਡਾਂ && max_ਰਿਕਾਰਡਾਂ < INTMAX_MAX)
         || (ਨਿਵੇਸ਼_flags | output_flags) & O_ਸਿੱਧਾ));

  iread_fnc = ((ਨਿਵੇਸ਼_flags & O_FULLBLOCK)
               ? iread_fullblock
               : iread);
  ਨਿਵੇਸ਼_flags &= ~O_FULLBLOCK;

  ਜੇ (ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (conversions_ਨਕਾਬ & (C_ASCII | C_EBCDIC | C_IBM)))
    error (EXIT_FAILURE, 0, _("cannot combine any two of {ascii,ebcdic,ibm}"));
  ਜੇ (ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (conversions_ਨਕਾਬ & (C_ਖੰਡ | C_UNBLOCK)))
    error (EXIT_FAILURE, 0, _("cannot combine block and unblock"));
  ਜੇ (ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (conversions_ਨਕਾਬ & (C_LCASE | C_UCASE)))
    error (EXIT_FAILURE, 0, _("cannot combine lcase and ucase"));
  ਜੇ (ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (conversions_ਨਕਾਬ & (C_EXCL | C_NOCREAT)))
    error (EXIT_FAILURE, 0, _("cannot combine excl and nocreat"));
  ਜੇ (ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (ਨਿਵੇਸ਼_flags & (O_ਸਿੱਧਾ | O_NOCACHE))
      || ਗੁਣਜ_ਬਿੱਟ_ਸੈੱਟ_2 (output_flags & (O_ਸਿੱਧਾ | O_NOCACHE)))
    error (EXIT_FAILURE, 0, _("cannot combine direct and nocache"));

  ਜੇ (ਨਿਵੇਸ਼_flags & O_NOCACHE)
    {
      i_nocache = true;
      i_nocache_eof = (max_ਰਿਕਾਰਡਾਂ == 0 && max_ਬਾਈਟਾਂ == 0);
      ਨਿਵੇਸ਼_flags &= ~O_NOCACHE;
    }
  ਜੇ (output_flags & O_NOCACHE)
    {
      o_nocache = true;
      o_nocache_eof = (max_ਰਿਕਾਰਡਾਂ == 0 && max_ਬਾਈਟਾਂ == 0);
      output_flags &= ~O_NOCACHE;
    }
}

/* Fix up translation table. */

ਸਥਿਰ ਖਾਲੀ
apply_translations (ਖਾਲੀ)
{
  ਜੇ (conversions_ਨਕਾਬ & C_ASCII)
    translate_charset (ebcdic_ਮੰਜ਼ਿਲ_ascii);

  ਜੇ (conversions_ਨਕਾਬ & C_UCASE)
    {
      ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
        trans_ਸਾਰਣੀ[i] = ਵੱਡਾ_ਅੱਖਰ (trans_ਸਾਰਣੀ[i]);
      translation_needed = true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (conversions_ਨਕਾਬ & C_LCASE)
    {
      ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
        trans_ਸਾਰਣੀ[i] = tolower (trans_ਸਾਰਣੀ[i]);
      translation_needed = true;
    }

  ਜੇ (conversions_ਨਕਾਬ & C_EBCDIC)
    {
      translate_charset (ascii_ਮੰਜ਼ਿਲ_ebcdic);
      newline_character = ascii_ਮੰਜ਼ਿਲ_ebcdic['\n'];
      space_character = ascii_ਮੰਜ਼ਿਲ_ebcdic[' '];
    }
  ਨਹੀਂਤਾਂ ਜੇ (conversions_ਨਕਾਬ & C_IBM)
    {
      translate_charset (ascii_ਮੰਜ਼ਿਲ_ibm);
      newline_character = ascii_ਮੰਜ਼ਿਲ_ibm['\n'];
      space_character = ascii_ਮੰਜ਼ਿਲ_ibm[' '];
    }
}

/* Apply the character-set translations specified by the user
   to the NREAD bytes in BUF.  */

ਸਥਿਰ ਖਾਲੀ
translate_ਬਫਰ (ਅੱਖਰ *buf, idx_t nread)
{
  ਅੱਖਰ *cp = buf;
  ਲਈ (idx_t i = nread; i; i--, cp++)
    *cp = trans_ਸਾਰਣੀ[ਮੰਜ਼ਿਲ_uchar (*cp)];
}

/* Swap *NREAD bytes in BUF, which should have room for an extra byte
   after the end because the swapping is not in-place.  If *SAVED_BYTE
   is nonnegative, also swap that initial byte from the previous call.
   Save the last byte into into *SAVED_BYTE if needed to make the
   resulting *NREAD even, and set *SAVED_BYTE to -1 otherwise.
   Return the buffer's adjusted start, either BUF or BUF + 1.  */

ਸਥਿਰ ਅੱਖਰ *
swab_ਬਫਰ (ਅੱਖਰ *buf, idx_t *nread, ਪੂਰਨ *saved_byte)
{
  ਜੇ (*nread == 0)
    ਵਾਪਸ buf;

  /* Update *SAVED_BYTE, and set PREV_SAVED to its old value.  */
  ਪੂਰਨ prev_saved = *saved_byte;
  ਜੇ ((prev_saved < 0) == (*nread & 1))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = buf[--*nread];
      *saved_byte = c;
    }
  ਨਹੀਂਤਾਂ
    *saved_byte = -1;

  /* Do the byte-swapping by moving every other byte two
     positions toward the end, working from the end of the buffer
     toward the beginning.  This way we move only half the data.  */
  ਲਈ (idx_t i = *nread; 1 < i; i -= 2)
    buf[i] = buf[i - 2];

  ਜੇ (prev_saved < 0)
    ਵਾਪਸ buf + 1;

  buf[1] = prev_saved;
  ++*nread;
  ਵਾਪਸ buf;
}

/* Add OFFSET to the input offset, setting the overflow flag if
   necessary.  */

ਸਥਿਰ ਖਾਲੀ
advance_ਨਿਵੇਸ਼_ਵਿੱਥ (intmax_t ਵਿੱਥ)
{
  ਜੇ (0 <= ਨਿਵੇਸ਼_ਵਿੱਥ && ckd_ਜੋੜੋ (&ਨਿਵੇਸ਼_ਵਿੱਥ, ਨਿਵੇਸ਼_ਵਿੱਥ, ਵਿੱਥ))
    ਨਿਵੇਸ਼_ਵਿੱਥ = -1;
}

/* Throw away RECORDS blocks of BLOCKSIZE bytes plus BYTES bytes on
   file descriptor FDESC, which is open with read permission for FILE.
   Store up to BLOCKSIZE bytes of the data at a time in IBUF or OBUF, if
   necessary. RECORDS or BYTES must be nonzero. If FDESC is
   STDIN_FILENO, advance the input offset. Return the number of
   records remaining, i.e., that were not skipped because EOF was
   reached.  If FDESC is STDOUT_FILENO, on return, BYTES is the
   remaining bytes in addition to the remaining records.  */

ਸਥਿਰ intmax_t
skip (ਪੂਰਨ fdesc, ਅੱਖਰ ਅਚਲ *file, intmax_t ਰਿਕਾਰਡਾਂ, idx_t blocksize,
      idx_t *ਬਾਈਟਾਂ)
{
  /* Try lseek and if an error indicates it was an inappropriate operation --
     or if the file offset is not representable as an off_t --
     fall back on using read.  */

  errno = 0;
  off_t ਵਿੱਥ;
  ਜੇ (! ckd_mul (&ਵਿੱਥ, ਰਿਕਾਰਡਾਂ, blocksize)
      && ! ckd_ਜੋੜੋ (&ਵਿੱਥ, ਵਿੱਥ, *ਬਾਈਟਾਂ)
      && 0 <= lseek (fdesc, ਵਿੱਥ, SEEK_CUR))
    {
      ਜੇ (fdesc == STDIN_FILENO)
        {
           ਬਣਤਰ stat st;
           ਜੇ (ifstat (STDIN_FILENO, &st) != 0)
             error (EXIT_FAILURE, errno, _("cannot fstat %s"), quoteaf (file));
           ਜੇ (usable_st_ਮਾਪ (&st) && 0 < st.st_ਮਾਪ && 0 <= ਨਿਵੇਸ਼_ਵਿੱਥ
               && st.st_ਮਾਪ - ਨਿਵੇਸ਼_ਵਿੱਥ < ਵਿੱਥ)
             {
               /* When skipping past EOF, return the number of _full_ blocks
                * that are not skipped, and set offset to EOF, so the caller
                * can determine the requested skip was not satisfied.  */
               ਰਿਕਾਰਡਾਂ = ( ਵਿੱਥ - st.st_ਮਾਪ ) / blocksize;
               ਵਿੱਥ = st.st_ਮਾਪ - ਨਿਵੇਸ਼_ਵਿੱਥ;
             }
           ਨਹੀਂਤਾਂ
             ਰਿਕਾਰਡਾਂ = 0;
           advance_ਨਿਵੇਸ਼_ਵਿੱਥ (ਵਿੱਥ);
        }
      ਨਹੀਂਤਾਂ
        {
          ਰਿਕਾਰਡਾਂ = 0;
          *ਬਾਈਟਾਂ = 0;
        }
      ਵਾਪਸ ਰਿਕਾਰਡਾਂ;
    }
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ lseek_errno = errno;

      /* The seek request may have failed above if it was too big
         (> device size, > max file size, etc.)
         Or it may not have been done at all (> OFF_T_MAX).
         Therefore try to seek to the end of the file,
         to avoid redundant reading.  */
      ਜੇ (lseek (fdesc, 0, SEEK_END) >= 0)
        {
          /* File is seekable, and we're at the end of it, and
             size <= OFF_T_MAX. So there's no point using read to advance.  */

          ਜੇ (!lseek_errno)
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

      ਅੱਖਰ *buf;
      ਜੇ (fdesc == STDIN_FILENO)
        {
          alloc_ibuf ();
          buf = ibuf;
        }
      ਨਹੀਂਤਾਂ
        {
          alloc_obuf ();
          buf = obuf;
        }

      ਕਰੋ
        {
          ssize_t nread = iread_fnc (fdesc, buf, ਰਿਕਾਰਡਾਂ ? blocksize : *ਬਾਈਟਾਂ);
          ਜੇ (nread < 0)
            {
              ਜੇ (fdesc == STDIN_FILENO)
                {
                  diagnose (errno, _("error reading %s"), quoteaf (file));
                  ਜੇ (conversions_ਨਕਾਬ & C_NOERROR)
                    print_stats ();
                }
              ਨਹੀਂਤਾਂ
                diagnose (lseek_errno, _("%s: cannot seek"), quotef (file));
              quit (EXIT_FAILURE);
            }
          ਨਹੀਂਤਾਂ ਜੇ (nread == 0)
            ਰੋਕੋ;
          ਨਹੀਂਤਾਂ ਜੇ (fdesc == STDIN_FILENO)
            advance_ਨਿਵੇਸ਼_ਵਿੱਥ (nread);

          ਜੇ (ਰਿਕਾਰਡਾਂ != 0)
            ਰਿਕਾਰਡਾਂ--;
          ਨਹੀਂਤਾਂ
            *ਬਾਈਟਾਂ = 0;
        }
      ਜਦੋਂ (ਰਿਕਾਰਡਾਂ || *ਬਾਈਟਾਂ);

      ਵਾਪਸ ਰਿਕਾਰਡਾਂ;
    }
}

/* Advance the input by NBYTES if possible, after a read error.
   The input file offset may or may not have advanced after the failed
   read; adjust it to point just after the bad record regardless.
   Return true if successful, or if the input is already known to not
   be seekable.  */

ਸਥਿਰ bool
advance_ਨਿਵੇਸ਼_after_read_error (idx_t nbytes)
{
  ਜੇ (! ਨਿਵੇਸ਼_seekable)
    {
      ਜੇ (ਨਿਵੇਸ਼_seek_errno == ESPIPE)
        ਵਾਪਸ true;
      errno = ਨਿਵੇਸ਼_seek_errno;
    }
  ਨਹੀਂਤਾਂ
    {
      off_t ਵਿੱਥ;
      advance_ਨਿਵੇਸ਼_ਵਿੱਥ (nbytes);
      ਜੇ (ਨਿਵੇਸ਼_ਵਿੱਥ < 0)
        {
          diagnose (0, _("offset overflow while reading file %s"),
                    quoteaf (ਨਿਵੇਸ਼_file));
          ਵਾਪਸ false;
        }
      ਵਿੱਥ = lseek (STDIN_FILENO, 0, SEEK_CUR);
      ਜੇ (0 <= ਵਿੱਥ)
        {
          off_t diff;
          ਜੇ (ਵਿੱਥ == ਨਿਵੇਸ਼_ਵਿੱਥ)
            ਵਾਪਸ true;
          diff = ਨਿਵੇਸ਼_ਵਿੱਥ - ਵਿੱਥ;
          ਜੇ (! (0 <= diff && diff <= nbytes) && ਸਥਿਤੀ_level != ਸਥਿਤੀ_NONE)
            diagnose (0, _("warning: invalid file offset after failed read"));
          ਜੇ (0 <= lseek (STDIN_FILENO, diff, SEEK_CUR))
            ਵਾਪਸ true;
          ਜੇ (errno == 0)
            diagnose (0, _("cannot work around kernel bug after all"));
        }
    }

  diagnose (errno, _("%s: cannot seek"), quotef (ਨਿਵੇਸ਼_file));
  ਵਾਪਸ false;
}

/* Copy NREAD bytes of BUF, with no conversions.  */

ਸਥਿਰ ਖਾਲੀ
copy_simple (ਅੱਖਰ ਅਚਲ *buf, idx_t nread)
{
  ਅੱਖਰ ਅਚਲ *ਸ਼ੁਰੂ = buf;	/* First uncopied char in BUF.  */

  ਕਰੋ
    {
      idx_t nfree = MIN (nread, output_blocksize - oc);

      ਸਮ੍ਰਿਤੀ_ਨਕਲ (obuf + oc, ਸ਼ੁਰੂ, nfree);

      nread -= nfree;		/* Update the number of bytes left to copy. */
      ਸ਼ੁਰੂ += nfree;
      oc += nfree;
      ਜੇ (oc >= output_blocksize)
        write_output ();
    }
  ਜਦੋਂ (nread != 0);
}

/* Copy NREAD bytes of BUF, doing conv=block
   (pad newline-terminated records to 'conversion_blocksize',
   replacing the newline with trailing spaces).  */

ਸਥਿਰ ਖਾਲੀ
copy_with_ਖੰਡ (ਅੱਖਰ ਅਚਲ *buf, idx_t nread)
{
  ਲਈ (idx_t i = nread; i; i--, buf++)
    {
      ਜੇ (*buf == newline_character)
        {
          ਜੇ (ਖਾਨਾ < conversion_blocksize)
            {
              ਲਈ (idx_t j = ਖਾਨਾ; j < conversion_blocksize; j++)
                output_char (space_character);
            }
          ਖਾਨਾ = 0;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (ਖਾਨਾ == conversion_blocksize)
            r_truncate++;
          ਨਹੀਂਤਾਂ ਜੇ (ਖਾਨਾ < conversion_blocksize)
            output_char (*buf);
          ਖਾਨਾ++;
        }
    }
}

/* Copy NREAD bytes of BUF, doing conv=unblock
   (replace trailing spaces in 'conversion_blocksize'-sized records
   with a newline).  */

ਸਥਿਰ ਖਾਲੀ
copy_with_unblock (ਅੱਖਰ ਅਚਲ *buf, idx_t nread)
{
  ਸਥਿਰ idx_t pending_spaces = 0;

  ਲਈ (idx_t i = 0; i < nread; i++)
    {
      ਅੱਖਰ c = buf[i];

      ਜੇ (ਖਾਨਾ++ >= conversion_blocksize)
        {
          ਖਾਨਾ = pending_spaces = 0; /* Wipe out any pending spaces.  */
          i--;			/* Push the char back; get it later. */
          output_char (newline_character);
        }
      ਨਹੀਂਤਾਂ ਜੇ (c == space_character)
        pending_spaces++;
      ਨਹੀਂਤਾਂ
        {
          /* 'c' is the character after a run of spaces that were not
             at the end of the conversion buffer.  Output them.  */
          ਜਦੋਂ (pending_spaces)
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

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_fd_flags (ਪੂਰਨ fd, ਪੂਰਨ ਜੋੜੋ_flags, ਅੱਖਰ ਅਚਲ *name)
{
  /* Ignore file creation flags that are no-ops on file descriptors.  */
  ਜੋੜੋ_flags &= ~ (O_NOCTTY | O_NOFOLLOW);

  ਜੇ (ਜੋੜੋ_flags)
    {
      ਪੂਰਨ old_flags = fcntl (fd, F_GETFL);
      ਪੂਰਨ new_flags = old_flags | ਜੋੜੋ_flags;
      bool ok = true;
      ਜੇ (old_flags < 0)
        ok = false;
      ਨਹੀਂਤਾਂ ਜੇ (old_flags != new_flags)
        {
          ਜੇ (new_flags & (O_DIRECTORY | O_NOLINKS))
            {
              /* NEW_FLAGS contains at least one file creation flag that
                 requires some checking of the open file descriptor.  */
              ਬਣਤਰ stat st;
              ਜੇ (ifstat (fd, &st) != 0)
                ok = false;
              ਨਹੀਂਤਾਂ ਜੇ ((new_flags & O_DIRECTORY) && ! S_ISDIR (st.st_ਬਹੁਲਕ))
                {
                  errno = ENOTDIR;
                  ok = false;
                }
              ਨਹੀਂਤਾਂ ਜੇ ((new_flags & O_NOLINKS) && 1 < st.st_nlink)
                {
                  errno = EMLINK;
                  ok = false;
                }
              new_flags &= ~ (O_DIRECTORY | O_NOLINKS);
            }

          ਜੇ (ok && old_flags != new_flags
              && fcntl (fd, F_SETFL, new_flags) == -1)
            ok = false;
        }

      ਜੇ (!ok)
        error (EXIT_FAILURE, errno, _("setting flags for %s"), quoteaf (name));
    }
}

/* The main loop.  */

ਸਥਿਰ ਪੂਰਨ
dd_copy (ਖਾਲੀ)
{
  ਅੱਖਰ *bufstart;		/* Input buffer. */
  ssize_t nread;		/* Bytes read in the current block.  */

  /* If nonzero, then the previously read block was partial and
     PARTREAD was its size.  */
  idx_t partread = 0;

  ਪੂਰਨ exit_ਸਥਿਤੀ = EXIT_SUCCESS;
  idx_t n_ਬਾਈਟਾਂ_read;

  ਜੇ (skip_ਰਿਕਾਰਡਾਂ != 0 || skip_ਬਾਈਟਾਂ != 0)
    {
      intmax_t us_ਬਾਈਟਾਂ;
      bool us_ਬਾਈਟਾਂ_ਉਛਾਲ =
        (ckd_mul (&us_ਬਾਈਟਾਂ, skip_ਰਿਕਾਰਡਾਂ, ਨਿਵੇਸ਼_blocksize)
         || ckd_ਜੋੜੋ (&us_ਬਾਈਟਾਂ, skip_ਬਾਈਟਾਂ, us_ਬਾਈਟਾਂ));
      off_t ਨਿਵੇਸ਼_offset0 = ਨਿਵੇਸ਼_ਵਿੱਥ;
      intmax_t us_ਖੰਡਾਂ = skip (STDIN_FILENO, ਨਿਵੇਸ਼_file,
                                 skip_ਰਿਕਾਰਡਾਂ, ਨਿਵੇਸ਼_blocksize, &skip_ਬਾਈਟਾਂ);

      /* POSIX doesn't say what to do when dd detects it has been
         asked to skip past EOF, so I assume it's non-fatal.
         There are 3 reasons why there might be unskipped blocks/bytes:
             1. file is too small
             2. pipe has not enough data
             3. partial reads  */
      ਜੇ ((us_ਖੰਡਾਂ
           || (0 <= ਨਿਵੇਸ਼_ਵਿੱਥ
               && (us_ਬਾਈਟਾਂ_ਉਛਾਲ
                   || us_ਬਾਈਟਾਂ != ਨਿਵੇਸ਼_ਵਿੱਥ - ਨਿਵੇਸ਼_offset0)))
          && ਸਥਿਤੀ_level != ਸਥਿਤੀ_NONE)
        {
          diagnose (0, _("%s: cannot skip to specified offset"),
                    quotef (ਨਿਵੇਸ਼_file));
        }
    }

  ਜੇ (seek_ਰਿਕਾਰਡਾਂ != 0 || seek_ਬਾਈਟਾਂ != 0)
    {
      idx_t ਬਾਈਟਾਂ = seek_ਬਾਈਟਾਂ;
      intmax_t write_ਰਿਕਾਰਡਾਂ = skip (STDOUT_FILENO, output_file,
                                      seek_ਰਿਕਾਰਡਾਂ, output_blocksize, &ਬਾਈਟਾਂ);

      ਜੇ (write_ਰਿਕਾਰਡਾਂ != 0 || ਬਾਈਟਾਂ != 0)
        {
          memset (obuf, 0, write_ਰਿਕਾਰਡਾਂ ? output_blocksize : ਬਾਈਟਾਂ);

          ਕਰੋ
            {
              idx_t ਮਾਪ = write_ਰਿਕਾਰਡਾਂ ? output_blocksize : ਬਾਈਟਾਂ;
              ਜੇ (iwrite (STDOUT_FILENO, obuf, ਮਾਪ) != ਮਾਪ)
                {
                  diagnose (errno, _("writing to %s"), quoteaf (output_file));
                  quit (EXIT_FAILURE);
                }

              ਜੇ (write_ਰਿਕਾਰਡਾਂ != 0)
                write_ਰਿਕਾਰਡਾਂ--;
              ਨਹੀਂਤਾਂ
                ਬਾਈਟਾਂ = 0;
            }
          ਜਦੋਂ (write_ਰਿਕਾਰਡਾਂ || ਬਾਈਟਾਂ);
        }
    }

  ਜੇ (max_ਰਿਕਾਰਡਾਂ == 0 && max_ਬਾਈਟਾਂ == 0)
    ਵਾਪਸ exit_ਸਥਿਤੀ;

  alloc_ibuf ();
  alloc_obuf ();
  ਪੂਰਨ saved_byte = -1;

  ਜਦੋਂ (true)
    {
      ਜੇ (ਸਥਿਤੀ_level == ਸਥਿਤੀ_ਤਰੱਕੀ)
        {
          xtime_t ਤਰੱਕੀ_ਸਮਾਂ = gethrxtime ();
          ਜੇ (ਅਗਲਾ_ਸਮਾਂ <= ਤਰੱਕੀ_ਸਮਾਂ)
            {
              print_xfer_stats (ਤਰੱਕੀ_ਸਮਾਂ);
              ਅਗਲਾ_ਸਮਾਂ += XTIME_PRECISION;
            }
        }

      ਜੇ (r_partial + r_ਭਰਿਆ >= max_ਰਿਕਾਰਡਾਂ + !!max_ਬਾਈਟਾਂ)
        ਰੋਕੋ;

      /* Zero the buffer before reading, so that if we get a read error,
         whatever data we are able to read is followed by zeros.
         This minimizes data loss. */
      ਜੇ ((conversions_ਨਕਾਬ & C_SYNC) && (conversions_ਨਕਾਬ & C_NOERROR))
        memset (ibuf,
                (conversions_ਨਕਾਬ & (C_ਖੰਡ | C_UNBLOCK)) ? ' ' : '\0',
                ਨਿਵੇਸ਼_blocksize);

      ਜੇ (r_partial + r_ਭਰਿਆ >= max_ਰਿਕਾਰਡਾਂ)
        nread = iread_fnc (STDIN_FILENO, ibuf, max_ਬਾਈਟਾਂ);
      ਨਹੀਂਤਾਂ
        nread = iread_fnc (STDIN_FILENO, ibuf, ਨਿਵੇਸ਼_blocksize);

      ਜੇ (nread > 0)
        {
          advance_ਨਿਵੇਸ਼_ਵਿੱਥ (nread);
          ਜੇ (i_nocache)
            invalidate_cache (STDIN_FILENO, nread);
        }
      ਨਹੀਂਤਾਂ ਜੇ (nread == 0)
        {
          i_nocache_eof |= i_nocache;
          o_nocache_eof |= o_nocache && ! (conversions_ਨਕਾਬ & C_NOTRUNC);
          ਰੋਕੋ;			/* EOF.  */
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (!(conversions_ਨਕਾਬ & C_NOERROR) || ਸਥਿਤੀ_level != ਸਥਿਤੀ_NONE)
            diagnose (errno, _("error reading %s"), quoteaf (ਨਿਵੇਸ਼_file));

          ਜੇ (conversions_ਨਕਾਬ & C_NOERROR)
            {
              print_stats ();
              idx_t bad_portion = ਨਿਵੇਸ਼_blocksize - partread;

              /* We already know this data is not cached,
                 but call this so that correct offsets are maintained.  */
              invalidate_cache (STDIN_FILENO, bad_portion);

              /* Seek past the bad block if possible. */
              ਜੇ (!advance_ਨਿਵੇਸ਼_after_read_error (bad_portion))
                {
                  exit_ਸਥਿਤੀ = EXIT_FAILURE;

                  /* Suppress duplicate diagnostics.  */
                  ਨਿਵੇਸ਼_seekable = false;
                  ਨਿਵੇਸ਼_seek_errno = ESPIPE;
                }
              ਜੇ ((conversions_ਨਕਾਬ & C_SYNC) && !partread)
                /* Replace the missing input with null bytes and
                   proceed normally.  */
                nread = 0;
              ਨਹੀਂਤਾਂ
                ਜਾਰੀ;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Write any partial block. */
              exit_ਸਥਿਤੀ = EXIT_FAILURE;
              ਰੋਕੋ;
            }
        }

      n_ਬਾਈਟਾਂ_read = nread;

      ਜੇ (n_ਬਾਈਟਾਂ_read < ਨਿਵੇਸ਼_blocksize)
        {
          r_partial++;
          partread = n_ਬਾਈਟਾਂ_read;
          ਜੇ (conversions_ਨਕਾਬ & C_SYNC)
            {
              ਜੇ (!(conversions_ਨਕਾਬ & C_NOERROR))
                /* If C_NOERROR, we zeroed the block before reading. */
                memset (ibuf + n_ਬਾਈਟਾਂ_read,
                        (conversions_ਨਕਾਬ & (C_ਖੰਡ | C_UNBLOCK)) ? ' ' : '\0',
                        ਨਿਵੇਸ਼_blocksize - n_ਬਾਈਟਾਂ_read);
              n_ਬਾਈਟਾਂ_read = ਨਿਵੇਸ਼_blocksize;
            }
        }
      ਨਹੀਂਤਾਂ
        {
          r_ਭਰਿਆ++;
          partread = 0;
        }

      ਜੇ (ibuf == obuf)		/* If not C_TWOBUFS. */
        {
          idx_t nwritten = iwrite (STDOUT_FILENO, obuf, n_ਬਾਈਟਾਂ_read);
          w_ਬਾਈਟਾਂ += nwritten;
          ਜੇ (nwritten != n_ਬਾਈਟਾਂ_read)
            {
              diagnose (errno, _("error writing %s"), quoteaf (output_file));
              ਜੇ (nwritten != 0)
                w_partial++;
              ਵਾਪਸ EXIT_FAILURE;
            }
          ਨਹੀਂਤਾਂ ਜੇ (n_ਬਾਈਟਾਂ_read == ਨਿਵੇਸ਼_blocksize)
            w_ਭਰਿਆ++;
          ਨਹੀਂਤਾਂ
            w_partial++;
          ਜਾਰੀ;
        }

      /* Do any translations on the whole buffer at once.  */

      ਜੇ (translation_needed)
        translate_ਬਫਰ (ibuf, n_ਬਾਈਟਾਂ_read);

      ਜੇ (conversions_ਨਕਾਬ & C_SWAB)
        bufstart = swab_ਬਫਰ (ibuf, &n_ਬਾਈਟਾਂ_read, &saved_byte);
      ਨਹੀਂਤਾਂ
        bufstart = ibuf;

      ਜੇ (conversions_ਨਕਾਬ & C_ਖੰਡ)
        copy_with_ਖੰਡ (bufstart, n_ਬਾਈਟਾਂ_read);
      ਨਹੀਂਤਾਂ ਜੇ (conversions_ਨਕਾਬ & C_UNBLOCK)
        copy_with_unblock (bufstart, n_ਬਾਈਟਾਂ_read);
      ਨਹੀਂਤਾਂ
        copy_simple (bufstart, n_ਬਾਈਟਾਂ_read);
    }

  /* If we have a char left as a result of conv=swab, output it.  */
  ਜੇ (0 <= saved_byte)
    {
      ਅੱਖਰ saved_char = saved_byte;
      ਜੇ (conversions_ਨਕਾਬ & C_ਖੰਡ)
        copy_with_ਖੰਡ (&saved_char, 1);
      ਨਹੀਂਤਾਂ ਜੇ (conversions_ਨਕਾਬ & C_UNBLOCK)
        copy_with_unblock (&saved_char, 1);
      ਨਹੀਂਤਾਂ
        output_char (saved_char);
    }

  ਜੇ ((conversions_ਨਕਾਬ & C_ਖੰਡ) && ਖਾਨਾ > 0)
    {
      /* If the final input line didn't end with a '\n', pad
         the output block to 'conversion_blocksize' chars.  */
      ਲਈ (idx_t i = ਖਾਨਾ; i < conversion_blocksize; i++)
        output_char (space_character);
    }

  ਜੇ (ਖਾਨਾ && (conversions_ਨਕਾਬ & C_UNBLOCK))
    {
      /* If there was any output, add a final '\n'.  */
      output_char (newline_character);
    }

  /* Write out the last block. */
  ਜੇ (oc != 0)
    {
      idx_t nwritten = iwrite (STDOUT_FILENO, obuf, oc);
      w_ਬਾਈਟਾਂ += nwritten;
      ਜੇ (nwritten != 0)
        w_partial++;
      ਜੇ (nwritten != oc)
        {
          diagnose (errno, _("error writing %s"), quoteaf (output_file));
          ਵਾਪਸ EXIT_FAILURE;
        }
    }

  /* If the last write was converted to a seek, then for a regular file
     or shared memory object, ftruncate to extend the size.  */
  ਜੇ (final_op_was_seek)
    {
      ਬਣਤਰ stat stdout_stat;
      ਜੇ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
        {
          diagnose (errno, _("cannot fstat %s"), quoteaf (output_file));
          ਵਾਪਸ EXIT_FAILURE;
        }
      ਜੇ (S_ISREG (stdout_stat.st_ਬਹੁਲਕ) || S_TYPEISSHM (&stdout_stat))
        {
          off_t output_ਵਿੱਥ = lseek (STDOUT_FILENO, 0, SEEK_CUR);
          ਜੇ (0 <= output_ਵਿੱਥ && stdout_stat.st_ਮਾਪ < output_ਵਿੱਥ)
            {
              ਜੇ (iftruncate (STDOUT_FILENO, output_ਵਿੱਥ) != 0)
                {
                  diagnose (errno, _("failed to truncate to %jd bytes"
                                     " in output file %s"),
                            (intmax_t) {output_ਵਿੱਥ}, quoteaf (output_file));
                  ਵਾਪਸ EXIT_FAILURE;
                }
            }
        }
    }

  /* fdatasync/fsync can take a long time, so issue a final progress
     indication now if progress has been made since the previous indication.  */
  ਜੇ (conversions_ਨਕਾਬ & (C_FDATASYNC | C_FSYNC)
      && ਸਥਿਤੀ_level == ਸਥਿਤੀ_ਤਰੱਕੀ
      && 0 <= reported_w_ਬਾਈਟਾਂ && reported_w_ਬਾਈਟਾਂ < w_ਬਾਈਟਾਂ)
    print_xfer_stats (0);

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}

/* Synchronize output according to conversions_mask.
   Do this even if w_bytes is zero, as fsync and fdatasync
   flush out write requests from other processes too.
   Clear bits in conversions_mask so that synchronization is done only once.
   Return zero if successful, an exit status otherwise.  */

ਸਥਿਰ ਪੂਰਨ
synchronize_output (ਖਾਲੀ)
{
  ਪੂਰਨ exit_ਸਥਿਤੀ = 0;
  ਪੂਰਨ ਨਕਾਬ = conversions_ਨਕਾਬ;
  conversions_ਨਕਾਬ &= ~ (C_FDATASYNC | C_FSYNC);

  ਜੇ ((ਨਕਾਬ & C_FDATASYNC) && ifdatasync (STDOUT_FILENO) != 0)
    {
      ਜੇ (errno != ENOSYS && errno != EINVAL)
        {
          diagnose (errno, _("fdatasync failed for %s"), quoteaf (output_file));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਨਕਾਬ |= C_FSYNC;
    }

  ਜੇ ((ਨਕਾਬ & C_FSYNC) && ifsync (STDOUT_FILENO) != 0)
    {
      diagnose (errno, _("fsync failed for %s"), quoteaf (output_file));
      ਵਾਪਸ EXIT_FAILURE;
    }

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  off_t ਵਿੱਥ;

  install_signal_handlers ();

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  /* Arrange to close stdout if parse_long_options exits.  */
  atexit (maybe_close_stdout);

  ਪੰਨਾ_ਮਾਪ = getpagesize ();

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE, ਸੰਸਕਰਣ,
                                   true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);
  close_stdout_required = false;

  /* Initialize translation table to identity translation. */
  ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
    trans_ਸਾਰਣੀ[i] = i;

  /* Decode arguments. */
  scanargs (argc, argv);

  apply_translations ();

  ਜੇ (ਨਿਵੇਸ਼_file == ਨਿਰਾ)
    {
      ਨਿਵੇਸ਼_file = _("standard input");
      ਸੈੱਟ_fd_flags (STDIN_FILENO, ਨਿਵੇਸ਼_flags, ਨਿਵੇਸ਼_file);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (ifd_reopen (STDIN_FILENO, ਨਿਵੇਸ਼_file, O_RDONLY | ਨਿਵੇਸ਼_flags, 0) < 0)
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (ਨਿਵੇਸ਼_file));
    }

  ਵਿੱਥ = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ਨਿਵੇਸ਼_seekable = (0 <= ਵਿੱਥ);
  ਨਿਵੇਸ਼_ਵਿੱਥ = MAX (0, ਵਿੱਥ);
  ਨਿਵੇਸ਼_seek_errno = errno;

  ਜੇ (output_file == ਨਿਰਾ)
    {
      output_file = _("standard output");
      ਸੈੱਟ_fd_flags (STDOUT_FILENO, output_flags, output_file);
    }
  ਨਹੀਂਤਾਂ
    {
      mode_t perms = ਬਹੁਲਕ_RW_UGO;
      ਪੂਰਨ opts
        = (output_flags
           | (conversions_ਨਕਾਬ & C_NOCREAT ? 0 : O_CREAT)
           | (conversions_ਨਕਾਬ & C_EXCL ? O_EXCL : 0)
           | (seek_ਰਿਕਾਰਡਾਂ || (conversions_ਨਕਾਬ & C_NOTRUNC) ? 0 : O_TRUNC));

      off_t ਮਾਪ;
      ਜੇ ((ckd_mul (&ਮਾਪ, seek_ਰਿਕਾਰਡਾਂ, output_blocksize)
           || ckd_ਜੋੜੋ (&ਮਾਪ, seek_ਬਾਈਟਾਂ, ਮਾਪ))
          && !(conversions_ਨਕਾਬ & C_NOTRUNC))
        error (EXIT_FAILURE, 0,
               _("offset too large: "
                 "cannot truncate to a length of seek=%jd"
                 " (%td-byte) blocks"),
               seek_ਰਿਕਾਰਡਾਂ, output_blocksize);

      /* Open the output file with *read* access only if we might
         need to read to satisfy a 'seek=' request.  If we can't read
         the file, go ahead with write-only access; it might work.  */
      ਜੇ ((! seek_ਰਿਕਾਰਡਾਂ
           || ifd_reopen (STDOUT_FILENO, output_file, O_RDWR | opts, perms) < 0)
          && (ifd_reopen (STDOUT_FILENO, output_file, O_WRONLY | opts, perms)
              < 0))
        error (EXIT_FAILURE, errno, _("failed to open %s"),
               quoteaf (output_file));

      ਜੇ (seek_ਰਿਕਾਰਡਾਂ != 0 && !(conversions_ਨਕਾਬ & C_NOTRUNC))
        {
          ਜੇ (iftruncate (STDOUT_FILENO, ਮਾਪ) != 0)
            {
              /* Complain only when ftruncate fails on a regular file, a
                 directory, or a shared memory object, as POSIX 1003.1-2004
                 specifies ftruncate's behavior only for these file types.
                 For example, do not complain when Linux kernel 2.4 ftruncate
                 fails on /dev/fd0.  */
              ਪੂਰਨ ftruncate_errno = errno;
              ਬਣਤਰ stat stdout_stat;
              ਜੇ (ifstat (STDOUT_FILENO, &stdout_stat) != 0)
                error (EXIT_FAILURE, errno, _("cannot fstat %s"),
                       quoteaf (output_file));
              ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (stdout_stat.st_ਬਹੁਲਕ)
                       || S_ISDIR (stdout_stat.st_ਬਹੁਲਕ)
                       || S_TYPEISSHM (&stdout_stat))
                {
                  intmax_t isize = ਮਾਪ;
                  error (EXIT_FAILURE, ftruncate_errno,
                         _("failed to truncate to %jd bytes"
                           " in output file %s"),
                         isize, quoteaf (output_file));
                }
            }
        }
    }

  ਸ਼ੁਰੂ_ਸਮਾਂ = gethrxtime ();
  ਅਗਲਾ_ਸਮਾਂ = ਸ਼ੁਰੂ_ਸਮਾਂ + XTIME_PRECISION;

  ਪੂਰਨ copy_ਸਥਿਤੀ = dd_copy ();
  ਪੂਰਨ sync_ਸਥਿਤੀ = synchronize_output ();
  ਪੂਰਨ exit_ਸਥਿਤੀ = copy_ਸਥਿਤੀ | sync_ਸਥਿਤੀ;

  ਜੇ (max_ਰਿਕਾਰਡਾਂ == 0 && max_ਬਾਈਟਾਂ == 0)
    {
      /* Special case to invalidate cache to end of file.  */
      ਜੇ (i_nocache && !invalidate_cache (STDIN_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (ਨਿਵੇਸ਼_file));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਜੇ (o_nocache && !invalidate_cache (STDOUT_FILENO, 0))
        {
          diagnose (errno, _("failed to discard cache for: %s"),
                    quotef (output_file));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* Invalidate any pending region or to EOF if appropriate.  */
      ਜੇ (i_nocache || i_nocache_eof)
        invalidate_cache (STDIN_FILENO, 0);
      ਜੇ (o_nocache || o_nocache_eof)
        invalidate_cache (STDOUT_FILENO, 0);
    }

  ਅੰਤ_ਉੱਪਰ ();
  main_exit (exit_ਸਥਿਤੀ);
}
